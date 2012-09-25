/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Elizabeth M Smith <auroraeosrose@gmail.com>                  |
  +----------------------------------------------------------------------+
*/

#include "php_wingui.h"
#include "messaging.h"

zend_class_entry *ce_wingui_messaging;

struct _wingui_callback_t {
	zend_fcall_info callback_info;
	zend_fcall_info_cache callback_cache;
	char *src_filename;
	uint src_lineno;
	int send_return;
	int send_args;
};

/* ----------------------------------------------------------------
    Win\Gui\Messaging C API
------------------------------------------------------------------*/
/* {{{ Helper function that asks "is this a statusbar window?"
   This is a workaround for statusbars not getting a WM_SIZE posted
   when a parent is resized and is used by the window procedure */
static BOOL CALLBACK wingui_check_for_statusbar(HWND hwndChild, LPARAM lParam) 
{ 
	char buf[20] = {0};

	if (!GetClassName(hwndChild, buf, 20)) {
		return FALSE;
	}

	if (strcmp(buf, STATUSCLASSNAME) == 0) {
		PostMessage(hwndChild, WM_SIZE, (WPARAM) 0, (LPARAM) 0);
	}

	return TRUE;
}
/* }}} */

/* {{{ wingui_messaging_destructor_helper - walks over a hashtable containing callbacks and cleans up */
void wingui_messaging_destructor_helper(HashTable *registered_callbacks TSRMLS_DC)
{
	HashTable **callback_table;

	for(zend_hash_internal_pointer_reset(registered_callbacks);
		zend_hash_get_current_data(registered_callbacks, (void**)&callback_table) == SUCCESS;
		zend_hash_move_forward(registered_callbacks)) {

		wingui_callback_t **callback;

		for(zend_hash_internal_pointer_reset(*callback_table);
			zend_hash_get_current_data(*callback_table, (void**)&callback) == SUCCESS;
			zend_hash_move_forward(*callback_table)) {
			if ((*callback)->callback_info.params) {

				zval ***params  = (*callback)->callback_info.params;
				int param_count = (*callback)->callback_info.param_count;

				while (--param_count) {
					/* setMsgHandler setup two refcounts to avoid getting dereferenced by GC */
					zval_ptr_dtor(params[param_count]);
					zval_ptr_dtor(params[param_count]);
				}
			}

			zval_ptr_dtor(&((*callback)->callback_info.function_name));
			if ((*callback)->callback_info.object_ptr) {
				zval_ptr_dtor(&((*callback)->callback_info.object_ptr));
			}
		}
		zend_hash_destroy(*callback_table);
		FREE_HASHTABLE(*callback_table);
	}
}
/* }}} */

/* Helper function for connect methods */
int wingui_messaging_connect_helper(HashTable* callback_table, int message_code, zval*** args, int argc, zend_fcall_info finfo, zend_fcall_info_cache fcache, int send_args, int send_return TSRMLS_DC)
{
	HashTable *message, **ptr_message;
	wingui_callback_t *store;
	int i;
	ulong key;

	/* grab message hash table or create it */
	if(zend_hash_index_find(callback_table, message_code, (void **)&ptr_message) == SUCCESS) {
		message = *ptr_message;
	} else {
			ALLOC_HASHTABLE(message);
			zend_hash_init(message, 0, NULL, NULL, 0);
			zend_hash_index_update(callback_table, message_code, (void **)&message, sizeof(HashTable *), NULL);
	}

	/* walk the arrrrgs and addref em - weee! - these are free'd in cleanup so we don't leak like a freaking sieve */
	if (argc) {
		for(i = 0; i < argc; i++) {
				zval **newppzval = emalloc(sizeof(zval*));
				MAKE_STD_ZVAL(*newppzval);
				ZVAL_ZVAL(*newppzval, *args[i], 1, 0);
				Z_SET_REFCOUNT_PP(newppzval, 2);
				args[i] = newppzval;
		}
	}
	store = emalloc(sizeof(wingui_callback_t));

	store->callback_info = finfo;
	if (store->callback_info.object_ptr) {
			zval_add_ref(&(store->callback_info.object_ptr));
	}
	zval_add_ref(&(store->callback_info.function_name));
	store->callback_cache = fcache;

	store->src_filename = estrdup(zend_get_executed_filename(TSRMLS_C));
	store->src_lineno = zend_get_executed_lineno(TSRMLS_C);
	store->callback_info.params      = args;
	store->callback_info.param_count = argc;

	store->send_args = send_args;
	store->send_return = send_return;

	key = zend_hash_next_free_element(message);
	zend_hash_next_index_insert(message, (void **) &store, sizeof(wingui_callback_t*), NULL);
	return key;
}
/* }}} */

/*  Does the callback dispatching, a helper function used by the default window procedure
    The dispatch happens in two steps - first it calls any attached callbacks
    in order (remember attached messages are pushed onto the top so FILO order)
    and then any "default" registered handlers are called

    This can be short circuited any time by a "true" return from a callback, which
    will halt ALL the bubbling.  Return values are available if a callback is connected
    with the full variant or default callback ONLY */
zval* wingui_winproc_callback_dispatch(wingui_window_object *window_object, int msg, zval ***extra,
	int extra_count, zend_bool *stop_default TSRMLS_DC)
{
	zval *return_value = NULL, ***param_list = NULL;
	HashTable **callbacks = NULL;
	int param_list_count = extra_count + 1;
	const char *callback_method;

	/* Attached callbacks are used first */
	if((zend_hash_num_elements(wingui_window_get_callbacks_struct(window_object)) > 0) &&
		(zend_hash_index_find(wingui_window_get_callbacks_struct(window_object), msg, (HashTable **) &callbacks) == SUCCESS)) {
		HashPosition pos;
		wingui_callback_t **callback;

		/* iterate BACKWARD and call and optionally bail if bubble ends up as true */ 
		for(zend_hash_internal_pointer_end_ex(*callbacks, &pos);
			zend_hash_get_current_data_ex(*callbacks, (wingui_callback_t **) &callback, &pos) == SUCCESS;
			zend_hash_move_backwards_ex(*callbacks, &pos)) {

			/* in loop declarations */
			zend_fcall_info *finfo = &((*callback)->callback_info);
			zval ***orig_params = finfo->params, ***params = orig_params;
			int orig_params_count = finfo->param_count, param_count = orig_params_count;
			zval *bubble_value = NULL;

			finfo->retval_ptr_ptr = &bubble_value;

			/* The parameters list is only created/allocated on demand but is used for all callbacks */
			if (!param_list) {
				param_list = safe_emalloc(param_list_count, sizeof(zval**), 0);
				param_list[0] = emalloc(sizeof(zval*));
				ALLOC_INIT_ZVAL(*param_list[0]);
				Z_ISREF_P(*param_list[0]);
				memcpy(param_list + 1, extra, extra_count * sizeof(zval**));
			}

			/* start the crazy args manipulation
			send_args is true, send_return is true, orig_params_count is > 0 */
			if ((*callback)->send_args && (*callback)->send_return && orig_params_count) {
				/* Two sets of parameters to mix with param_list version */
				param_count = param_list_count + orig_params_count;
				params = safe_emalloc(param_count, sizeof(zval**), 0);

				memcpy(params, param_list, param_list_count * sizeof(zval**));
				memcpy(params + param_list_count, orig_params, orig_params_count * sizeof(zval**));

			/* send_args is true, send_return is true, orig_params_count is 0 */
			} else if((*callback)->send_args && (*callback)->send_return) {
				param_count = param_list_count;
				params = param_list;
			/* send_args is true, send_return is false, orig_params_count > 0 */
			} else if((*callback)->send_args && orig_params_count && extra_count) {
				/* Two sets of parameters to mix with extra version */
				param_count = extra_count + orig_params_count;
				params = safe_emalloc(param_count, sizeof(zval**), 0);

				memcpy(params, extra, extra_count * sizeof(zval**));
				memcpy(params + extra_count, orig_params, orig_params_count * sizeof(zval**));
			/* send_args is true, send return is false, orig_params_count is 0 */
			} else if((*callback)->send_args) {
				param_count = extra_count;
				params = extra;
			/* send_args is false, send_return is false, use the original params is default */
			}

			/* Place params where they go */
			finfo->params = params;
			finfo->param_count = param_count;

			if (SUCCESS == zend_call_function(finfo, &((**callback).callback_cache) TSRMLS_CC)) {
				/* returning true will break the callback chain */
				if(bubble_value && Z_TYPE_P(bubble_value) == IS_BOOL && Z_BVAL_P(bubble_value))
				{
					*stop_default = TRUE;
					finfo->params = orig_params;
					finfo->param_count = orig_params_count;
					if (param_list_count && orig_params_count) {
						efree(params);
					}
					if (bubble_value) {
						zval_ptr_dtor(&bubble_value);
					}
					break;
				}
			} else {
				/* If we ever hit this, things are very wrong in Kansas, blow up nicely */
				php_error(E_RECOVERABLE_ERROR, "Error calling %s for msg:%d", finfo->function_name, msg);
			}

			finfo->params = orig_params;
			finfo->param_count = orig_params_count;

			if (param_list_count && orig_params_count) {
				efree(params);
			}
			if (bubble_value) {
				zval_ptr_dtor(&bubble_value);
			}

		}
	}

	/* first, is the stop_default off and do we have an item in the callback map that matches a method in the class - if so final callback time 
	if(*stop_default == FALSE && (zend_hash_index_find(window_object->callback_map, msg, (void **) &callback_method) == SUCCESS) && 
		zend_hash_num_elements(&Z_OBJCE_P(window_object->object_zval)->function_table) > 0 &&
		zend_hash_exists(&Z_OBJCE_P(window_object->object_zval)->function_table, callback_method, strlen(callback_method) + 1)) {
		
		zval *function;
		zval *bubble_value = NULL;

		MAKE_STD_ZVAL(function);
		ZVAL_STRING(function, callback_method, 1);

		/* The parameters list is only created/allocated on demand but is used for all callbacks 
		if (!param_list) {
			param_list = safe_emalloc(param_list_count, sizeof(zval**), 0);
			param_list[0] = emalloc(sizeof(zval*));
			ALLOC_INIT_ZVAL(*param_list[0]);
			Z_ISREF_P(*param_list[0]);
			memcpy(param_list + 1, extra, extra_count * sizeof(zval**));
		}

		if (call_user_function_ex(
					&Z_OBJCE_P(window_object->object_zval)->function_table,
					&window_object->object_zval,
					function,
					&bubble_value,
					param_list_count,
					param_list,
					0,
					NULL TSRMLS_CC
					)  == FAILURE) {
				/* If we ever hit this, things are very wrong in Kansas, blow up nicely 
				php_error(E_RECOVERABLE_ERROR, "Error calling %s for msg:%d", callback_method, msg);
		}

		zval_ptr_dtor(&function);
		/* returning true will break the callback chain 
		if(bubble_value && Z_TYPE_P(bubble_value) == IS_BOOL && Z_BVAL_P(bubble_value)) {
			*stop_default = TRUE;
		}
		if (bubble_value) {
			zval_ptr_dtor(&bubble_value);
		}
	} */

	if (param_list) {
		return_value = *param_list[0];
		efree(param_list);
	}

	return return_value;
}
/* }}} */

/* This is the window procedure used for all window classes and controls
   dispatches messages and uses four types of stored callbacks to process data */
LRESULT CALLBACK wingui_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	zval ***extra = NULL;
	int extra_count = 0;
	zend_bool stop_default = FALSE;
	zval *return_value = NULL;
	WNDPROC proc;
	wingui_window_object *window_object = NULL;
#ifdef ZTS
	TSRMLS_D;
#endif;

	/* This is ugly as sin.  This says take all the child windows,
	   check if they are a statusbar send them a WM_SIZE if so
	   Why? because the default proc is too stupid to do this */
	if(msg == WM_SIZE) {
		EnumChildWindows(hwnd, wingui_check_for_statusbar, (LPARAM) NULL);
	}

	/* we have two ways to get a window object, first we check to see if it's stored,
	   then we try to get it from a CREATESTRUCT, if neither work we bail
	   by sending back defwindowproc handling of the message */
	window_object = (wingui_window_object *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (!window_object) {
		if (msg == WM_CREATE || msg == WM_NCCREATE) {
			window_object = (wingui_window_object *)((*(CREATESTRUCT*)(lParam)).lpCreateParams);
			if (!window_object) {
				return CallWindowProc(DefWindowProc, hwnd, msg, wParam, lParam);
			}
		} else {
			return CallWindowProc(DefWindowProc, hwnd, msg, wParam, lParam);
		}
	}

#ifdef ZTS
	TSRMLS_C = wingui_window_get_tsrm(window_object);
#endif

	extra = NULL;
	//extra = window_object->messages_cracker(msg, &wParam, &lParam, &extra_count TSRMLS_CC);

	if(window_object) {
		return_value = wingui_winproc_callback_dispatch(window_object, msg, extra, extra_count, &stop_default TSRMLS_CC);
	}

	if (stop_default == FALSE) {
		//window_object->messages_packer(msg, &wParam, &lParam, extra TSRMLS_CC);
	}

	if (extra) {
		//wingui_callback_extra_zvals_dtor(extra_count, extra);
	}
	proc = wingui_window_get_proc(window_object);

	if(proc == NULL) {
		if (IsWindowUnicode(hwnd)) {
			proc = DefWindowProcW;
		} else {
			proc = DefWindowProcA;
		}
	}

	/* return is based on if we need to call the default proc handling, and if we have an overwritten
	   return value to use instead of the default value */
	if (stop_default && return_value) {
		//return window_object->messages_results(msg, return_value TSRMLS_CC);
	} else if (stop_default) {
		//return (LRESULT) NULL;
	} else if (return_value) {
		//CallWindowProc(window_object->window_proc, hwnd, msg, wParam, lParam);
		//return window_object->messages_results(msg, return_value TSRMLS_CC);
		//return CallWindowProc(window_object->window_proc, hwnd, msg, wParam, lParam);
	} else {
		//return CallWindowProc(window_object->window_proc, hwnd, msg, wParam, lParam);
	}
	return CallWindowProc(proc, hwnd, msg, wParam, lParam);
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Messaging Userland API
------------------------------------------------------------------*/

/* {{{ proto int Win\Gui\Messaging->connect(int message, callback [, optional user args])
       Register a callback for a message, this version will push additional
       args to the message handler if they are available for a specific message */
PHP_METHOD(WinGuiMessaging, connect)
{
	long message_code;
	zend_fcall_info finfo;
	zend_fcall_info_cache fcache;
	zval ***args = NULL;
	int argc = 0;
	
	PHP_WINGUI_EXCEPTIONS
	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lf*", &message_code, &finfo, &fcache, &args, &argc)) {
		PHP_WINGUI_RESTORE_ERRORS
		return;
	}
	PHP_WINGUI_RESTORE_ERRORS

	RETURN_LONG(wingui_messaging_connect_helper(wingui_window_get_callbacks(&getThis() TSRMLS_CC), message_code, args, argc, finfo, fcache, 1, 0 TSRMLS_CC));
}
/* }}} */

/* {{{ proto int Win\Gui\Messaging->connectFull(int message, callback [, optional user args])
       Register a callback for a message, this version will push additional
       args to the message handler if they are available for a specific message and 
	   will also send a return parameter, you should accept as a reference that can be altered in your callback */
PHP_METHOD(WinGuiMessaging, connectFull)
{
	long message_code;
	zend_fcall_info finfo;
	zend_fcall_info_cache fcache;
	zval ***args = NULL;
	int argc = 0;

	PHP_WINGUI_EXCEPTIONS
	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lf*", &message_code, &finfo, &fcache, &args, &argc)) {
		PHP_WINGUI_RESTORE_ERRORS
		return;
	}
	PHP_WINGUI_RESTORE_ERRORS

	RETURN_LONG(wingui_messaging_connect_helper(wingui_window_get_callbacks(&getThis() TSRMLS_CC), message_code, args, argc, finfo, fcache, 1, 1 TSRMLS_CC));
}
/* }}} */

/* {{{ proto int Win\Gui\Messaging->connectSimple(int message, callback [, optional user args])
       Register a callback for a message, this version will only send optional user args */
PHP_METHOD(WinGuiMessaging, connectSimple)
{
	long message_code;
	zend_fcall_info finfo;
	zend_fcall_info_cache fcache;
	zval ***args = NULL;
	int argc = 0;

	PHP_WINGUI_EXCEPTIONS
	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lf*", &message_code, &finfo, &fcache, &args, &argc)) {
		PHP_WINGUI_RESTORE_ERRORS
		return;
	}
	PHP_WINGUI_RESTORE_ERRORS

	RETURN_LONG(wingui_messaging_connect_helper(wingui_window_get_callbacks(&getThis() TSRMLS_CC), message_code, args, argc, finfo, fcache, 0, 0 TSRMLS_CC));
}
/* }}} */

/* {{{ proto bool Win\Gui\Messaging->disconnect(int message, int id)
     Remove a callback for a message */
PHP_METHOD(WinGuiMessaging, disconnect)
{
	int msg, id;
	HashTable *message, **ptr_message;
	        
	PHP_WINGUI_EXCEPTIONS
	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &msg, &id)) {
		PHP_WINGUI_RESTORE_ERRORS
		return;
	}
	PHP_WINGUI_RESTORE_ERRORS

	/* grab message hash table or we can't unregister, obviously */
	if(SUCCESS == zend_hash_index_find(wingui_window_get_callbacks(&getThis() TSRMLS_CC), msg, (void **)&ptr_message)) {
		message = *ptr_message;
	} else {
		RETURN_FALSE;
	}

	/* delete it */
	if(SUCCESS == zend_hash_index_del(message, id)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto int Win\Gui\Messaging->get([, int filtermin, int filtermax])
       Retrieves a message posted to the current object
	   The function dispatches incoming sent messages until a posted message is available for retrieval. */
PHP_METHOD(WinGuiMessaging, get)
{
}
/* }}} */

/* {{{ proto int Win\Gui\Messaging->peek([, int filtermin, int filtermax])
		retrieves a message from the window's message queue.
		dispatches incoming sent messages,
		checks the message queue for a posted message, and retrieves the message (if any exist) */
PHP_METHOD(WinGuiMessaging, peek)
{
}
/* }}} */

/* {{{ proto mixed Win\Gui\Messaging->post(int msg[, param1, param2])
     Post a message to a window's queue, will not wait for a return value like send
	false is returned if attempting to do the post failed */
PHP_METHOD(WinGuiMessaging, post)
{
	WPARAM wParam = (WPARAM) NULL;
	LPARAM lParam = (LPARAM) NULL;
	UINT msg;
	
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	/* parse once because we MUST have a msg to mess with */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l*", &msg, &args, &argc) == FAILURE) {
		return;
	}

	if (args) {
		efree(args);
	}

	/* parse again with specific dispatcher 
	if (window_object->messages_dispatch(INTERNAL_FUNCTION_PARAM_PASSTHRU, msg, &wParam, &lParam) == FAILURE) {
		return;
	}

	RETURN_BOOL(PostMessage(window_object->window_handle, msg, (WPARAM) wParam, (LPARAM) lParam));*/
}
/* }}} */

/* {{{ proto mixed Win\Gui\Messaging->send(int msg[, param1, param2])
     Send a message directly to a window and wait for it to return */
PHP_METHOD(WinGuiMessaging, send)
{
	WPARAM wParam = (WPARAM) NULL;
	LPARAM lParam = (LPARAM) NULL;
	LRESULT result;
	UINT msg;
	
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	/* parse once because we MUST have a msg to mess with */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l*", &msg, &args, &argc) == FAILURE) {
		return;
	}

	if (args) {
		efree(args);
	}

	/* parse again with specific dispatcher 
	if (window_object->messages_dispatch(INTERNAL_FUNCTION_PARAM_PASSTHRU, msg, &wParam, &lParam) == FAILURE) {
		return;
	}

	result = SendMessage(window_object->window_handle, msg, (WPARAM) wParam, (LPARAM) lParam);
	
	window_object->messages_zvals(msg, result, return_value TSRMLS_CC);*/
}
/* }}} */

/* {{{ proto mixed Win\Gui\Messaging->sendNotify(int msg[, param1, param2])
     Acts like a send (waits for the processing of the message) in windows in the current thread
	 Acts like a post (returns immediately) in windows NOT in the current thread */
PHP_METHOD(WinGuiMessaging, sendNotify)
{
	WPARAM wParam = (WPARAM) NULL;
	LPARAM lParam = (LPARAM) NULL;
	UINT msg;
	
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	/* parse once because we MUST have a msg to mess with 
	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l*", &msg, &args, &argc) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (args) {
		efree(args);
	}

	/* parse again with specific dispatcher 
	if (window_object->messages_dispatch(INTERNAL_FUNCTION_PARAM_PASSTHRU, msg, &wParam, &lParam) == FAILURE) {
		return;
	}

	RETURN_BOOL(SendNotifyMessage(window_object->window_handle, msg, (WPARAM) wParam, (LPARAM) lParam));*/
}
/* }}} */

/* {{{ proto mixed Win\Gui\Messaging->sendCallback(int msg, mixed callback, mixed data[, param1, param2])
     Send a message directly to a window and wait for it to return, pass the return value and any values passed in 
	 data into the user defined callback. This is an async process */
PHP_METHOD(WinGuiMessaging, sendCallback)
{
}
/* }}} */

/* {{{ proto mixed Win\Gui\Messaging->sendTimeout(int msg, int flags, int timeout[, param1, param2])
     Send a message directly to a window, handle return behavior depending on the flags passed */
PHP_METHOD(WinGuiMessaging, sendTimeout)
{
}
/* }}} */

/* register Messaging methods */
static zend_function_entry wingui_messaging_functions[] = {
	PHP_ABSTRACT_ME(WinGuiMessaging, connect, WinGuiMessaging_connect_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, connectFull, WinGuiMessaging_connectFull_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, connectSimple, WinGuiMessaging_connectSimple_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, disconnect, WinGuiMessaging_disconnect_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, get, WinGuiMessaging_get_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, peek, WinGuiMessaging_peek_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, post, WinGuiMessaging_post_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, send, WinGuiMessaging_send_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, sendNotify, WinGuiMessaging_sendNotify_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, sendCallback, WinGuiMessaging_sendCallback_args)
	PHP_ABSTRACT_ME(WinGuiMessaging, sendTimeout, WinGuiMessaging_sendTimeout_args)
	ZEND_FE_END
};

/* ----------------------------------------------------------------
  Win\Gui\Message LifeCycle Functions
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_messaging)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Messaging", wingui_messaging_functions);
	ce_wingui_messaging = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_messaging->ce_flags |= ZEND_ACC_INTERFACE;

	return SUCCESS;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
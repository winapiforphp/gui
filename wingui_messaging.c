/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Elizabeth Smith <auroraeosrose@php.net>                      |
  +----------------------------------------------------------------------+
*/

#include "php_wingui.h"
#include "zend_exceptions.h"
#include "implement_messaging.h"

/* All the classes in this file */
zend_class_entry *ce_wingui_messaging;

/* the body of this is after stuff that uses it, but it is static and so here not in a header file */
int wingui_messaging_connect_helper(HashTable* callback_table, int message_code, zval*** args, int argc, zend_fcall_info finfo, zend_fcall_info_cache fcache, int send_args, int send_return TSRMLS_DC);

/* ----------------------------------------------------------------
  Win\Gui\Messaging Userland API
------------------------------------------------------------------*/

/* {{{ proto int Win\Gui\Messaging->connect(int message, callback [, optional user args])
       Register a callback for a message, this version will push additional
       args to the message handler if they are available for a specific message */
PHP_METHOD(WinGuiMessaging, connect)
{
	zend_error_handling error_handling;
	long message_code;
	zend_fcall_info finfo;
	zend_fcall_info_cache fcache;
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lf*", &message_code, &finfo, &fcache, &args, &argc) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_LONG(wingui_messaging_connect_helper(window_object->registered_callbacks, message_code, args, argc, finfo, fcache, 1, 0 TSRMLS_CC));
}
/* }}} */

/* {{{ proto int Win\Gui\Messaging->connectFull(int message, callback [, optional user args])
       Register a callback for a message, this version will push additional
       args to the message handler if they are available for a specific message and 
	   will also send a return parameter, you should accept as a reference that can be altered in your callback */
PHP_METHOD(WinGuiMessaging, connectFull)
{
	zend_error_handling error_handling;
	long message_code;
	zend_fcall_info finfo;
	zend_fcall_info_cache fcache;
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lf*", &message_code, &finfo, &fcache, &args, &argc) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_LONG(wingui_messaging_connect_helper(window_object->registered_callbacks, message_code, args, argc, finfo, fcache, 1, 1 TSRMLS_CC));
}
/* }}} */

/* {{{ proto int Win\Gui\Messaging->connectSimple(int message, callback [, optional user args])
       Register a callback for a message, this version will only send optional user args */
PHP_METHOD(WinGuiMessaging, connectSimple)
{
	zend_error_handling error_handling;
	long message_code;
	zend_fcall_info finfo;
	zend_fcall_info_cache fcache;
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lf*", &message_code, &finfo, &fcache, &args, &argc) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_LONG(wingui_messaging_connect_helper(window_object->registered_callbacks, message_code, args, argc, finfo, fcache, 0, 0 TSRMLS_CC));
}
/* }}} */

/* {{{ proto bool Win\Gui\Messaging->disconnect(int message, int id)
     Remove a callback for a message */
PHP_METHOD(WinGuiMessaging, disconnect)
{
	zend_error_handling error_handling;
	int msg, id;
	HashTable *message, **ptr_message;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	        
	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &msg, &id) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	// grab message hash table or we can't unregister, obviously
	if(zend_hash_index_find(window_object->registered_callbacks, msg, (void **)&ptr_message) == SUCCESS) {
		message = *ptr_message;
	} else {
		RETURN_FALSE;
	}

	// unset message with id or fail
	if(zend_hash_index_del(message, id) == SUCCESS) {
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
	zend_error_handling error_handling;
	WPARAM wParam = (WPARAM) NULL;
	LPARAM lParam = (LPARAM) NULL;
	UINT msg;
	
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	/* parse once because we MUST have a msg to mess with */
	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l*", &msg, &args, &argc) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (args) {
		efree(args);
	}

	/* parse again with specific dispatcher */
	if (window_object->messages_dispatch(INTERNAL_FUNCTION_PARAM_PASSTHRU, msg, &wParam, &lParam) == FAILURE) {
		return;
	}

	RETURN_BOOL(PostMessage(window_object->window_handle, msg, (WPARAM) wParam, (LPARAM) lParam));
}
/* }}} */

/* {{{ proto mixed Win\Gui\Messaging->send(int msg[, param1, param2])
     Send a message directly to a window and wait for it to return */
PHP_METHOD(WinGuiMessaging, send)
{
	zend_error_handling error_handling;
	WPARAM wParam = (WPARAM) NULL;
	LPARAM lParam = (LPARAM) NULL;
	LRESULT result;
	UINT msg;
	
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	/* parse once because we MUST have a msg to mess with */
	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l*", &msg, &args, &argc) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (args) {
		efree(args);
	}

	/* parse again with specific dispatcher */
	if (window_object->messages_dispatch(INTERNAL_FUNCTION_PARAM_PASSTHRU, msg, &wParam, &lParam) == FAILURE) {
		return;
	}

	result = SendMessage(window_object->window_handle, msg, (WPARAM) wParam, (LPARAM) lParam);
	
	window_object->messages_zvals(msg, result, return_value TSRMLS_CC);
}
/* }}} */

/* {{{ proto mixed Win\Gui\Messaging->sendNotify(int msg[, param1, param2])
     Acts like a send (waits for the processing of the message) in windows in the current thread
	 Acts like a post (returns immediately) in windows NOT in the current thread */
PHP_METHOD(WinGuiMessaging, sendNotify)
{
	zend_error_handling error_handling;
	WPARAM wParam = (WPARAM) NULL;
	LPARAM lParam = (LPARAM) NULL;
	UINT msg;
	
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	
	/* parse once because we MUST have a msg to mess with */
	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l*", &msg, &args, &argc) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (args) {
		efree(args);
	}

	/* parse again with specific dispatcher */
	if (window_object->messages_dispatch(INTERNAL_FUNCTION_PARAM_PASSTHRU, msg, &wParam, &lParam) == FAILURE) {
		return;
	}

	RETURN_BOOL(SendNotifyMessage(window_object->window_handle, msg, (WPARAM) wParam, (LPARAM) lParam));
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
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Window C API                                           
------------------------------------------------------------------*/
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
					// setMsgHandler setup two refcounts to avoid getting referenced 
					zval_ptr_dtor(params[param_count]);
					zval_ptr_dtor(params[param_count]);
				}
			}

			zval_ptr_dtor(&((*callback)->callback_info.function_name));
			if ((*callback)->callback_info.object_ptr) {
				zval_ptr_dtor(&((*callback)->callback_info.object_ptr));
			}
		}
	}
}
/* }}} */

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
/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
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


/* All the classes in this file */
zend_class_entry *ce_wingui_message;
//zend_class_entry *ce_wingui_message_queuestatus;
//zend_class_entry *ce_wingui_message_insend;
//HashTable wingui_message_prop_handlers;

/* ----------------------------------------------------------------
  Win\Gui\Message Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO(WinGuiMessage_broadcastSystemMessage_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, recipients)
	ZEND_ARG_INFO(0, message)
	/* any number of additional arguments */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiMessage_get_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, window)
	ZEND_ARG_INFO(0, filtermin)
	ZEND_ARG_INFO(0, filtermax)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiMessage_peek_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, window)
	ZEND_ARG_INFO(0, filtermin)
	ZEND_ARG_INFO(0, filtermax)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_sendNotifyMessage_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, window)
	ZEND_ARG_INFO(0, msg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_setExtraInfo_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_translate_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, msg)
ZEND_END_ARG_INFO()

/* {{{ proto bool Win\Gui\Message->__construct(void)
      Cannot construct message */
PHP_METHOD(WinGuiMessage, __construct)
{
	zend_throw_exception(ce_wingui_exception, "Win\\Gui\\Message cannot be constructed, use Win\\Gui\\Message::get()", 0 TSRMLS_CC);
	return;
}
/* }}} */

/* {{{ proto bool Win\Gui\Message::broadcastSystem(int flags, int recipients, int message[, param1, param2])
     sends a message to all items specified, will return message and additional information to send
	 acts similiar to the send and post methods, only to multiple targets */
PHP_METHOD(WinGuiMessage, broadcastSystem)
{
	/*
	long broadcast_value;
	WPARAM wParam = (WPARAM) NULL;
	LPARAM lParam = (LPARAM) NULL;
	UINT msg;
	DWORD recipients = BSM_ALLCOMPONENTS, flags = 0;
	zval ***args = NULL;
	int argc = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll*", &flags, &recipients, &msg, &args, &argc) == FAILURE) {
		return;
	}

	wingui_messages_packer_dispatch(INTERNAL_FUNCTION_PARAM_PASSTHRU, msg, &wParam, &lParam);

	broadcast_value = BroadcastSystemMessage(flags, &recipients, msg, (WPARAM) wParam, (LPARAM) lParam);

	if (broadcast_value == 0) {
		winapi_create_error(GetLastError(), WINGUI_G(hinstance), wingui_exception_get_ce(TSRMLS_C) TSRMLS_CC);
	}
	
	if (broadcast_value > 0) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}

	if (args) {
		efree(args);
	}*/
}
/* }}} */

/* {{{  proto bool Win\Gui\Message::getInputState(void)
		are there mousebutton or keyboard messages in a thread's queue */
PHP_METHOD(WinGuiMessage, getInputState)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(GetInputState());
}
/* }}} */

/* {{{  proto bool Win\Gui\Message::dispatch(void)
		typically used to dispatch a message retrieved by the getMessage function */
PHP_METHOD(WinGuiMessage, dispatch)
{
	/*
	zend_error_handling error_handling;
	zval *msg_zval;
	wingui_message_object *msg_object;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &msg_zval, ce_wingui_message) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	msg_object = (wingui_message_object *)zend_object_store_get_object(msg_zval TSRMLS_CC);

	RETURN_BOOL(DispatchMessage(msg_object->msg));*/
}
/* }}} */

/* {{{  proto bool Win\Gui\Message::get(Win\Gui\Window window [, int filtermin, int filtermax])
		retrieves a message from the calling thread's message queue.
		The function dispatches incoming sent messages until a posted message is available for retrieval. */
PHP_METHOD(WinGuiMessage, get)
{
	/*
	zend_error_handling error_handling;
	HWND handle;
	wingui_window_object *window_object;
	zval *window_zval;
	MSG *msg;
	BOOL worked;
	long minfilter = 0, maxfilter = 0;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	/* Parse stuff out 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|ll", &window_zval, &minfilter, &maxfilter) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	
	/* we can have NULL, -1, or window 
	if(Z_TYPE_P(window_zval) == IS_NULL) {
		handle = NULL;
	} else if(Z_TYPE_P(window_zval) == IS_LONG && Z_LVAL_P(window_zval) == -1) {
		handle = (HWND) -1;
	} else if(Z_TYPE_P(window_zval) == IS_OBJECT && instanceof_function(Z_OBJCE_P(window_zval), ce_wingui_window TSRMLS_CC)) {
		window_object = (wingui_window_object *)wingui_window_object_get(window_zval TSRMLS_CC);
		handle = window_object->window_handle;
	} else {
		zend_throw_exception(ce_wingui_exception, "Win\\Gui\\Message::get() expects parameter 1 to be null, -1, or a Win\\Gui\\Window instance", 0 TSRMLS_CC);
		return;
	}
	msg = ecalloc(1, sizeof(MSG));

	worked = GetMessage(msg, handle, minfilter, maxfilter);
	if (worked) {
		wingui_message_object *message_object;

		object_init_ex(return_value, ce_wingui_message);
		message_object = (wingui_message_object *)zend_object_store_get_object(return_value TSRMLS_CC);
		message_object->msg = msg;
	} else {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
	}*/
}
/* }}} */

/* {{{  proto mixed Win\Gui\Message::getExtraInfo()
		gets the extra message information, use setExtraInfo to set it */
PHP_METHOD(WinGuiMessage, getExtraInfo)
{
	zend_error_handling error_handling;
	LPARAM data;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	data = GetMessageExtraInfo();

	if (data) {
		zval_dtor(return_value);
		*return_value = *(zval *) data;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
	}
}
/* }}} */

/* {{{ proto array Win\Gui\Message::getPos()
     returns an array of x, y coordinates of cursor of last message */
PHP_METHOD(WinGuiMessage, getPos)
{
	DWORD position;
	POINTS points;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	position = GetMessagePos();
	points = MAKEPOINTS(position);

	array_init(return_value);
	add_next_index_long(return_value, points.x);
	add_next_index_long(return_value, points.y);
}
/* }}} */

/* {{{ proto int Win\Gui\Message::getTime()
     elapsed time, in milliseconds, from the time the system was started to the
	 time the message was placed in the queue */
PHP_METHOD(WinGuiMessage, getTime)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_LONG(GetMessageTime());
}
/* }}} */

/* {{{ proto array Win\Gui\Message::getQueueStatus(flags)
      checks to see what types of messages are in the queue */
PHP_METHOD(WinGuiMessage, getQueueStatus)
{
	DWORD status;
	UINT flags;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &flags) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	status = GetQueueStatus(flags);

	array_init(return_value);
	add_next_index_long(return_value, HIWORD(status));
	add_next_index_long(return_value, LOWORD(status));
}
/* }}} */

/* {{{ proto int Win\Gui\Message::inSend()
     determines whether the current window procedure
	 is processing a message that was sent from another thread */
PHP_METHOD(WinGuiMessage, inSend)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_LONG(InSendMessageEx(NULL));
}
/* }}} */

/* {{{  proto bool Win\Gui\Message::peek(Win\Gui\Window window [, int filtermin, int filtermax])
		retrieves a message from the calling thread's message queue.
		The function dispatches incoming sent messages until a posted message is available for retrieval. */
PHP_METHOD(WinGuiMessage, peek)
{
	/*
	zend_error_handling error_handling;
	HWND handle;
	wingui_window_object *window_object;
	zval *window_zval;
	MSG *msg;
	BOOL worked;
	long minfilter = 0, maxfilter = 0;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	/* Parse stuff out 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|ll", &window_zval, &minfilter, &maxfilter) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	
	/* we can have NULL, -1, or window 
	if(Z_TYPE_P(window_zval) == IS_NULL) {
		handle = NULL;
	} else if(Z_TYPE_P(window_zval) == IS_LONG && Z_LVAL_P(window_zval) == -1) {
		handle = (HWND) -1;
	} else if(Z_TYPE_P(window_zval) == IS_OBJECT && instanceof_function(Z_OBJCE_P(window_zval), ce_wingui_window TSRMLS_CC)) {
		window_object = (wingui_window_object *)wingui_window_object_get(window_zval TSRMLS_CC);
		handle = window_object->window_handle;
	} else {
		zend_throw_exception(ce_wingui_exception, "Win\\Gui\\Message::get() expects parameter 1 to be null, -1, or a Win\\Gui\\Window instance", 0 TSRMLS_CC);
		return;
	}
	msg = ecalloc(1, sizeof(MSG));

	worked = GetMessage(msg, handle, minfilter, maxfilter);
	if (worked) {
		wingui_message_object *message_object;

		object_init_ex(return_value, ce_wingui_message);
		message_object = (wingui_message_object *)zend_object_store_get_object(return_value TSRMLS_CC);
		message_object->msg = msg;
	} else {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
	}*/
}
/* }}} */

/* {{{ proto void Win\Gui\Message::postQuit([int exitcode])
     stop message pump and destroy all active windows */
PHP_METHOD(WinGuiMessage, postQuit)
{
	int exit = 0;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &exit) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	PostQuitMessage(exit);
}
/* }}} */

/* {{{ proto bool Win\Gui\Message::sendNotifyMessage(Win\Gui\Window int message[, param1, param2])
     Send a message and wait for it to return if its in the same thread */
PHP_METHOD(WinGuiMessage, sendNotifyMessage)
{
	/* TODO - standard backend for send* parsing
	WPARAM wParam = (WPARAM) NULL;
	LPARAM lParam = (LPARAM) NULL;
	LRESULT result;
	UINT msg;
	
	zval ***args = NULL;
	int argc = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_objects_get_address(getThis() TSRMLS_CC);
	
	/* parse once because we MUST have a msg to mess with 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l*", &msg, &args, &argc) == FAILURE) {
		return;
	}

	// TODO: check dispatch return value
	wingui_messages_packer_dispatch(INTERNAL_FUNCTION_PARAM_PASSTHRU, msg, &wParam, &lParam);

	// TODO: interpret results
	result = SendMessage(window_object->window_handle, msg, (WPARAM) wParam, (LPARAM) lParam);
	*/
}
/* }}} */

/* {{{  proto void Win\Gui\Message::setExtraInfo()
		sets the extra message information, use getExtraInfo to retrieve it */
PHP_METHOD(WinGuiMessage, setExtraInfo)
{
	zend_error_handling error_handling;
	zval *data_zval;
	LPARAM data;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &data_zval) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* if we have data stored right now, we must deref , then ref the new */
	data = GetMessageExtraInfo();
	if (data) {
		Z_DELREF_P((zval *) data);
	}
	Z_ADDREF_P(data_zval);

	SetMessageExtraInfo((LPARAM) data_zval);
}
/* }}} */

/* {{{  proto bool Win\Gui\Message::translate()
		takes a message object and translates virtual-key messages into character messages
		and posts them to the queue */
PHP_METHOD(WinGuiMessage, translate)
{
	/*
	zend_error_handling error_handling;
	zval *msg_zval;
	wingui_message_object *msg_object;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &msg_zval, ce_wingui_message) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	msg_object = (wingui_message_object *)zend_object_store_get_object(msg_zval TSRMLS_CC);

	RETURN_BOOL(TranslateMessage(msg_object->msg)); */
}
/* }}} */

/* {{{  proto bool Win\Gui\Message::wait()
		wait for a message to be put in the queue */
PHP_METHOD(WinGuiMessage, wait)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(WaitMessage());
}
/* }}} */

/* {{{ proto long Win\Gui\Message::run()
     Begins a standard messing loop for UI applications */
PHP_METHOD(WinGuiMessage, run)
{
	HRESULT result;
	HINSTANCE hinstance;
	MSG msg;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	hinstance = GetModuleHandle(NULL);

	while((result = GetMessage(&msg, NULL, 0, 0 )) != 0) {
		if (FAILED(result)) {
			/* Blow up and quit */
			php_error(E_RECOVERABLE_ERROR, "Result of %d for msg:%d at time: %d", result, msg.message, msg.time);
			PostQuitMessage(1);
		} else if (msg.message == PHP_WINGUI_END_MESSAGE) {
			/* our special message, so return */
			return;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

/*
	while((result = GetMessage(&msg, NULL, 0, 0 )) > 0) {
		//if (FAILED(result)) {
		//} else {
			// todo - support accelerator table stuff
			//if(!TranslateAccelerator(hAccelWnd, hAccelTable, &msg))
			//{
				// todo - support dialog stuff
				//if(!hCurrentDlg || !IsDialogMessaging(hCurrentDlg, &msg))
				//{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				//}
			//}
		//}
	} */

	PostQuitMessage(0);
	RETURN_LONG(msg.wParam);
}
 /* }}} */

/* {{{ proto void Win\Gui\Message::end()
     Ends the standard Messaging loop for UI applications */
PHP_METHOD(WinGuiMessage, end)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* Force the messaging pump */
	PostMessage(NULL, PHP_WINGUI_END_MESSAGE, (WPARAM) NULL, (LPARAM) NULL);
}
 /* }}} */

/* register Messaging methods */
static zend_function_entry wingui_message_functions[] = {
	PHP_ME(WinGuiMessage, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiMessage, broadcastSystem, WinGuiMessage_broadcastSystemMessage_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, dispatch, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, get, WinGuiMessage_get_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, getExtraInfo, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, getInputState, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, getPos, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, getTime, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, getQueueStatus, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, inSend, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, peek, WinGuiMessage_peek_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

	PHP_ME(WinGuiMessage, postQuit, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

	PHP_ME(WinGuiMessage, sendNotifyMessage, WinGuiMessage_sendNotifyMessage_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, setExtraInfo, WinGuiMessage_setExtraInfo_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, translate, WinGuiMessage_translate_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, wait, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, run, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessage, end, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Message Custom Object magic                                                 
------------------------------------------------------------------*/
/* cleans up the message object */
void wingui_message_destructor(void *object TSRMLS_DC)
{
	/*
	wingui_message_object *message_object = (wingui_message_object *)object;

	zend_hash_destroy(message_object->std.properties);
	FREE_HASHTABLE(message_object->std.properties);

	if(message_object->msg){
		efree(message_object->msg);
	}
	efree(object);*/
}
/* }}} */

/* {{{ wingui_message_object_new
       creates a new wingui_handle object and takes care of the internal junk */
zend_object_value wingui_message_object_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	/*
	wingui_message_object *message_object;
	zval *tmp;

	message_object = emalloc(sizeof(wingui_message_object));
	message_object->std.ce = ce;
	message_object->std.guards = NULL;
	message_object->msg = NULL;
	message_object->prop_handler = &wingui_message_prop_handlers;

	ALLOC_HASHTABLE(message_object->std.properties);
	zend_hash_init(message_object->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(message_object->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(message_object, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) wingui_message_destructor, NULL TSRMLS_CC);
	message_object->handle = retval.handle;
	retval.handlers = &wingui_object_handlers;*/
	return retval;
}
/* }}} */

/* Gets the right value out of the msg parameter */
int wingui_message_read(void *object, zval *member, zval **retval TSRMLS_DC)
{
	/*
	wingui_message_object *message_object = (wingui_message_object*)object;
	member = wingui_juggle_type(member, IS_STRING TSRMLS_CC);

	if(strcmp(Z_STRVAL_P(member), "window") == 0) {
		if (message_object->msg->hwnd == NULL) {
			ZVAL_NULL(*retval);
		} else {
			wingui_window_object *window_object = (wingui_window_object *)GetWindowLongPtr(message_object->msg->hwnd, GWLP_USERDATA);
			if (window_object == NULL || window_object->object == NULL) {
				/* TODO some day: get the window class
				get the appropriate CE besed on the class returned 
				ZVAL_NULL(*retval);
			} else {
				zval_dtor(*retval);
				**retval = *window_object->object;
				zval_copy_ctor(*retval);
				Z_SET_REFCOUNT_P(*retval, 1);
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "time") == 0) {
		ZVAL_LONG(*retval, message_object->msg->time);
	} else if(strcmp(Z_STRVAL_P(member), "message") == 0) {
		ZVAL_LONG(*retval, message_object->msg->message);
	} else if(strcmp(Z_STRVAL_P(member), "x") == 0) {
		ZVAL_LONG(*retval, message_object->msg->pt.x);
	} else if(strcmp(Z_STRVAL_P(member), "y") == 0){
		ZVAL_LONG(*retval, message_object->msg->pt.y);
	}
	return SUCCESS;*/
	return SUCCESS;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Message LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_message)
{
	zend_class_entry ce, queue_ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Message", wingui_message_functions);
	ce_wingui_message = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_message->create_object  = wingui_message_object_new;

	//zend_hash_init(&wingui_message_prop_handlers, 0, NULL, NULL, 1);
	//wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "window", wingui_message_read, NULL TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "message", wingui_message_read, NULL TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "time", wingui_message_read, NULL TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "x", wingui_message_read, NULL TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "y", wingui_message_read, NULL TSRMLS_CC);

	//INIT_NS_CLASS_ENTRY(queue_ce, PHP_WINGUI_NS, ZEND_NS_NAME("Message", "QueueStatus"), NULL);
	//ce_wingui_message_queuestatus = zend_register_internal_class(&queue_ce TSRMLS_CC);
	//ce_wingui_message_queuestatus->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

	/* These are Queue Status constants 
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"ALLEVENTS", QS_ALLEVENTS);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"ALLINPUT", QS_ALLINPUT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"ALLPOSTMESSAGE", QS_ALLPOSTMESSAGE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"HOTKEY", QS_HOTKEY);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"INPUT", QS_INPUT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"KEY", QS_KEY);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"MOUSE", QS_MOUSE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"MOUSEBUTTON", QS_MOUSEBUTTON);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"MOUSEMOVE", QS_MOUSEMOVE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"PAINT", QS_PAINT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"POSTMESSAGE", QS_POSTMESSAGE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"RAWINPUT", QS_RAWINPUT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"SENDMESSAGE", QS_SENDMESSAGE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_queuestatus,	"TIMER", QS_TIMER);

	/* These are "in send" constants 
	INIT_NS_CLASS_ENTRY(queue_ce, PHP_WINGUI_NS, ZEND_NS_NAME("Message", "InSend"), NULL);
	ce_wingui_message_insend = zend_register_internal_class(&queue_ce TSRMLS_CC);
	ce_wingui_message_insend->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_insend,	"NOSEND", ISMEX_NOSEND);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_insend,	"CALLBACK", ISMEX_CALLBACK);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_insend,	"NOTIFY", ISMEX_NOTIFY);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_insend,	"REPLIED", ISMEX_REPLIED);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_message_insend,	"SEND", ISMEX_SEND);

	/* These are broadcast options */
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSF_ALLOWSFW);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSF_FLUSHDISK);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSF_FORCEIFHUNG);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSF_IGNORECURRENTTASK);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSF_NOHANG);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSF_NOTIMEOUTIFNOTHUNG);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSF_POSTMESSAGE);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSF_QUERY);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSF_SENDNOTIFYMESSAGE);

	/* These are broadcast recipient options */
	////REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSM_ALLCOMPONENTS);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSM_ALLDESKTOPS);
	//REGISTER_WINGUI_CONSTANT(ce_wingui_message,	BSM_APPLICATIONS);

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
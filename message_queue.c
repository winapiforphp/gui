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

zend_class_entry *ce_wingui_message_queue;

/* ----------------------------------------------------------------
  Win\Gui\Message\Queue Userland API
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiMessageQueue_get_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, window)
	ZEND_ARG_INFO(0, filtermin)
	ZEND_ARG_INFO(0, filtermax)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_getExtraInfo_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_getInputState_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_getPos_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_getTime_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_getStatus_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_inSend_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiMessageQueue_peek_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, window)
	ZEND_ARG_INFO(0, filtermin)
	ZEND_ARG_INFO(0, filtermax)
	ZEND_ARG_INFO(0, remove)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_postQuit_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, exit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_register_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, exit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_setExtraInfo_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_wait_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_start_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessageQueue_stop_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

/* {{{  proto bool Win\Gui\Message\Queue::getInputState(void)
        are there mousebutton or keyboard messages in a thread's queue */
PHP_METHOD(WinGuiMessageQueue, getInputState)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(GetInputState());
}
/* }}} */

/* {{{  proto bool Win\Gui\Message\Queue::get(bool window[, int filtermin, int filtermax])
        retrieves a message from the calling thread's message queue.
        retrieves window and all other messages if window is set to true, otherwise retrieves only
        messages not attached to a window
        The function dispatches incoming sent messages until a posted message is available for retrieval. */
PHP_METHOD(WinGuiMessageQueue, get)
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

/* {{{  proto mixed Win\Gui\Message\Queue::getExtraInfo()
		gets the extra message information, use setExtraInfo to set it */
PHP_METHOD(WinGuiMessageQueue, getExtraInfo)
{
	zend_error_handling error_handling;
	LPARAM data;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
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

/* {{{ proto array Win\Gui\Message\Queue::getPos()
     returns an array of x, y coordinates of cursor of last message */
PHP_METHOD(WinGuiMessageQueue, getPos)
{
	DWORD position;
	POINTS points;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
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

/* {{{ proto int Win\Gui\Message\Queue::getTime()
     elapsed time, in milliseconds, from the time the system was started to the
	 time the message was placed in the queue */
PHP_METHOD(WinGuiMessageQueue, getTime)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_LONG(GetMessageTime());
}
/* }}} */

/* {{{ proto array Win\Gui\Message\Queue::getStatus(flags)
      checks to see what types of messages are in the queue */
PHP_METHOD(WinGuiMessageQueue, getStatus)
{
	DWORD status;
	UINT flags;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
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

/* {{{ proto int Win\Gui\Message\Queue::inSend()
     determines whether the current window procedure
     is processing a message that was sent from another thread */
PHP_METHOD(WinGuiMessageQueue, inSend)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_LONG(InSendMessageEx(NULL));
}
/* }}} */

/* {{{  proto bool Win\Gui\Message\Queue::peek(bool window, [int filtermin, int filtermax, int remove])
        retrieves a post message from the calling thread's message queue.
        retrieves window and all other messages if window is set to true, otherwise retrieves only
        messages not attached to a window
        The function dispatches incoming sent messages until a posted message is available for retrieval. */
PHP_METHOD(WinGuiMessageQueue, peek)
{
	/*
	zend_error_handling error_handling;
	HWND handle;
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

/* {{{ proto void Win\Gui\Message\Queue::postQuit([int exitcode])
     stop message pump and destroy all active windows */
PHP_METHOD(WinGuiMessageQueue, postQuit)
{
	int exit = 0;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &exit) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	PostQuitMessage(exit);
}
/* }}} */

/* {{{  proto void Win\Gui\Message\Queue::setExtraInfo()
        sets the extra message information, use getExtraInfo to retrieve it */
PHP_METHOD(WinGuiMessageQueue, setExtraInfo)
{
	zend_error_handling error_handling;
	zval *data_zval;
	LPARAM data;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
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

/* {{{  proto bool Win\Gui\Message\Queue::wait()
        wait for a message to be put in the queue */
PHP_METHOD(WinGuiMessageQueue, wait)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(WaitMessage());
}
/* }}} */

/* {{{ proto long Win\Gui\Message\Queue::start()
     Begins a standard messing loop for UI applications */
PHP_METHOD(WinGuiMessageQueue, start)
{
	HRESULT result;
	HINSTANCE hinstance;
	MSG msg;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
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
		} else if (msg.message == PHP_WINGUI_STOP_LOOP) {
			/* our special message, so return */
			return;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

/*  TODO: add in support for dialogs and accelerator tables when those are completed
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

/* {{{ proto void Win\Gui\Message\Queue::stop()
     Ends the standard Messaging loop for UI applications */
PHP_METHOD(WinGuiMessageQueue, stop)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* Force the messaging pump */
	PostMessage(NULL, PHP_WINGUI_STOP_LOOP, (WPARAM) NULL, (LPARAM) NULL);
}
 /* }}} */

/* register Messaging methods */
static zend_function_entry wingui_message_queue_functions[] = {
	PHP_ME(WinGuiMessageQueue, get, WinGuiMessageQueue_get_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, getExtraInfo, WinGuiMessageQueue_getExtraInfo_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, getInputState, WinGuiMessageQueue_getInputState_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, getPos, WinGuiMessageQueue_getPos_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, getTime, WinGuiMessageQueue_getTime_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, getStatus, WinGuiMessageQueue_getStatus_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, inSend, WinGuiMessageQueue_inSend_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, peek, WinGuiMessageQueue_peek_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, postQuit, WinGuiMessageQueue_postQuit_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, setExtraInfo, WinGuiMessageQueue_setExtraInfo_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, wait, WinGuiMessageQueue_wait_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, start, WinGuiMessageQueue_start_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiMessageQueue, stop, WinGuiMessageQueue_stop_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Message\Queue LifeCycle Functions
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_message_queue)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS_NAME("Message", "Queue"), wingui_message_queue_functions);
	ce_wingui_message_queue = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_message_queue->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

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
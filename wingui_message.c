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

zend_class_entry *ce_wingui_message;
HashTable wingui_message_prop_handlers;
static zend_object_handlers wingui_message_object_handlers;
static zend_function wingui_message_constructor_wrapper;

/* ----------------------------------------------------------------
  Win\Gui\Message Userland API
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiMessage___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VAL, 1)
	ZEND_ARG_INFO(0, msg)
	ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_broadcast_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, recipients)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_dispatch_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_post_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_postThread_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, thread_id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_send_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_sendCallback_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, callback)
	ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_sendTimeout_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_sendNotify_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessage_translate_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

/* {{{ proto bool Win\Gui\Message->__construct(int msg[, ...])
      Create a new message */
PHP_METHOD(WinGuiMessage, __construct)
{
	// TODO: fun
}
/* }}} */

/* {{{ proto bool Win\Gui\Message->broadcast(int flags, int recipients)
     sends the message to all items specified,
	 will return message and additional information to send
	 acts similiar to the send and post methods, only to multiple targets */
PHP_METHOD(WinGuiMessage, broadcast)
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

/* {{{  proto bool Win\Gui\Message->dispatch(void)
        dispatches a method, useful for user handled message loops */
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

/* {{{ proto bool Win\Gui\Message->post()
      Posts the current message */
PHP_METHOD(WinGuiMessage, post)
{
	// TODO: fun
}
/* }}} */

/* {{{ proto bool Win\Gui\Message->postThread(int thread_id)
      Posts the current message to a specific thread*/
PHP_METHOD(WinGuiMessage, postThread)
{
	// TODO: fun
}
/* }}} */

/* {{{ proto bool Win\Gui\Message->send()
      Sends the current message */
PHP_METHOD(WinGuiMessage, send)
{
	// TODO: fun
}
/* }}} */

/* {{{ proto bool Win\Gui\Message->sendCallback(mixed callback, ...)
      Sends the current message with a callback*/
PHP_METHOD(WinGuiMessage, sendCallback)
{
	// TODO: fun
}
/* }}} */

/* {{{ proto bool Win\Gui\Message->sendTimeout(mixed callback, ...)
      Sends the current message with a callback*/
PHP_METHOD(WinGuiMessage, sendTimeout)
{
	// TODO: fun
}
/* }}} */

/* {{{ proto bool Win\Gui\Message->sendNotify()
     Send a message and wait for it to return if its in the same thread */
PHP_METHOD(WinGuiMessage, sendNotify)
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

/* register Messaging methods */
static zend_function_entry wingui_message_functions[] = {
	PHP_ME(WinGuiMessage, __construct, WinGuiMessage___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiMessage, broadcast, WinGuiMessage_broadcast_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessage, dispatch, WinGuiMessage_dispatch_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessage, post, WinGuiMessage_post_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessage, postThread, WinGuiMessage_postThread_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessage, send, WinGuiMessage_send_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessage, sendCallback, WinGuiMessage_sendCallback_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessage, sendTimeout, WinGuiMessage_sendTimeout_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessage, sendNotify, WinGuiMessage_sendNotify_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessage, translate, WinGuiMessage_translate_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Message Custom Object magic
------------------------------------------------------------------*/

/* {{{ wingui_message_get_constructor
       gets the constructor for the class  */
static zend_function *wingui_message_get_constructor(zval *object TSRMLS_DC)
{
	if (Z_OBJCE_P(object) == ce_wingui_message)
		return zend_get_std_object_handlers()->
			get_constructor(object TSRMLS_CC);
	else
		return &wingui_message_constructor_wrapper;
}
/* }}} */

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

/* Gets the correct value out of the msg parameter */
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
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Message", wingui_message_functions);

	ce_wingui_message = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_message->create_object = wingui_message_object_new;

	wingui_message_constructor_wrapper.type = ZEND_INTERNAL_FUNCTION;
	wingui_message_constructor_wrapper.common.function_name = "internal_construction_wrapper";
	wingui_message_constructor_wrapper.common.scope = ce_wingui_message;
	wingui_message_constructor_wrapper.common.fn_flags = ZEND_ACC_PROTECTED;
	wingui_message_constructor_wrapper.common.prototype = NULL;
	wingui_message_constructor_wrapper.common.required_num_args = 0;
	wingui_message_constructor_wrapper.common.arg_info = NULL;
	wingui_message_constructor_wrapper.common.pass_rest_by_reference = 0;
	wingui_message_constructor_wrapper.common.return_reference = 0;
	wingui_message_constructor_wrapper.internal_function.handler = wingui_object_construction_wrapper;
	wingui_message_constructor_wrapper.internal_function.module = EG(current_module);

	memcpy(&wingui_message_object_handlers, &wingui_object_handlers,
		sizeof wingui_message_object_handlers);
	wingui_message_object_handlers.get_constructor = wingui_message_get_constructor;

	zend_hash_init(&wingui_message_prop_handlers, 0, NULL, NULL, 1);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "window", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "message", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "time", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "params", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "x", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "y", wingui_message_read, NULL TSRMLS_CC);

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
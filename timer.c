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

zend_class_entry *ce_wingui_timer;

/* ----------------------------------------------------------------
  Win\Gui\Timer Userland API
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiTimer___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, window)
	ZEND_ARG_INFO(0, id)
	ZEND_ARG_INFO(0, elapse)
	ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\Timer::__construct([object window, int id, int elapse,callback callback])
     Create timer that goes off every elapse milliseconds */
PHP_METHOD(WinGuiTimer, __construct)
{
}
/* }}} */

/* Timer Methods */
static zend_function_entry wingui_resource_functions[] = {
	PHP_ME(WinGuiTimer, __construct, WinGuiTimer___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Timer C API
------------------------------------------------------------------*/

VOID CALLBACK php_wingui_timer_callback(HWND hwnd, UINT msg, UINT_PTR event_id, DWORD time)
{
	if (param) {
		zval *retval_ptr = NULL;
		winsystem_timer_callback * store = (winsystem_timer_callback *) param;
#ifdef ZTS
		TSRMLS_D = store->TSRMLS_C;
#endif
		store->callback_info.retval_ptr_ptr = &retval_ptr;
		if (FAILURE == zend_call_function(&(store->callback_info), &(store->callback_cache) TSRMLS_CC)) {
			php_error(E_RECOVERABLE_ERROR, "Error calling %s", store->callback_info.function_name);
		}
	}
}

/* ----------------------------------------------------------------
  Win\Gui\Timer LifeCycle Functions
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_timer)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Timer", wingui_timer_functions);

	ce_wingui_timer = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_timer->create_object = wingui_timer_object_new;

	wingui_timer_constructor_wrapper.type = ZEND_INTERNAL_FUNCTION;
	wingui_timer_constructor_wrapper.common.function_name = "internal_construction_wrapper";
	wingui_timer_constructor_wrapper.common.scope = ce_wingui_message;
	wingui_timer_constructor_wrapper.common.fn_flags = ZEND_ACC_PROTECTED;
	wingui_timer_constructor_wrapper.common.prototype = NULL;
	wingui_timer_constructor_wrapper.common.required_num_args = 0;
	wingui_timer_constructor_wrapper.common.arg_info = NULL;
	wingui_timer_constructor_wrapper.common.pass_rest_by_reference = 0;
	wingui_timer_constructor_wrapper.common.return_reference = 0;
	wingui_timer_constructor_wrapper.internal_function.handler = wingui_object_construction_wrapper;
	wingui_timer_constructor_wrapper.internal_function.module = EG(current_module);

	memcpy(&wingui_timer_object_handlers, &wingui_object_handlers,
		sizeof wingui_timer_object_handlers);
	wingui_timer_object_handlers.get_constructor = wingui_timer_get_constructor;

	/*zend_hash_init(&wingui_message_prop_handlers, 0, NULL, NULL, 1);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "window", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "message", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "time", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "params", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "x", wingui_message_read, NULL TSRMLS_CC);
	wingui_register_prop_handler(&wingui_message_prop_handlers, ce_wingui_message, "y", wingui_message_read, NULL TSRMLS_CC); */

	return SUCCESS;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
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
#include "php_winsystem_api.h"
#include "implement_control.h"

/* All used by other classes, none static */
zend_class_entry *ce_wingui_control;

HashTable wingui_control_prop_handlers;
HashTable wingui_control_callback_map;

/* ----------------------------------------------------------------
  Win\Gui\Control Userland API
------------------------------------------------------------------*/

/* register Control methods */
static zend_function_entry wingui_control_functions[] = {
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Control LifeCycle Functions
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_control)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Control", wingui_control_functions);
	ce_wingui_control = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_control->ce_flags |= ZEND_ACC_INTERFACE;

	zend_hash_init(&wingui_control_prop_handlers, 0, NULL, NULL, 1);
	zend_hash_init(&wingui_control_callback_map, 0, NULL, NULL, 1);

	return SUCCESS;
}

/* {{{ PHP_MSHUTDOWN_FUNCTION(wingui_control) */
PHP_MSHUTDOWN_FUNCTION(wingui_control)
{
	zend_hash_destroy(&wingui_control_prop_handlers);
	zend_hash_destroy(&wingui_control_callback_map);
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
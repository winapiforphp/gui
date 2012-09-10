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
#include "ext/spl/spl_exceptions.h"

zend_class_entry *ce_wingui_exception;
zend_class_entry *ce_wingui_argexception;
zend_class_entry *ce_wingui_versionexception;

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(wingui_util)
{
	zend_class_entry ce, version_ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Exception", NULL);
	ce_wingui_exception = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), "Exception" TSRMLS_CC);

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "ArgumentException", NULL);
	ce_wingui_argexception = zend_register_internal_class_ex(&ce, spl_ce_InvalidArgumentException, "InvalidArgumentException" TSRMLS_CC);

	INIT_NS_CLASS_ENTRY(version_ce, PHP_WINGUI_NS, "VersionException", NULL);
	ce_wingui_versionexception = zend_register_internal_class_ex(&version_ce, spl_ce_RuntimeException, "RuntimeException" TSRMLS_CC);

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

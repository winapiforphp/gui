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

/* $Id$ */

#include "php_wingui.h"
#include "zend_exceptions.h"

ZEND_DECLARE_MODULE_GLOBALS(wingui);

/* All the classes in this file */
zend_class_entry *ce_wingui_theme;

/* ----------------------------------------------------------------
  Win\Gui\Theme Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO(WinGuiTheme_disable_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, window)
ZEND_END_ARG_INFO()

/* {{{ proto bool Win\Gui\Theme::disable(Win\Gui\Window window)
       Turns off themes for a single window */
PHP_METHOD(WinGuiTheme, disable)
{
	zend_error_handling error_handling;
	LPCWSTR string = L" ";
	HMODULE module;
	wingui_window_object *window_object;

	typedef HRESULT (WINAPI *SetWindowThemeFunc)(HWND, LPCWSTR, LPCWSTR);
	SetWindowThemeFunc SetWindowThemeFuncHandle;

	/* Short Circuit if we're NOT on xp */
	if(!wingui_is_xp(TSRMLS_C)) {
		zend_throw_exception(ce_wingui_versionexception, "Win\\Gui\\Theme::disable() does not work on Windows versions before XP", 0 TSRMLS_CC);
		return;
	}

	window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	module = LoadLibrary("UxTheme.dll");

	if (module) {
		SetWindowThemeFuncHandle = (SetWindowThemeFunc)GetProcAddress(module, "SetWindowTheme");
		if (SetWindowThemeFuncHandle == NULL) {
			wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
			return;
		}
	} else {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}

	RETVAL_BOOL(SetWindowThemeFuncHandle(window_object->window_handle, string, string));
	FreeLibrary(module);
}
/* }}} */

/* register Theme methods */
static zend_function_entry wingui_theme_functions[] = {
	PHP_ME(WinGuiTheme, disable, WinGuiTheme_disable_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Theme LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_theme)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Theme", wingui_theme_functions);
	ce_wingui_theme = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_theme->ce_flags |= ZEND_ACC_ABSTRACT | ZEND_ACC_FINAL;

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
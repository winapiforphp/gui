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
#include "ext/standard/info.h"

ZEND_DECLARE_MODULE_GLOBALS(wingui);

zend_class_entry *ce_wingui;

/* ----------------------------------------------------------------
  Win\Gui User API                                                      
------------------------------------------------------------------*/

/* Arg Info */
ZEND_BEGIN_ARG_INFO(WinGui_isWin7_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGui_isVista_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGui_isXp_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui::isWin7()
    tells if underlying OS has windows 7 support */
PHP_METHOD(WinGui, isWin7)
{
	zend_error_handling error_handling;
	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(wingui_is_win7(TSRMLS_C));
}
 /* }}} */

/* {{{ proto void Win\Gui::isVista()
    tells if underlying OS has vista support */
PHP_METHOD(WinGui, isVista)
{
	zend_error_handling error_handling;
	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(wingui_is_vista(TSRMLS_C));
}
 /* }}} */

/* {{{ proto void Win\Gui::isXp()
    tells if underlying OS has XP support */
PHP_METHOD(WinGui, isXp)
{
	zend_error_handling error_handling;
	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(wingui_is_xp(TSRMLS_C));
}
 /* }}} */

/* List methods */
static const zend_function_entry wingui_methods[] = {
	PHP_ME(WinGui, isWin7, WinGui_isWin7_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGui, isVista, WinGui_isVista_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGui, isXp, WinGui_isXp_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{ NULL, NULL, NULL }
};

/* ----------------------------------------------------------------
  Win\Gui C API                                                      
------------------------------------------------------------------*/
/* {{{ wingui_is_win7 - are we running on windows7 or higher (some things won't work unless we are) */
BOOL wingui_is_win7 (TSRMLS_D) 
{
	if (EG(windows_version_info).dwMajorVersion < 7 && EG(windows_version_info).dwMinorVersion < 1) {
		return FALSE;
	} else {
		return TRUE;
	}
}
/* }}} */


/* {{{ wingui_is_vista - are we running on vista or higher (some things won't work unless we are) */
BOOL wingui_is_vista (TSRMLS_D) 
{
	if (EG(windows_version_info).dwMajorVersion < 6) {
		return FALSE;
	} else {
		return TRUE;
	}
}
/* }}} */

/* {{{ wingui_is_xp - are we running on xp or higher (some things won't work unless we are) */
BOOL wingui_is_xp (TSRMLS_D) 
{
	if (EG(windows_version_info).dwMajorVersion < 5) {
		return FALSE;
	} else {
		return TRUE;
	}
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui LifeCycle Functions                                                    
------------------------------------------------------------------*/

/* Constructor for globals */
static void wingui_globals_ctor(zend_wingui_globals *wingui_globals TSRMLS_DC) {
	wingui_globals->hinstance = NULL;
	wingui_globals->end_main = FALSE;
}

/* Register classes and set up globals and common controls stuff */
PHP_MINIT_FUNCTION(wingui)
{
	zend_class_entry ce;
	INITCOMMONCONTROLSEX icex;

	/* Initiate windows common controls so we can use them all */
	icex.dwSize = sizeof(icex);
	icex.dwICC  = ICC_BAR_CLASSES | ICC_COOL_CLASSES;
	InitCommonControls();
	InitCommonControlsEx(&icex);

	INIT_NS_CLASS_ENTRY(ce, "Win", "Gui", wingui_methods);
	ce_wingui = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

	/* Utility stuff */
	PHP_MINIT(wingui_util)(INIT_FUNC_ARGS_PASSTHRU);

	/* Interfaces */
	PHP_MINIT(wingui_messaging)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_inputing)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_windowing)(INIT_FUNC_ARGS_PASSTHRU);

	/* Tools */
	//PHP_MINIT(wingui_message)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_input)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_window)(INIT_FUNC_ARGS_PASSTHRU);

	/* Resources*/
	PHP_MINIT(wingui_resource_bitmap)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_resource_icon)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_resource_cursor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_imagelist)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_menu)(INIT_FUNC_ARGS_PASSTHRU);

	/* Controls */
	PHP_MINIT(wingui_control)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_control_button)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_control_listview)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_control_static)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_control_statusbar)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}

/* Free thread stuff if needed */
PHP_MSHUTDOWN_FUNCTION(wingui)
{
	PHP_MSHUTDOWN(wingui_control_statusbar)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	PHP_MSHUTDOWN(wingui_control_listview)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	PHP_MSHUTDOWN(wingui_control)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	PHP_MSHUTDOWN(wingui_window)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	PHP_MSHUTDOWN(wingui_inputing)(SHUTDOWN_FUNC_ARGS_PASSTHRU);

#ifdef ZTS
	ts_free_id(wingui_globals_id);
#endif

	return SUCCESS;
}

/* register a default "class" for windows */
PHP_RINIT_FUNCTION(wingui)
{
	HINSTANCE hinstance;
	WNDCLASSEX wcx = { 0 };
	ATOM worked;

	ZEND_INIT_MODULE_GLOBALS(wingui, wingui_globals_ctor, NULL);

	hinstance = GetModuleHandle(NULL);
	WINGUI_G(hinstance) = hinstance;

	/* these are default values that can be overridden */
	wcx.cbSize = sizeof(wcx);							
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = sizeof(wingui_window_object *);
	wcx.hInstance = hinstance;							
	wcx.lpfnWndProc = DefWindowProc;					
	wcx.lpszMenuName =  NULL;							
	wcx.lpszClassName = "php_wingui_window";					
	wcx.style = 0;
	wcx.hIcon = NULL;								
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wcx.hIconSm = NULL;
	
	worked = RegisterClassEx(&wcx);

	if (worked == 0)  {
		return FAILURE;
	} else {
		return SUCCESS;
	}
}

/* Unregister our class */
PHP_RSHUTDOWN_FUNCTION(wingui)
{
	HINSTANCE hinstance;
	BOOL worked;

	/* grab the application-instance handle. */
	hinstance = WINGUI_G(hinstance);

	worked = UnregisterClass("php_wingui_window", hinstance);
	if (worked == 0) {
		return FAILURE;
	} else {
		return SUCCESS;
	}
}

/* Basic information */
PHP_MINFO_FUNCTION(wingui)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Win32 Gui", "enabled");
	php_info_print_table_row(2, "Version", PHP_WINGUI_VERSION);
	php_info_print_table_end();
}

/* Module entry for winui */
zend_module_entry wingui_module_entry = {
	STANDARD_MODULE_HEADER,
	"wingui",
	NULL,
	PHP_MINIT(wingui),
	PHP_MSHUTDOWN(wingui),
	PHP_RINIT(wingui),
	PHP_RSHUTDOWN(wingui),
	PHP_MINFO(wingui),
	PHP_WINGUI_VERSION,
	STANDARD_MODULE_PROPERTIES
};

/* Required for dynamic linking */
#ifdef COMPILE_DL_WINGUI
ZEND_GET_MODULE(wingui)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
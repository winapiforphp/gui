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
#include "ext/standard/info.h"

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

/* Register classes and set up globals and common controls stuff */
PHP_MINIT_FUNCTION(wingui)
{
	INITCOMMONCONTROLSEX icex;

	/* Initiate windows common controls so we can use them all */
	icex.dwSize = sizeof(icex);
	icex.dwICC  = ICC_BAR_CLASSES | ICC_COOL_CLASSES;
	InitCommonControls();
	InitCommonControlsEx(&icex);

	/* Utility stuff */
	PHP_MINIT(wingui_util)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_object)(INIT_FUNC_ARGS_PASSTHRU);

	/* Interfaces */
	PHP_MINIT(wingui_messaging)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_inputing)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_windowing)(INIT_FUNC_ARGS_PASSTHRU);

	/* Tools */
	PHP_MINIT(wingui_message)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_input)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_window)(INIT_FUNC_ARGS_PASSTHRU);

	/* Resources*/
	PHP_MINIT(wingui_resource)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_resource_cursor)(INIT_FUNC_ARGS_PASSTHRU);


	//PHP_MINIT(wingui_resource_bitmap)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_resource_icon)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_imagelist)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_menu)(INIT_FUNC_ARGS_PASSTHRU);

	/* Controls */
	//PHP_MINIT(wingui_control)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_control_button)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_control_listview)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_control_static)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_control_statusbar)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}

/* Free thread stuff if needed */
PHP_MSHUTDOWN_FUNCTION(wingui)
{
	//PHP_MSHUTDOWN(wingui_control_statusbar)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	//PHP_MSHUTDOWN(wingui_control_listview)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	//PHP_MSHUTDOWN(wingui_control)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	PHP_MSHUTDOWN(wingui_window)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	//PHP_MSHUTDOWN(wingui_inputing)(SHUTDOWN_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}

/* register a default "class" for windows */
PHP_RINIT_FUNCTION(wingui)
{
	/* Do this twice, once for a unicode window, once without */
	WNDCLASSEX wcx = { 0 };
	WNDCLASSEXW wcxw = { 0 };
	ATOM worked;

	/* these are default values that can be overridden */
	wcx.cbSize = sizeof(wcx);
	wcxw.cbSize = sizeof(wcxw);
	wcx.cbClsExtra = 0;
	wcxw.cbClsExtra = 0;
	wcx.cbWndExtra = sizeof(wingui_window_object *);
	wcxw.cbWndExtra = sizeof(wingui_window_object *);
	wcx.hInstance = GetModuleHandleW(NULL);
	wcxw.hInstance = GetModuleHandle(NULL);
	wcx.lpfnWndProc = wingui_proc_handler;
	wcxw.lpfnWndProc = wingui_proc_handler;
	wcx.lpszMenuName =  NULL;
	wcxw.lpszMenuName =  NULL;
	wcx.lpszClassName = "php_wingui_window";
	wcxw.lpszClassName = L"php_wingui_window_unicode";
	wcx.style = 0;
	wcxw.style = 0;
	wcx.hIcon = NULL;
	wcxw.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcxw.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wcxw.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wcx.hIconSm = NULL;
	wcxw.hIconSm = NULL;

	worked = RegisterClassExA(&wcx);

	if (worked == 0) {
		return FAILURE;
	} else {
		worked = RegisterClassExW(&wcxw);
		if (worked == 0) {
			return FAILURE;
		}
		return SUCCESS;
	}
}

/* Unregister our class */
PHP_RSHUTDOWN_FUNCTION(wingui)
{
	BOOL worked;

	worked = UnregisterClassA("php_wingui_window", GetModuleHandle(NULL));
	if (worked == 0) {
		return FAILURE;
	} else {
		worked = UnregisterClassW(L"php_wingui_window_unicode", GetModuleHandleW(NULL));
		if (worked == 0) {
			return FAILURE;
		}
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

/* Requires the win\system for unicode/exceptions
   and win\gdi for drawing stuff */
static const zend_module_dep wingui_deps[] = {
	ZEND_MOD_REQUIRED("winsystem")
	ZEND_MOD_REQUIRED("wingdi")
	{NULL, NULL, NULL}
};

/* Module entry for winui */
zend_module_entry wingui_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	wingui_deps,
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
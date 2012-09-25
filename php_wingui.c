/* 
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Elizabeth Smith <auroraeosrose@gmail.com>                    |
  +----------------------------------------------------------------------+
*/

#include "php_wingui.h"

/* ----------------------------------------------------------------
  Win\Gui LifeCycle Functions                                     
------------------------------------------------------------------*/

/* Register classes and set up globals and common controls stuff */
PHP_MINIT_FUNCTION(wingui)
{
	/* Enums */
	PHP_MINIT(wingui_windowmessage)(INIT_FUNC_ARGS_PASSTHRU);

	/* Interfaces */
	PHP_MINIT(wingui_messaging)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_inputing)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_windowing)(INIT_FUNC_ARGS_PASSTHRU);

	/* Tools */
	PHP_MINIT(wingui_message_queue)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_message)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_input)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_window)(INIT_FUNC_ARGS_PASSTHRU);

	/* Resources*/
	//PHP_MINIT(wingui_resource)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_resource_cursor)(INIT_FUNC_ARGS_PASSTHRU);


	//PHP_MINIT(wingui_resource_bitmap)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_resource_icon)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_imagelist)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_menu)(INIT_FUNC_ARGS_PASSTHRU);

	/* Controls */
	PHP_MINIT(wingui_control)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_control_button)(INIT_FUNC_ARGS_PASSTHRU);
	//PHP_MINIT(wingui_control_listview)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(wingui_control_static)(INIT_FUNC_ARGS_PASSTHRU);
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
	NULL,
	NULL,
	NULL,
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
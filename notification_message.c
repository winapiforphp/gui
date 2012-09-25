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
  | Author: Elizabeth M Smith <auroraeosrose@gmail.com>                  |
  +----------------------------------------------------------------------+
*/

#include "php_wingui.h"
#include <php_winsystem_public.h>

zend_class_entry *ce_wingui_notificationmessage;

/* ----------------------------------------------------------------
    Win\Gui\Notification\Message Enum for NM_ messages
------------------------------------------------------------------*/

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(wingui_notificationmessage)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS("Notification", "Message", NULL);
	ce_wingui_notificationmessage = zend_register_internal_class_ex(&ce, ce_winsystem_enum, NULL TSRMLS_CC);

	REGISTER_ENUM_CONST("ERASEBKGND", WM_ERASEBKGND, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETFONT", WM_GETFONT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETTEXT", WM_GETTEXT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETTEXTLENGTH", WM_GETTEXTLENGTH, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SETFONT", WM_SETFONT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SETICON", WM_SETICON, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SETTEXT", WM_SETTEXT, ce_wingui_windowmessage);

	REGISTER_ENUM_CONST("ACTIVATEAPP", WM_ACTIVATEAPP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("CANCELMODE", WM_CANCELMODE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("CHILDACTIVATE", WM_CHILDACTIVATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("CLOSE", WM_CLOSE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("COMPACTING", WM_COMPACTING, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("CREATE", WM_CREATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DESTROY", WM_DESTROY, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("ENABLE", WM_ENABLE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("ENTERSIZEMOVE", WM_ENTERSIZEMOVE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("EXITSIZEMOVE", WM_EXITSIZEMOVE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETICON", WM_GETICON, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETMINMAXINFO", WM_GETMINMAXINFO, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("INPUTLANGCHANGE", WM_INPUTLANGCHANGE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("INPUTLANGCHANGEREQUEST", WM_INPUTLANGCHANGEREQUEST, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MOVE", WM_MOVE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MOVING", WM_MOVING, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCACTIVATE", WM_NCACTIVATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCCALCSIZE", WM_NCCALCSIZE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCCREATE", WM_NCCREATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCDESTROY", WM_NCDESTROY, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NULL", WM_NULL, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("QUERYDRAGICON", WM_QUERYDRAGICON, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("QUERYOPEN", WM_QUERYOPEN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("QUIT", WM_QUIT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SHOWWINDOW", WM_SHOWWINDOW, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SIZE", WM_SIZE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SIZING", WM_SIZING, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("STYLECHANGED", WM_STYLECHANGED, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("STYLECHANGING", WM_STYLECHANGING, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("THEMECHANGED", WM_THEMECHANGED, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("USERCHANGED", WM_USERCHANGED, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("WINDOWPOSCHANGED", WM_WINDOWPOSCHANGED, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("WINDOWPOSCHANGING", WM_WINDOWPOSCHANGING, ce_wingui_windowmessage);

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
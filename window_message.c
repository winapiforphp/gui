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
#include <Olectl.h>

zend_class_entry *ce_wingui_windowmessage;

/* ----------------------------------------------------------------
    Win\Gui\Window\Message Enum for WM_ messages
------------------------------------------------------------------*/

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(wingui_windowmessage)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS_NAME("Window", "Message"), NULL);
	ce_wingui_windowmessage = zend_register_internal_class_ex(&ce, ce_winsystem_enum, NULL TSRMLS_CC);

	/* Basics */
	REGISTER_ENUM_CONST("BASE", OCM__BASE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("APP", WM_APP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("USER", WM_USER, ce_wingui_windowmessage);

	/* Window Messages */
	REGISTER_ENUM_CONST("ERASEBKGND", WM_ERASEBKGND, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETFONT", WM_GETFONT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETTEXT", WM_GETTEXT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETTEXTLENGTH", WM_GETTEXTLENGTH, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SETFONT", WM_SETFONT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SETICON", WM_SETICON, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SETTEXT", WM_SETTEXT, ce_wingui_windowmessage);

	/* Window Notifications */
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

	/* Timer Notifications */
	REGISTER_ENUM_CONST("TIMER", WM_TIMER, ce_wingui_windowmessage);

	/* Hook Notifications */
	REGISTER_ENUM_CONST("CANCELJOURNAL", WM_CANCELJOURNAL, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("QUEUESYNC", WM_QUEUESYNC, ce_wingui_windowmessage); 	

	/* MDI messages */
	REGISTER_ENUM_CONST("MDIACTIVATE", WM_MDIACTIVATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDICASCADE", WM_MDICASCADE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDICREATE", WM_MDICREATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDIDESTROY", WM_MDIDESTROY, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDIGETACTIVE", WM_MDIGETACTIVE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDIICONARRANGE", WM_MDIICONARRANGE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDIMAXIMIZE", WM_MDIMAXIMIZE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDINEXT", WM_MDINEXT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDIREFRESHMENU", WM_MDIREFRESHMENU, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDINEXT", WM_MDINEXT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDIRESTORE", WM_MDIRESTORE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDISETMENU", WM_MDISETMENU, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MDITILE", WM_MDITILE, ce_wingui_windowmessage);

	/* Keyboard Messages */
	REGISTER_ENUM_CONST("GETHOTKEY", WM_GETHOTKEY, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SETHOTKEY", WM_SETHOTKEY, ce_wingui_windowmessage);
	 	
	/* Keyboard Notifications */
	REGISTER_ENUM_CONST("ACTIVATE", WM_ACTIVATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("APPCOMMAND", WM_APPCOMMAND, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("CHAR", WM_CHAR, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DEADCHAR", WM_DEADCHAR, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("HOTKEY", WM_HOTKEY, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("KEYDOWN", WM_KEYDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("KEYUP", WM_KEYUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("KILLFOCUS", WM_KILLFOCUS, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SETFOCUS", WM_SETFOCUS, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SYSDEADCHAR", WM_SYSDEADCHAR, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SYSKEYDOWN", WM_SYSKEYDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SYSKEYUP", WM_SYSKEYUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("UNICHAR", WM_UNICHAR, ce_wingui_windowmessage);

	/* Mouse Notifications */
	REGISTER_ENUM_CONST("CAPTURECHANGED", WM_CAPTURECHANGED, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("LBUTTONDBLCLK", WM_LBUTTONDBLCLK, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("LBUTTONDOWN", WM_LBUTTONDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("LBUTTONUP", WM_LBUTTONUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MBUTTONDBLCLK", WM_MBUTTONDBLCLK, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MBUTTONDOWN", WM_MBUTTONDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MBUTTONUP", WM_MBUTTONUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MOUSEACTIVATE", WM_MOUSEACTIVATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MOUSEHOVER", WM_MOUSEHOVER, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MOUSEHWHEEL", WM_MOUSEHWHEEL, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MOUSELEAVE", WM_MOUSELEAVE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MOUSEMOVE", WM_MOUSEMOVE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MOUSEWHEEL", WM_MOUSEWHEEL, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCHITTEST", WM_NCHITTEST, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCLBUTTONDBLCLK", WM_NCLBUTTONDBLCLK, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCLBUTTONDOWN", WM_NCLBUTTONDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCLBUTTONUP", WM_NCLBUTTONUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCMBUTTONDBLCLK", WM_NCMBUTTONDBLCLK, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCMBUTTONDOWN", WM_NCMBUTTONDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCMBUTTONUP", WM_NCMBUTTONUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCMOUSEHOVER", WM_NCMOUSEHOVER, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCMOUSELEAVE", WM_NCMOUSELEAVE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCMOUSEMOVE", WM_NCMOUSEMOVE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCRBUTTONDBLCLK", WM_NCRBUTTONDBLCLK, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCRBUTTONDOWN", WM_NCRBUTTONDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCRBUTTONUP", WM_NCRBUTTONUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCXBUTTONDBLCLK", WM_NCXBUTTONDBLCLK, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCXBUTTONDOWN", WM_NCXBUTTONDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NCXBUTTONUP", WM_NCXBUTTONUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("RBUTTONDBLCLK", WM_RBUTTONDBLCLK, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("RBUTTONDOWN", WM_RBUTTONDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("RBUTTONUP", WM_RBUTTONUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("XBUTTONDBLCLK", WM_XBUTTONDBLCLK, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("XBUTTONDOWN", WM_XBUTTONDOWN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("XBUTTONUP", WM_XBUTTONUP, ce_wingui_windowmessage);

	/* Raw Input Notifications */
	REGISTER_ENUM_CONST("INPUT", WM_INPUT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("INPUT_DEVICE_CHANGE", WM_INPUT_DEVICE_CHANGE, ce_wingui_windowmessage);

	/* Dialog Notifications */
	REGISTER_ENUM_CONST("CTLCOLORDLG", WM_CTLCOLORDLG, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("ENTERIDLE", WM_ENTERIDLE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETDLGCODE", WM_GETDLGCODE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("INITDIALOG", WM_INITDIALOG, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NEXTDLGCTL", WM_NEXTDLGCTL, ce_wingui_windowmessage);
 	
	/* Menu messages */
	REGISTER_ENUM_CONST("COMMAND", WM_COMMAND, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("CONTEXTMENU", WM_CONTEXTMENU, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("ENTERMENULOOP", WM_ENTERMENULOOP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("EXITMENULOOP", WM_EXITMENULOOP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("GETTITLEBARINFOEX", WM_GETTITLEBARINFOEX, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MENUCOMMAND", WM_MENUCOMMAND, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MENUDRAG", WM_MENUDRAG, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MENUGETOBJECT", WM_MENUGETOBJECT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MENURBUTTONUP", WM_MENURBUTTONUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NEXTMENU", WM_NEXTMENU, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("UNINITMENUPOPUP", WM_UNINITMENUPOPUP, ce_wingui_windowmessage);

	/* Keyboard Accelerator Messages */
	REGISTER_ENUM_CONST("CHANGEUISTATE", WM_CHANGEUISTATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("INITMENU", WM_INITMENU, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("QUERYUISTATE", WM_QUERYUISTATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("UPDATEUISTATE", WM_UPDATEUISTATE, ce_wingui_windowmessage);

	/* Keyboard Accelerator Notifications */
	REGISTER_ENUM_CONST("INITMENUPOPUP", WM_INITMENUPOPUP, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MENUCHAR", WM_MENUCHAR, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("MENUSELECT", WM_MENUSELECT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SYSCHAR", WM_SYSCHAR, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SYSCOMMAND", WM_SYSCOMMAND, ce_wingui_windowmessage);

	/* Cursor Notifications */
	REGISTER_ENUM_CONST("SETCURSOR", WM_SETCURSOR, ce_wingui_windowmessage);

	/* Clipboard Messages */
	REGISTER_ENUM_CONST("CLEAR", WM_CLEAR, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("COPY", WM_COPY, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("CUT", WM_CUT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("PASTE", WM_PASTE, ce_wingui_windowmessage);

	/* Clipboard Notifications */
	REGISTER_ENUM_CONST("ASKCBFORMATNAME", WM_ASKCBFORMATNAME, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("CHANGECBCHAIN", WM_CHANGECBCHAIN, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("CLIPBOARDUPDATE", WM_CLIPBOARDUPDATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DESTROYCLIPBOARD", WM_DESTROYCLIPBOARD, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DRAWCLIPBOARD", WM_DRAWCLIPBOARD, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("HSCROLLCLIPBOARD", WM_HSCROLLCLIPBOARD, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("PAINTCLIPBOARD", WM_PAINTCLIPBOARD, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("RENDERALLFORMATS", WM_RENDERALLFORMATS, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("RENDERFORMAT", WM_RENDERFORMAT, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("SIZECLIPBOARD", WM_SIZECLIPBOARD, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("VSCROLLCLIPBOARD", WM_VSCROLLCLIPBOARD, ce_wingui_windowmessage);

	/* Data Copy Messages */
	REGISTER_ENUM_CONST("COPYDATA", WM_COPYDATA, ce_wingui_windowmessage);

	/* DDE Messages */
	REGISTER_ENUM_CONST("DDE_INITIATE", WM_DDE_INITIATE, ce_wingui_windowmessage);

	/* DDE Notifications */
	REGISTER_ENUM_CONST("DDE_ACK", WM_DDE_ACK, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DDE_ADVISE", WM_DDE_ADVISE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DDE_DATA", WM_DDE_DATA, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DDE_EXECUTE", WM_DDE_EXECUTE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DDE_POKE", WM_DDE_POKE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DDE_REQUEST", WM_DDE_REQUEST, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DDE_TERMINATE", WM_DDE_TERMINATE, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("DDE_UNADVISE", WM_DDE_UNADVISE, ce_wingui_windowmessage);

	/* Common Control Notifications */
	REGISTER_ENUM_CONST("NOTIFY", WM_NOTIFY, ce_wingui_windowmessage);
	REGISTER_ENUM_CONST("NOTIFYFORMAT", WM_NOTIFYFORMAT, ce_wingui_windowmessage);
	 	
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
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
  | Author: Elizabeth M Smith <auroraeosrose@php.net>                    |
  +----------------------------------------------------------------------+
*/

#include "php_wingui.h"
#include "php_winsystem_api.h"
#include "zend_exceptions.h"

/* ----------------------------------------------------------------
  wingui_window_object window procedure functionality
------------------------------------------------------------------*/






/* ----------------------------------------------------------------
  wingui_window_object basic window crackers, packers
  LRESULTs in and LRESULTS out
------------------------------------------------------------------*/

/* put zvals into lparams, wparams before calling defaultproc */
void wingui_window_messages_packer(int msg, WPARAM *wParam, LPARAM *lParam, zval ***extra TSRMLS_DC)
{
	zval *value = NULL;

	return;

	switch(msg) {
		/* All the no params msgs go first */
		case WM_CANCELMODE:
		case WM_CHILDACTIVATE:
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_ENTERSIZEMOVE:
		case WM_EXITSIZEMOVE:
			break;
		case WM_ACTIVATEAPP:
		case WM_ENABLE:
		case WM_GETICON:
			if(winsystem_juggle_type(*extra[0], IS_LONG TSRMLS_CC) == SUCCESS) {
				*wParam = Z_LVAL_P(*extra[0]);
			}
			break;
		case WM_GETMINMAXINFO:
			/* maxsize */
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[0]), 0, &value) == SUCCESS) {
				if(winsystem_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
					((MINMAXINFO*)*lParam)->ptMaxSize.x = Z_LVAL_P(value);
				}
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[0]), 1, &value) == SUCCESS) {
				if(winsystem_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
					((MINMAXINFO*)*lParam)->ptMaxSize.y = Z_LVAL_P(value);
				}
			}
			
			/* maxposition */
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[1]), 0, &value) == SUCCESS) {
				if(winsystem_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
					((MINMAXINFO*)*lParam)->ptMaxPosition.x = Z_LVAL_P(value);
				}
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[1]), 1, &value) == SUCCESS) {
				winsystem_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMaxPosition.y = Z_LVAL_P(value);
			}

			/* mintracksize */
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[2]), 0, &value) == SUCCESS) {
				winsystem_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMinTrackSize.x = Z_LVAL_P(value);
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[2]), 1, &value) == SUCCESS) {
				winsystem_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMinTrackSize.y = Z_LVAL_P(value);
			}	

			/* maxtracksize */
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[3]), 0, &value) == SUCCESS) {
				winsystem_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMaxTrackSize.x = Z_LVAL_P(value);
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[3]), 1, &value) == SUCCESS) {
				winsystem_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMaxTrackSize.y = Z_LVAL_P(value);
			}
			break;
		case WM_INPUTLANGCHANGE:
		case WM_INPUTLANGCHANGEREQUEST:
			winsystem_juggle_type(*extra[0], IS_LONG TSRMLS_CC);
			*wParam = Z_LVAL_P(*extra[0]);
			winsystem_juggle_type(*extra[1], IS_LONG TSRMLS_CC);
			*lParam = Z_LVAL_P(*extra[1]);
			break;
		case WM_MOVE:
			winsystem_juggle_type(*extra[0], IS_LONG TSRMLS_CC);
			winsystem_juggle_type(*extra[1], IS_LONG TSRMLS_CC);
			*lParam = MAKELPARAM(*extra[0], *extra[1]);
			break;
		case WM_SIZE:
			winsystem_juggle_type(*extra[0], IS_LONG TSRMLS_CC);
			*wParam = Z_LVAL_P(*extra[0]);
			winsystem_juggle_type(*extra[1], IS_LONG TSRMLS_CC);
			winsystem_juggle_type(*extra[2], IS_LONG TSRMLS_CC);
			*lParam = MAKELPARAM(*extra[1], *extra[2]);
			break;
		case WM_COMMAND:
			/* lparam is 0 we have Menu */
			if (*lParam == 0 && HIWORD(*wParam) == 0) {
				if(winsystem_juggle_type(*extra[0], IS_LONG TSRMLS_CC) == SUCCESS) {
					*wParam = MAKEWPARAM(*extra[0], 0);
				}
			}
			break;
	/* basic window message crackers 
	/* Send these on to their destination classes 
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
			/* LPNMHDR STRUCT 
			HWND hwndFrom;
			UINT_PTR idFrom;
			UINT code; 
			/* Statusbar rethrowing 
			case NM_CLICK:
			case NM_DBLCLK:
			case NM_RCLICK:
			case NM_RDBLCLK:
			{
				SendMessage((HWND) ((LPNMHDR)*lParam)->hwndFrom, WINGUI_INTERNAL_MESSAGE, ((LPNMHDR)*lParam)->code, *lParam);
			}
		}
	case WM_COMMAND:
			/* Button messages are sent by default to the parent, this redirects to the button itself 
			switch(HIWORD(*wParam)) {
				case BCN_HOTITEMCHANGE:
				case BN_CLICKED:
				case BN_DBLCLK:
				case BN_DISABLE:
				case BN_HILITE: // BN_PUSHED
				case BN_KILLFOCUS:
				case BN_PAINT:
				case BN_SETFOCUS:
				case BN_UNHILITE: // BN_UNPUSHED
				{
					SendMessage((HWND) *lParam, HIWORD(*wParam), 0, 0);
					break;
				}
			}
			
		}
		break;*/
	}
}
/* }}} */

/* put lparams and wparams into zvals for windows notifications and messages */
zval ***wingui_window_messages_cracker(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC)
{
	zval ***extra = NULL;

	switch(msg) {
		/* All the no params msgs go first */
		case WM_CANCELMODE:
		case WM_CHILDACTIVATE:
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_ENTERSIZEMOVE:
		case WM_EXITSIZEMOVE:
			break;
		case WM_ACTIVATEAPP:
		case WM_ENABLE:
			*extra_count = 1;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_BOOL(*extra[0], *wParam); /* activated or enabled */
			/* Thread ID is ignored for WM_ACTIVATEAPP */
			break;
		case WM_GETICON:
			*extra_count = 1;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], *wParam); /* ICON_BIG, ICON_SMALL, ICON_SMALL2 */
			break;
		case WM_GETMINMAXINFO:
			*extra_count = 4;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			array_init(*extra[0]); /* maxsize */
			add_next_index_long(*extra[0], ((MINMAXINFO*)*lParam)->ptMaxSize.x);
			add_next_index_long(*extra[0], ((MINMAXINFO*)*lParam)->ptMaxSize.y);
			array_init(*extra[1]); /* maxposition */
			add_next_index_long(*extra[1], ((MINMAXINFO*)*lParam)->ptMaxPosition.x);
			add_next_index_long(*extra[1], ((MINMAXINFO*)*lParam)->ptMaxPosition.y);
			array_init(*extra[2]); /* mintracksize */
			add_next_index_long(*extra[2], ((MINMAXINFO*)*lParam)->ptMinTrackSize.x);
			add_next_index_long(*extra[2], ((MINMAXINFO*)*lParam)->ptMinTrackSize.y);
			array_init(*extra[3]); /* maxtracksize */
			add_next_index_long(*extra[3], ((MINMAXINFO*)*lParam)->ptMaxTrackSize.x);
			add_next_index_long(*extra[3], ((MINMAXINFO*)*lParam)->ptMaxTrackSize.y);
			break;
		case WM_INPUTLANGCHANGE:
		case WM_INPUTLANGCHANGEREQUEST:
			*extra_count = 2;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], *wParam);	/* character set, changerequest flags */
			ZVAL_LONG(*extra[1], *lParam);	/* locale identifier */
			break;
		case WM_MOVE:
			*extra_count = 2;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], LOWORD(*lParam));	/* x */
			ZVAL_LONG(*extra[1], HIWORD(*lParam));	/* y */
			break;
		case WM_SIZE:
			*extra_count = 3;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], *wParam);			/* state */
			ZVAL_LONG(*extra[1], LOWORD(*lParam));	/* width */
			ZVAL_LONG(*extra[2], HIWORD(*lParam));	/* height */
			break;
		case WM_COMMAND:
			/* lparam is 0 we have Menu */
			if (*lParam == 0 && HIWORD(*wParam) == 0) {
				*extra_count = 1;
				extra = wingui_callback_extra_zvals_ctor(*extra_count);
				ZVAL_LONG(*extra[0], LOWORD(*wParam));	/* menu identifier */
			}
			break;
	/* basic window message crackers 
	case WM_GETICON:
		// TODO: we don't have an icon class yet!
		break;
	/* Send these on to their destination classes 
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
			/* LPNMHDR STRUCT 
			HWND hwndFrom;
			UINT_PTR idFrom;
			UINT code; 
			/* Statusbar rethrowing 
			case NM_CLICK:
			case NM_DBLCLK:
			case NM_RCLICK:
			case NM_RDBLCLK:
			{
				SendMessage((HWND) ((LPNMHDR)*lParam)->hwndFrom, WINGUI_INTERNAL_MESSAGE, ((LPNMHDR)*lParam)->code, *lParam);
			}
		}
	case WM_COMMAND:
		if (*lParam == 0) {
			//extra = wingui_wmcommand_message_cracker(&wParam, &lParam, &extra_count);
		} else {
			/* Button messages are sent by default to the parent, this redirects to the button itself 
			switch(HIWORD(*wParam)) {
				case BCN_HOTITEMCHANGE:
				case BN_CLICKED:
				case BN_DBLCLK:
				case BN_DISABLE:
				case BN_HILITE: // BN_PUSHED
				case BN_KILLFOCUS:
				case BN_PAINT:
				case BN_SETFOCUS:
				case BN_UNHILITE: // BN_UNPUSHED
				{
					SendMessage((HWND) *lParam, HIWORD(*wParam), 0, 0);
					break;
				}
			}
			
		}
		break;*/
	}
	return extra;
}
/* }}} */

/* put zval return values from from messages into lresult vals */
LRESULT wingui_window_messages_results(int msg, zval *return_value TSRMLS_DC)
{
	return (LRESULT) NULL;
	switch(msg) {
		/* POST only - no return */
		case WM_INPUTLANGCHANGEREQUEST:
			return (LRESULT) NULL;
			break;
		/* These messages should return 0 if they process the message */
		case WM_ACTIVATEAPP:
		case WM_CANCELMODE:
		case WM_CHILDACTIVATE:
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_ENABLE:
		case WM_ENTERSIZEMOVE:
		case WM_EXITSIZEMOVE:
		case WM_GETMINMAXINFO:
		case WM_MOVE:
		case WM_COMMAND:
		/* These messages should return non-zero if they process the message */
		case WM_INPUTLANGCHANGE:
			if (winsystem_juggle_type(return_value, IS_LONG TSRMLS_CC) == SUCCESS) {
				return (LRESULT) Z_LVAL_P(return_value);
			} else {
				return (LRESULT) NULL;
			}
			break;
		case WM_GETICON:
			// TODO: need the icon ce, grab out the icon handle and return it
			return (LRESULT) NULL;
			break;
		default:
			return (LRESULT) NULL;
	}
}
/* }}} */

int wingui_window_object_get_styles(HashTable *options, long *style, long *extrastyle TSRMLS_DC) {
		zval **value;

	/*
	if (zend_hash_find(Z_ARRVAL_P(options), "align", sizeof("align"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_RIGHT;
		} else {
			*extrastyle |= WS_EX_LEFT;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "textdir", sizeof("textdir"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_RTLREADING;
		} else {
			*extrastyle |= WS_EX_LTRREADING;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "scrollbar", sizeof("scrollbar"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_LEFTSCROLLBAR;
		} else {
			*extrastyle |= WS_EX_RIGHTSCROLLBAR;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "maximizebox", sizeof("maximizebox"), (void **) &value) == SUCCESS){
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_MAXIMIZEBOX;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "minimizebox", sizeof("minimizebox"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_MINIMIZEBOX;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "helpbox", sizeof("helpbox"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_CONTEXTHELP;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "composited", sizeof("composited"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_COMPOSITED;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "layered", sizeof("layered"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_LAYERED;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "transparent", sizeof("transparent"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_TRANSPARENT;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "layout", sizeof("layout"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_LAYOUTRTL;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "inheritlayout", sizeof("inheritlayout"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (!Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_NOINHERITLAYOUT;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "notifyparent", sizeof("notifyparent"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (!Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_NOPARENTNOTIFY;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "controlparent", sizeof("controlparent"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_CONTROLPARENT;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "forcetaskbar", sizeof("forcetaskbar"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_APPWINDOW;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "tabstop", sizeof("tabstop"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_TABSTOP;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "clipchildren", sizeof("clipchildren"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_CLIPCHILDREN;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "clipsiblings", sizeof("clipsiblings"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_CLIPSIBLINGS;
		} else if (!((*style & WS_CHILD) == WS_CHILD)) {
			zend_throw_exception(wingui_exception_get_ce(TSRMLS_C), "A POPUP or TILED window must have clipsiblings enabled", 0 TSRMLS_CC);
			return FAILURE;
		}
	}


	if (zend_hash_find(Z_ARRVAL_P(options), "hscroll", sizeof("hscroll"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_HSCROLL;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "vscroll", sizeof("vscroll"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_VSCROLL;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "caption", sizeof("caption"), (void **) &value) == SUCCESS) {
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_CAPTION;
		} else if ((!((*style & WS_POPUP) == WS_POPUP)) && !((*style & WS_CHILD) == WS_CHILD)) {
			zend_throw_exception(wingui_exception_get_ce(TSRMLS_C), "A TILED window must have a caption", 0 TSRMLS_CC);
			return FAILURE;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "sysmenu", sizeof("sysmenu"), (void **) &value) == SUCCESS)
	{
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value))
		{
			if (!(*style & WS_CAPTION))
			{
				zend_throw_exception(wingui_exception_get_ce(TSRMLS_C), "A window with a sysmenu must also have a caption", 0 TSRMLS_CC);
				return FAILURE;
			}
			*style |= WS_SYSMENU;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "toolwindow", sizeof("toolwindow"), (void **) &value) == SUCCESS)
	{
		*value = winapi_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value))
		{
			*extrastyle |= WS_EX_TOOLWINDOW;
		}
	} */
	return SUCCESS;
}

/* Grab current properties in class and use it to populate constructor data 
int wingui_window_object_property_values(zval *object, int *x, int *y, int *height, int *width, char **text, int *style, int *extrastyle TSRMLS_DC)
{
	zval *member, *temp;

	MAKE_STD_ZVAL(member);
	
	ZVAL_STRING(member, "x", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_LONG TSRMLS_CC);
		*x = Z_LVAL_P(temp);
	}
	ZVAL_STRING(member, "y", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_LONG TSRMLS_CC);
		*y = Z_LVAL_P(temp);
	}
	ZVAL_STRING(member, "width", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_LONG TSRMLS_CC);
		*width = Z_LVAL_P(temp);
	}
	ZVAL_STRING(member, "height", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_LONG TSRMLS_CC);
		*height = Z_LVAL_P(temp);
	}
	ZVAL_STRING(member, "text", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_STRING TSRMLS_CC);
		*text = Z_STRVAL_P(temp);
	}
	/* If parent is set as a property we remove it 
	ZVAL_STRING(member, "parent", 0);
	std_object_handlers.unset_property(object, member TSRMLS_CC);
	/*
	ZVAL_STRING(member, "minimizebox", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*style |= WS_MINIMIZEBOX;
		}
	}
	ZVAL_STRING(member, "maximizebox", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*style |= WS_MAXIMIZEBOX;
		}
	}
	ZVAL_STRING(member, "helpbox", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_CONTEXTHELP;
		}
	}
	ZVAL_STRING(member, "align", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_RIGHT;
		}
		else
		{
			*extrastyle |= WS_EX_LEFT;
		}
	}
	ZVAL_STRING(member, "textdir", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_RTLREADING;
		}
		else
		{
			*extrastyle |= WS_EX_LTRREADING;
		}
	}
	ZVAL_STRING(member, "scrollbar", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_LEFTSCROLLBAR;
		}
		else
		{
			*extrastyle |= WS_EX_RIGHTSCROLLBAR;
		}
	}
	ZVAL_STRING(member, "composited", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_COMPOSITED;
		}
	}
	ZVAL_STRING(member, "layered", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_LAYERED;
		}
	}
	ZVAL_STRING(member, "transparent", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_TRANSPARENT;
		}
	}
	ZVAL_STRING(member, "inheritlayout", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (!Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_NOINHERITLAYOUT;
		}
	}
	ZVAL_STRING(member, "layout", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_LAYOUTRTL;
		}
	}
	ZVAL_STRING(member, "notifyparent", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (!Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_NOPARENTNOTIFY;
		}
	}
	ZVAL_STRING(member, "controlparent", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_CONTROLPARENT;
		}
	}
	ZVAL_STRING(member, "forcetaskbar", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*extrastyle |= WS_EX_APPWINDOW;
		}
	}
	ZVAL_STRING(member, "tabstop", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*style |= WS_TABSTOP;
		}
	}
	ZVAL_STRING(member, "clipsiblings", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*style |= WS_CLIPSIBLINGS;
		}
		else if (!((*style & WS_CHILD) == WS_CHILD))
		{
			zend_throw_exception(wingui_exception_get_ce(TSRMLS_C), "A POPUP or TILED window must have clipsiblings enabled", 0 TSRMLS_CC);
			return FAILURE;
		}
	}
	ZVAL_STRING(member, "clipchildren", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*style |= WS_CLIPCHILDREN;
		}
	}
	ZVAL_STRING(member, "hscroll", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*style |= WS_HSCROLL;
		}
	}
	ZVAL_STRING(member, "vscroll", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*style |= WS_VSCROLL;
		}
	}
	ZVAL_STRING(member, "resize", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*style |= WS_THICKFRAME;
		}
	}
	ZVAL_STRING(member, "caption", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC))
	{
		temp = winapi_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp))
		{
			*style |= WS_CAPTION;
		}
	} 
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
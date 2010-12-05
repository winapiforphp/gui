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
#include "php_winsystem_api.h"
#include "php_wingdi_api.h"

#include "zend_exceptions.h"

#include "implement_messaging.h"
#include "implement_windowing.h"

/* All the classes in this file */
zend_class_entry *ce_wingui_window;

/* ----------------------------------------------------------------
  Win\Gui\Window Userland API                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_allowSetForeground_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, process_id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindow_bringToTop_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_getPopupPosition_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 4)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, y)
	ZEND_ARG_INFO(0, width)
	ZEND_ARG_INFO(0, height)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, exclude)
ZEND_END_ARG_INFO()






ZEND_BEGIN_ARG_INFO(WinGuiWindow_soundSentry_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_tile_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, rect)
	ZEND_ARG_INFO(0, kids)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindow_getFromPhysicalPoint_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindow_getFromPoint_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindow_registerClass_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindow_unregisterClass_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

/* {{{ proto array Win\Gui\Window::allowSetForeground([long process_id])
       allows a specific process with a process_id to set the foreground window.
       Pass -1 to allow anything to set the foreground window, use null for the current process */
PHP_METHOD(WinGuiWindow, allowSetForeground)
{
	zend_error_handling error_handling;
	long process_id = 0;
	BOOL worked = FALSE;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l!", &process_id) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* If call works properly, return bool, otherwise throw exception */
	worked = AllowSetForegroundWindow(process_id);

	RETVAL_BOOL(worked);
	if (!worked) {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
	}
}
/* }}} */

/* {{{ proto void Win\Gui\Window->bringToTop()
       brings a window to the top of the Z-index and activates it */
PHP_METHOD(WinGuiWindow, bringToTop)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(BringWindowToTop(window_object->window_handle));
}
/* }}} */

/* {{{ proto array Win\Gui\Window::getPopupPosition(int x, int y, int width, int height, int flags[, array exclude])
       Calculates an appropriate pop-up window position using the specified anchor point,
       pop-up window size, flags, and the optional exclude rectangle. When the specified
       pop-up window size is smaller than the desktop window size, this will ensure that
       the pop-up window is fully visible on the desktop window, regardless of the specified anchor point */
PHP_METHOD(WinGuiWindow, getPopupPosition)
{
	zend_error_handling error_handling;
	HMODULE module;
	long x = 0, y = 0, width = 0, height = 0, flags = 0;
	zval *exclude_array = NULL, **value;
	POINT point;
	SIZE size;
	RECT *exclude_rect, new_position;

	typedef BOOL (WINAPI *CalculatePopupWindowPositionFunc)(const POINT *anchorPoint, const SIZE *windowSize,
		UINT flags, RECT *excludeRect, RECT *popupWindowPosition);

	CalculatePopupWindowPositionFunc CalculatePopupWindowPositionFuncHandle;

	/* Short Circuit if we're NOT on windows 7 */
	if(!wingui_is_win7(TSRMLS_C)) {
		zend_throw_exception(ce_wingui_versionexception, "Win\\Gui\\Window::popupPosition() does not work on Windows versions before Windows 7", 0 TSRMLS_CC);
		return;
	}

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll|la", &x, &y, &width, &height, &flags, &exclude_array) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	module = GetModuleHandle("user32.dll");

	if (module) {
		CalculatePopupWindowPositionFuncHandle = (CalculatePopupWindowPositionFunc)GetProcAddress(module, "CalculatePopupWindowPosition");
		if (CalculatePopupWindowPositionFuncHandle == NULL) {
			winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
			return;
		}
	} else {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}

	point.x = x;
	point.y = y;
	size.cx = width;
	size.cy = height;

	/* exclude RECT is NULL if nothing is passed */
	if(exclude_array) {
		exclude_rect = ecalloc(1, sizeof(RECT));
		exclude_rect->bottom = 0;
		exclude_rect->top = 0;
		exclude_rect->left = 0;
		exclude_rect->right = 0;
		if (zend_hash_find(Z_ARRVAL_P(exclude_array), "left", sizeof("left"), (void **) &value) == SUCCESS) {
			if (SUCCESS == winsystem_juggle_type(*value, IS_LONG TSRMLS_CC) ) {
				exclude_rect->left = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(exclude_array), "right", sizeof("right"), (void **) &value) == SUCCESS) {
			if (SUCCESS == winsystem_juggle_type(*value, IS_LONG TSRMLS_CC) ) {
				exclude_rect->right = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(exclude_array), "top", sizeof("top"), (void **) &value) == SUCCESS) {
			if (SUCCESS == winsystem_juggle_type(*value, IS_LONG TSRMLS_CC) ) {
				exclude_rect->top = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(exclude_array), "bottom", sizeof("bottom"), (void **) &value) == SUCCESS) {
			if (SUCCESS == winsystem_juggle_type(*value, IS_LONG TSRMLS_CC) ) {
				exclude_rect->bottom = Z_LVAL_PP(value);
			}
		}
	} else {
		exclude_rect = (RECT *) NULL;
	}

	CalculatePopupWindowPositionFuncHandle(&point, &size, flags, exclude_rect, &new_position);

	if (exclude_array) {
		efree(exclude_rect);
	}

	array_init(return_value);
	add_assoc_long(return_value, "left", new_position.left);
	add_assoc_long(return_value, "right", new_position.right);
	add_assoc_long(return_value, "bottom", new_position.bottom);
	add_assoc_long(return_value, "top", new_position.top);
}
/* }}} */














/* {{{ proto bool Win\Gui\Container::soundSentry(void)
       triggers a visual signal to indicate that a sound is playing */
PHP_METHOD(WinGuiWindow, soundSentry)
{
	zend_error_handling error_handling;
	HMODULE module;

	typedef BOOL (WINAPI *SoundSentryFunc)(VOID);
	SoundSentryFunc SoundSentryFuncHandle;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* Short Circuit if we're NOT on vista */
	if(!wingui_is_vista(TSRMLS_C)) {
		zend_throw_exception(ce_wingui_versionexception, "Win\\Gui\\Container::soundSentry() does not work on Windows versions before Vista", 0 TSRMLS_CC);
		return;
	}
	
	module = GetModuleHandle("user32.dll");

	if (module) {
		SoundSentryFuncHandle = (SoundSentryFunc)GetProcAddress(module, "SoundSentry");
		if (SoundSentryFuncHandle == NULL) {
			winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
			return;
		}
	} else {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}

	RETVAL_BOOL(SoundSentryFuncHandle());
}
/* }}} */

/* {{{ proto int Win\Gui\Window->tile([int flags, array rect, array kids])
       Tiles the specified children of the window */
PHP_METHOD(WinGuiWindow, tile)
{
	zend_error_handling error_handling;
	zval *rect_zval = NULL, **value;
	long tile_flags = 0;
	RECT *rect;
	UINT num_kids = 0;
	HWND *kids;
	WORD total;
	HashTable *kids_hash = NULL;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|laH", &tile_flags, &rect_zval, &kids_hash) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* RECT is NULL if nothing is passed */
	if(rect_zval) {
		rect = ecalloc(1, sizeof(RECT));
		rect->bottom = 0;
		rect->top = 0;
		rect->left = 0;
		rect->right = 0;
		if (zend_hash_find(Z_ARRVAL_P(rect_zval), "left", sizeof("left"), (void **) &value) == SUCCESS) {
			if (SUCCESS == winsystem_juggle_type(*value, IS_LONG TSRMLS_CC) ) {
				rect->left = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(rect_zval), "right", sizeof("right"), (void **) &value) == SUCCESS) {
			if (SUCCESS == winsystem_juggle_type(*value, IS_LONG TSRMLS_CC) ) {
				rect->right = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(rect_zval), "top", sizeof("top"), (void **) &value) == SUCCESS) {
			if (SUCCESS == winsystem_juggle_type(*value, IS_LONG TSRMLS_CC) ) {
				rect->top = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(rect_zval), "bottom", sizeof("bottom"), (void **) &value) == SUCCESS) {
			if (SUCCESS == winsystem_juggle_type(*value, IS_LONG TSRMLS_CC) ) {
				rect->bottom = Z_LVAL_PP(value);
			}
		}
	} else {
		rect = (RECT *) NULL;
	}

	/* walk the kids hash and look for legal containers */
	if(kids_hash) {
		/* emalloc our space as number of elements in the hash - that MIGHT be too big if we hit bad values */
		kids = emalloc(zend_hash_num_elements(kids_hash) * sizeof(HANDLE));

		/* iterate the array, each value inside MUST be an instance of Win\Gui\Container */
		for(zend_hash_internal_pointer_reset(kids_hash); zend_hash_has_more_elements(kids_hash) == SUCCESS; zend_hash_move_forward(kids_hash)) { 
			if (zend_hash_get_current_data(kids_hash, (void **)&value) == FAILURE) {
				continue; 
			}

			/* Check for instanceof */
			if (Z_TYPE_PP(value) == IS_OBJECT && instanceof_function(Z_OBJCE_PP(value), ce_wingui_windowing TSRMLS_CC)) {
				/* we have success, grab the internal window object and stick it in the array */
				wingui_window_object *child_object = (wingui_window_object*)zend_object_store_get_object(*value TSRMLS_CC);
				kids[++num_kids] = child_object->window_handle;
			}
		}

	} else {
		kids = NULL;
	}

	/* make the call */
	total = TileWindows(window_object->window_handle, tile_flags, rect, num_kids, kids);
	
	if (rect_zval) {
		efree(rect);
	}
	if (kids) {
		efree(kids);
	}
	if (total < 1) {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
	} else {
		RETURN_LONG(total);
	}
}
/* }}} */

/* {{{ proto Win\Gui\Window object Win\Gui\Window::getFromPhysicalPoint(x, y)
       Returns a value from a specific physical point, will not return disabled or hidden items
       this will NOT currently work on non-wingui generated windows */
PHP_METHOD(WinGuiWindow, getFromPhysicalPoint)
{
	zend_error_handling error_handling;
	POINT point;
	long x, y;
	HWND handle;
	HMODULE module;
	wingui_window_object *window_object;

	typedef HWND (WINAPI *WindowFromPhysicalPointFunc)(POINT);
	WindowFromPhysicalPointFunc WindowFromPhysicalPointFuncHandle;

	/* Short Circuit if we're NOT on vista */
	if(!wingui_is_vista(TSRMLS_C)) {
		zend_throw_exception(ce_wingui_versionexception, "Win\\Gui\\Window::getFromPhysicalPoint() does not work on Windows versions before Vista", 0 TSRMLS_CC);
		return;
	}

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	point.x = x;
	point.y = y;
	
	module = GetModuleHandle("user32.dll");

	if (module) {
		WindowFromPhysicalPointFuncHandle = (WindowFromPhysicalPointFunc)GetProcAddress(module, "WindowFromPhysicalPoint");
		if (WindowFromPhysicalPointFuncHandle == NULL) {
			winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
			return;
		}
	} else {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}

	handle = WindowFromPhysicalPointFuncHandle(point);

	if(handle == NULL) {
		return;
	}

	window_object = (wingui_window_object *)GetWindowLongPtr(handle, GWLP_USERDATA);
	if (window_object == NULL || window_object->object_zval == NULL) {
		/* TODO some day: get the window class get the appropriate CE besed on the class returned */
		zend_throw_exception(ce_wingui_exception, "Win\\Gui\\Window::getFromPhysicalPoint() could not retrieve the requested window - it may have not been created with wingui", 0 TSRMLS_CC);
		return;
	} else {
		zval_dtor(return_value);
		*return_value = *window_object->object_zval;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
	}
}
/* }}} */


/* {{{ proto Win\Gui\Window object Win\Gui\Window::getFromPoint(x, y)
       Returns a value from a specific physical point, will not return disabled or hidden items
       but will NOT currently work on non-wingui generated windows */
PHP_METHOD(WinGuiWindow, getFromPoint)
{
	zend_error_handling error_handling;
	POINT point;
	long x, y;
	HWND handle;
	wingui_window_object *window_object;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	point.x = x;
	point.y = y;

	handle = WindowFromPoint(point);

	if(handle == NULL) {
		return;
	}

	window_object = (wingui_window_object *)GetWindowLongPtr(handle, GWLP_USERDATA);
	if (window_object == NULL || window_object->object_zval == NULL) {
		/* TODO some day: get the window class get the appropriate CE besed on the class returned */
		zend_throw_exception(ce_wingui_exception, "Win\\Gui\\Window::getFromPoint() could not retrieve the requested window - it may have not been created with wingui", 0 TSRMLS_CC);
		return;
	} else {
		zval_dtor(return_value);
		*return_value = *window_object->object_zval;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
	}
}
/* }}} */

/* {{{ proto bool Win\Gui\Window::registerClass(string|object name, array options)
        allows one to create a "class type" with a default style,
        icon, small icon, background, cursor, and menu
        class types are identified by name, passing a unicode name
        will create a unicode class */
PHP_METHOD(WinGuiWindow, registerClass)
{
	/* Used for regular string */
	char * name = NULL;
	int name_length;

	/* Used for wchar string */
	zval *unicode = NULL;
	winsystem_unicode_object *unicode_object = NULL;
	int use_unicode = 0;

	/* Default struct values */
	int style = 0;
	LPCSTR menu_name = NULL;
	LPCWSTR menu_nameW = NULL;
	HICON icon = NULL;
	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	HBRUSH background = GetSysColorBrush(COLOR_WINDOW);
	HICON small_icon = NULL;

	ATOM worked;
	HashTable *options = NULL;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);

	/* version one, use unicode */
	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "OH", &unicode, ce_winsystem_unicode, &options) != FAILURE) {
		use_unicode = 1;
		unicode_object = (winsystem_unicode_object *)zend_object_store_get_object(unicode TSRMLS_CC);
	} else if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sH", &name, &name_length, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* parse out any options */
	if(options) {
		zval **value;

		if (zend_hash_find(options, "style", sizeof("style"), (void **) &value) == SUCCESS) {
			if (winsystem_juggle_type(*value, IS_LONG TSRMLS_CC) == SUCCESS){
				style = Z_LVAL_PP(value);
			}
		}
		// TODO: icon handling
		if (zend_hash_find(options, "icon", sizeof("style"), (void **) &value) == SUCCESS) {
			// MUST BE HICON
		}
		if (zend_hash_find(options, "small_icon", sizeof("small_icon"), (void **) &value) == SUCCESS) {
			// MUST BE HICON
		}
		if (zend_hash_find(options, "background", sizeof("background"), (void **) &value) == SUCCESS) {
			if (Z_TYPE_PP(value) == IS_OBJECT && instanceof_function(Z_OBJCE_PP(value), ce_wingdi_brush TSRMLS_CC)) {
				wingdi_brush_object *brush_object = (wingdi_brush_object *)zend_object_store_get_object(*value TSRMLS_CC);
				background = brush_object->brush_handle;
			}
		}
		if (zend_hash_find(options, "cursor", sizeof("cursor"), (void **) &value) == SUCCESS) {
			if (Z_TYPE_PP(value) == IS_OBJECT && instanceof_function(Z_OBJCE_PP(value), ce_wingui_resource_cursor TSRMLS_CC)) {
				wingui_resource_object *cursor_object = (wingui_resource_object *)zend_object_store_get_object(*value TSRMLS_CC);
				cursor = cursor_object->resource_handle;
			}
		}
		// TODO: menu handling
		if (zend_hash_find(options, "menu", sizeof("menu"), (void **) &value) == SUCCESS) {
			// can be int (makeintresource fun)
			// or unicode/string (must match is_unicode setting)
		}
	}

	if (use_unicode) {
		WNDCLASSEXW wcx = { 0 };

		/* these values cannot be changed */
		wcx.cbSize = sizeof(wcx);
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = sizeof(wingui_window_object *);
		wcx.hInstance = GetModuleHandleW(NULL);
		wcx.lpfnWndProc = DefWindowProc;

		/* these values can be changed by the user */
		wcx.style = style;
		wcx.lpszMenuName = menu_nameW;
		wcx.hIcon = icon;
		wcx.hCursor = cursor;
		wcx.hbrBackground = background;
		wcx.hIconSm = small_icon;

		wcx.lpszClassName = unicode_object->unicode_string;
		worked = RegisterClassExW(&wcx);
	} else {
		WNDCLASSEX wcx = { 0 };

		/* these values cannot be changed */
		wcx.cbSize = sizeof(wcx);
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = sizeof(wingui_window_object *);
		wcx.hInstance = GetModuleHandleW(NULL);
		wcx.lpfnWndProc = DefWindowProc;

		/* these values can be changed by the user */
		wcx.style = style;
		wcx.lpszMenuName = menu_name;
		wcx.hIcon = icon;
		wcx.hCursor = cursor;
		wcx.hbrBackground = background;
		wcx.hIconSm = small_icon;

		wcx.lpszClassName = name;
		worked = RegisterClassExA(&wcx);
	}

	if (0 == worked) {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
	} else {
		RETURN_TRUE
	}
}
/* }}} */

/* {{{ proto bool Win\Gui\Window::unregisterClass(string|object name)
        removes a "class type"*/
PHP_METHOD(WinGuiWindow, unregisterClass)
{
	/* Used for regular string */
	char * name = NULL;
	int name_length;

	/* Used for wchar string */
	zval *unicode = NULL;
	winsystem_unicode_object *unicode_object = NULL;
	int use_unicode = 0;

	ATOM worked;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);

	/* version one, use unicode */
	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "O", &unicode, ce_winsystem_unicode) != FAILURE) {
		use_unicode = 1;
		unicode_object = (winsystem_unicode_object *)zend_object_store_get_object(unicode TSRMLS_CC);
	} else if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_length) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (use_unicode) {
		worked = UnregisterClassW(unicode_object->unicode_string, GetModuleHandleW(NULL));
	} else {
		worked = UnregisterClass(name, GetModuleHandle(NULL));
	}

	if (0 == worked) {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
	} else {
		RETURN_TRUE
	}
}
/* }}} */



/* Window Methods */
static zend_function_entry wingui_window_functions[] = {
	PHP_ME(WinGuiWindow, allowSetForeground, WinGuiWindow_allowSetForeground_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, getPopupPosition, WinGuiWindow_getPopupPosition_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, bringToTop, WinGuiWindow_bringToTop_args, ZEND_ACC_PUBLIC)

	PHP_ME(WinGuiWindow, soundSentry, WinGuiWindow_soundSentry_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, tile, WinGuiWindow_tile_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, getFromPhysicalPoint, WinGuiWindow_getFromPhysicalPoint_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, getFromPoint, WinGuiWindow_getFromPoint_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

	/* Window Class Stuff */
	PHP_ME(WinGuiWindow, registerClass, WinGuiWindow_registerClass_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, unregisterClass, WinGuiWindow_unregisterClass_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

	/* Implement messaging */
	PHP_ME(WinGuiMessaging, connect, WinGuiMessaging_connect_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, connectFull, WinGuiMessaging_connectFull_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, connectSimple, WinGuiMessaging_connectSimple_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, disconnect, WinGuiMessaging_disconnect_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, get, WinGuiMessaging_get_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, peek, WinGuiMessaging_peek_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, post, WinGuiMessaging_post_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, send, WinGuiMessaging_send_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, sendNotify, WinGuiMessaging_sendNotify_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, sendCallback, WinGuiMessaging_sendCallback_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, sendTimeout, WinGuiMessaging_sendTimeout_args, ZEND_ACC_PUBLIC)

	/* Implement Windowing */
	PHP_ME(WinGuiWindowing, adjustSize, WinGuiWindowing_adjustSize_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, animateShow, WinGuiWindowing_animateShow_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, animateHide, WinGuiWindowing_animateHide_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, getNext, WinGuiWindowing_getNext_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, getPrevious, WinGuiWindowing_getPrevious_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Window Custom Object magic                               
------------------------------------------------------------------*/

/* ----------------------------------------------------------------
  Win\Gui\Window LifeCycle Functions                               
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_window)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Window", wingui_window_functions);
	ce_wingui_window = zend_register_internal_class(&ce TSRMLS_CC);

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
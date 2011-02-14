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
#include "php_winsystem_api.h"
#include "php_wingdi_api.h"

#include "zend_exceptions.h"

#include "implement_messaging.h"
#include "implement_windowing.h"

/* All the classes in this file */
zend_class_entry *ce_wingui_window;

static HashTable wingui_window_prop_handlers;
static HashTable wingui_window_callback_map;
static zend_object_handlers wingui_window_object_handlers;
static zend_function wingui_window_constructor_wrapper;

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

/**
 * Window Class related methods
 */


/* {{{ proto void Win\Gui\Window::__construct([type, text, parent, ][array options])
     Create a new Window - types are TOPLEVEL (default), POPUP or CHILD
     x and y are in client coordinates not screen coordinates,
     height and width are client area, not window area */
PHP_METHOD(WinGuiWindow, __construct)
{
	zend_error_handling error_handling;
	HashTable *options = NULL;
	HWND handle;

	zend_bool use_unicode = FALSE;
	char * name = "Title";
	wchar_t * unicode = L"Title";

	long x = CW_USEDEFAULT, y = CW_USEDEFAULT, width = CW_USEDEFAULT, height = CW_USEDEFAULT,
		style = 0, extrastyle = 0;

	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	window_object->object_zval = getThis();

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|H", &options) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (options) {
		zval **value;

		wingui_window_object_get_basics(options, &use_unicode, &name, &unicode,
			&x, &y, &width, &height TSRMLS_CC);

		/* These are special settings that are only available through the constructor, not as properties because
		  they can be manipulated programatically after creating the window - they affect the "initial state" */
		if (zend_hash_find(options, "maximize", sizeof("maximize"), (void **) &value) == SUCCESS) {
			if (winsystem_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				style |= WS_MAXIMIZE;
			}
		}
		if (zend_hash_find(options, "minimize", sizeof("minimize"), (void **) &value) == SUCCESS) {
			if (winsystem_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				style |= WS_MINIMIZE;
			}
		}
		if (zend_hash_find(options, "visible", sizeof("visible"), (void **) &value) == SUCCESS) {
			if (winsystem_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				style |= WS_VISIBLE;
			}
		}
		if (zend_hash_find(options, "disabled", sizeof("disabled"), (void **) &value) == SUCCESS) {
			if (winsystem_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				style |= WS_DISABLED;
			}
		}
		if (zend_hash_find(options, "deactivated", sizeof("deactivated"), (void **) &value) == SUCCESS) {
			if (winsystem_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				extrastyle |= WS_EX_NOACTIVATE;
			}
		}
		if (zend_hash_find(options, "topmost", sizeof("topmost"), (void **) &value) == SUCCESS) {
			if (winsystem_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				extrastyle |= WS_EX_TOPMOST;
			}
		}
	}

	if (use_unicode) {
		handle = CreateWindowExW(
			extrastyle,
			L"php_wingui_window_unicode", 
			unicode,
			style,
			x,
			y,
			width,
			height,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			(LPVOID) window_object);
	} else {
		handle = CreateWindowExA(
			extrastyle,
			"php_wingui_window", 
			name,
			style,
			x,
			y,
			width,
			height,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			(LPVOID) window_object);
	}

	if (!handle) {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} else {
		window_object->window_handle = handle;
		window_object->is_constructed = TRUE;
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR) window_object);
	}
}
/* }}} */

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



/* Window Methods - why are there three of these?  Because altering the struct at runtime is a lesson
   in pain, far easier to just do three structs and register the right set in MINIT depending on platform */
static zend_function_entry wingui_window_functions_win7[] = {
	PHP_ME(WinGuiWindow, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)

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
	PHP_ME(WinGuiWindowing, hide, WinGuiWindowing_hide_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, show, WinGuiWindowing_show_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

static zend_function_entry wingui_window_functions_vista[] = {
	PHP_ME(WinGuiWindow, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)

	PHP_ME(WinGuiWindow, allowSetForeground, WinGuiWindow_allowSetForeground_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
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
	PHP_ME(WinGuiWindowing, hide, WinGuiWindowing_hide_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, show, WinGuiWindowing_show_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

static zend_function_entry wingui_window_functions_xp[] = {
	PHP_ME(WinGuiWindow, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)

	PHP_ME(WinGuiWindow, allowSetForeground, WinGuiWindow_allowSetForeground_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, bringToTop, WinGuiWindow_bringToTop_args, ZEND_ACC_PUBLIC)

	PHP_ME(WinGuiWindow, tile, WinGuiWindow_tile_args, ZEND_ACC_PUBLIC)
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
	PHP_ME(WinGuiWindowing, hide, WinGuiWindowing_hide_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, show, WinGuiWindowing_show_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Window Custom Object magic                               
------------------------------------------------------------------*/



/* {{{ wingui_window_get_constructor
       gets the constructor for the class  */
static zend_function *wingui_window_get_constructor(zval *object TSRMLS_DC)
{
	/* Could always return constr_wrapper_fun, but it's uncessary to call the
	 * wrapper if instantiating the superclass */
	if (Z_OBJCE_P(object) == ce_wingui_window)
		return zend_get_std_object_handlers()->
			get_constructor(object TSRMLS_CC);
	else
		return &wingui_window_constructor_wrapper;
}
/* }}} */

/* cleans up the handle object */
void wingui_window_object_free(void *object TSRMLS_DC)
{
	wingui_window_object *window_object = (wingui_window_object *)object;
	HWND parent;
	int style;

	/* if our window object was temporary, we have to deref it */
	if (window_object->parent_is_temp)
	{
		style = GetWindowLongPtr(window_object->window_handle, GWL_STYLE);
		/* Grab the stored parent handle */
		if (style & WS_POPUP) {
			parent = GetAncestor(window_object->window_handle, GA_PARENT);
		} else {
			parent = GetParent(window_object->window_handle);
		}

		if (parent && IsWindow(parent)) {
			wingui_window_object *parent_object = (wingui_window_object*)GetWindowLongPtr(parent, GWLP_USERDATA);
			if (parent_object->object_zval) {
				Z_DELREF_P(parent_object->object_zval);
			}
		}
	}
	/*
	if (window_object->data.window.menu) {
		Z_DELREF_P(window_object->data.window.menu);
		window_object->data.window.menu = NULL;
	}
	if (window_object->data.window.image) {
		Z_DELREF_P(window_object->data.window.image);
		window_object->data.window.image = NULL;
	}*/

	zend_hash_destroy(window_object->std.properties);
	FREE_HASHTABLE(window_object->std.properties);

	if(window_object->window_handle != NULL && IsWindow(window_object->window_handle)){
		DestroyWindow(window_object->window_handle);
	}

	/* Destroy window will still pump things into the proc! because of this, we don't destroy the callback
	  map until AFTER we have destroyed the window */
	wingui_messaging_destructor_helper(window_object->registered_callbacks TSRMLS_CC);

	zend_hash_destroy(window_object->registered_callbacks);
	FREE_HASHTABLE(window_object->registered_callbacks);
	
	efree(object);
}
/* }}} */

/* {{{ wingui_window_object_create
       creates a new wingui window object and takes care of the internal junk */
zend_object_value wingui_window_object_create(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_window_object *window_object;

	window_object = ecalloc(1, sizeof(wingui_window_object));
	zend_object_std_init((zend_object *) window_object, ce TSRMLS_CC);

	window_object->is_constructed = FALSE;
	window_object->parent_is_temp = 0;
	window_object->window_handle = NULL;
	window_object->prop_handler = &wingui_window_prop_handlers;
	window_object->callback_map = &wingui_window_callback_map;
	window_object->window_proc = NULL;
	window_object->messages_cracker = wingui_window_messages_cracker;
	window_object->messages_results = wingui_window_messages_results;
	window_object->messages_packer = wingui_window_messages_packer;
#ifdef ZTS
	window_object->TSRMLS_C = TSRMLS_C;
#endif

	zend_hash_copy(window_object->std.properties, &ce->default_properties,
		(copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));

	ALLOC_HASHTABLE(window_object->registered_callbacks);
	zend_hash_init(window_object->registered_callbacks, 0, NULL, NULL, 0);

	retval.handle = zend_objects_store_put(window_object, (zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t) wingui_window_object_free, NULL TSRMLS_CC);
	retval.handlers = &wingui_window_object_handlers;
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Window LifeCycle Functions                               
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_window)
{
	zend_class_entry ce;
	int builtin_function_length = 28;

	memcpy(&wingui_window_object_handlers, &wingui_object_handlers,
		sizeof wingui_window_object_handlers);
	wingui_window_object_handlers.get_constructor = wingui_window_get_constructor;
	//wingui_window_object_handlers.get_debug_info = winsystem_mutex_get_debug_info;

	/* Functions available in Window vary by version - annoyingly */
	if (EG(windows_version_info).dwMajorVersion < 5) {
		INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Window", wingui_window_functions_xp);
	} else if (EG(windows_version_info).dwMajorVersion < 6) {
		INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Window", wingui_window_functions_vista);
	} else {
		INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Window", wingui_window_functions_win7);
	}
	ce_wingui_window = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_window->create_object = wingui_window_object_create;

	//zend_class_implements(ce_wingui_window TSRMLS_CC, 3, ce_wingui_windowing,
	//	ce_wingui_messaging, ce_wingui_inputing);

	wingui_window_constructor_wrapper.type = ZEND_INTERNAL_FUNCTION;
	wingui_window_constructor_wrapper.common.function_name = "internal_construction_wrapper";
	wingui_window_constructor_wrapper.common.scope = ce_wingui_window;
	wingui_window_constructor_wrapper.common.fn_flags = ZEND_ACC_PROTECTED;
	wingui_window_constructor_wrapper.common.prototype = NULL;
	wingui_window_constructor_wrapper.common.required_num_args = 0;
	wingui_window_constructor_wrapper.common.arg_info = NULL;
	wingui_window_constructor_wrapper.common.pass_rest_by_reference = 0;
	wingui_window_constructor_wrapper.common.return_reference = 0;
	wingui_window_constructor_wrapper.internal_function.handler = wingui_object_construction_wrapper;
	wingui_window_constructor_wrapper.internal_function.module = EG(current_module);

	/* Callback map for windows */
	zend_hash_init(&wingui_window_callback_map, 0, NULL, NULL, 1);

	/* Window Notifications */
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_ACTIVATEAPP,				&wingui_window_callback_map,	__onwmactivateapp);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_CANCELMODE,				&wingui_window_callback_map,	__onwmcancelmode);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_CHILDACTIVATE,			&wingui_window_callback_map,	__onwmchildactivate);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_CLOSE,					&wingui_window_callback_map,	__onwmclose);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_CREATE,					&wingui_window_callback_map,	__onwmcreate);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_DESTROY,					&wingui_window_callback_map,	__onwmdestroy);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_ENABLE,					&wingui_window_callback_map,	__onwmenable);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_ENTERSIZEMOVE,			&wingui_window_callback_map,	__onwmentersizemove);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_EXITSIZEMOVE,			&wingui_window_callback_map,	__onwmexitsizemove);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_EXITSIZEMOVE,			&wingui_window_callback_map,	__onwmexitsizemove);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_GETICON,					&wingui_window_callback_map,	__onwmgeticon);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_GETMINMAXINFO,			&wingui_window_callback_map,	__onwmgetminmaxinfo);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_INPUTLANGCHANGE,			&wingui_window_callback_map,	__onwminputlangchange);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_INPUTLANGCHANGEREQUEST,	&wingui_window_callback_map,	__onwminputlangchangerequest);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_MOVE,					&wingui_window_callback_map,	__onwmmove);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_MOVING,					&wingui_window_callback_map,	__onwmmoving);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_NCACTIVATE,				&wingui_window_callback_map,	__onwmncactivate);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_NCCALCSIZE,				&wingui_window_callback_map,	__onwmnccalcsize);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_NCCREATE,				&wingui_window_callback_map,	__onwmnccreate);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_NCDESTROY,				&wingui_window_callback_map,	__onwmdestroy);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_NULL,					&wingui_window_callback_map,	__onwmnull);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_PARENTNOTIFY,			&wingui_window_callback_map,	__onwmparentnotify);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_QUERYDRAGICON,			&wingui_window_callback_map,	__onwmquerydragicon);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_QUERYOPEN,				&wingui_window_callback_map,	__onwmqueryopen);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_SHOWWINDOW,				&wingui_window_callback_map,	__onwmshowwindow);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_SIZE,					&wingui_window_callback_map,	__onwmsize);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_SIZING,					&wingui_window_callback_map,	__onwmsizing);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_STYLECHANGED,			&wingui_window_callback_map,	__onwmstylechanged);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_STYLECHANGING,			&wingui_window_callback_map,	__onwmstylechanging);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_THEMECHANGED,			&wingui_window_callback_map,	__onwmthemechanged);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_USERCHANGED,				&wingui_window_callback_map,	__onwmuserchanged);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_WINDOWPOSCHANGED,		&wingui_window_callback_map,	__onwmwindowposchanged);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_WINDOWPOSCHANGING,		&wingui_window_callback_map,	__onwmwindowposchanging);

	/* Window Messages */
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	MN_GETHMENU,				&wingui_window_callback_map,	__onmngethmenu);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_ERASEBKGND,				&wingui_window_callback_map,	__onerasebkgnd);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_GETFONT,					&wingui_window_callback_map,	__onwmgetfont);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_GETTEXT,					&wingui_window_callback_map,	__onwmgettext);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_GETTEXTLENGTH,			&wingui_window_callback_map,	__onwmtettextlength);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_SETFONT,					&wingui_window_callback_map,	__onwmsetfont);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_SETICON,					&wingui_window_callback_map,	__onwmseticon);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_SETTEXT,					&wingui_window_callback_map,	__onwmsettext);

	/* Callback map for properties */
	zend_hash_init(&wingui_window_prop_handlers, 0, NULL, NULL, 1);
	zend_hash_copy(&wingui_window_prop_handlers, &wingui_windowing_prop_handlers, NULL, NULL, sizeof(wingui_prop_handler));

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION(wingui_window)
       frees the callback map and property handler hash */
PHP_MSHUTDOWN_FUNCTION(wingui_window)
{
	zend_hash_destroy(&wingui_window_callback_map);
	zend_hash_destroy(&wingui_window_prop_handlers);
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
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
#include "windowing.h"
#include "inputing.h"

zend_class_entry *ce_wingui_window;
static zend_object_handlers wingui_window_object_handlers;

struct _wingui_window_object {
	zend_object std;
	zend_bool   is_constructed;
	HWND        window_handle;
	WNDPROC     window_proc;
#ifdef ZTS
	TSRMLS_D;
#endif
};

/* ----------------------------------------------------------------
    Win\Gui\Window C API
------------------------------------------------------------------*/

/* {{{ function to take a zval** window instance and give you back the window handle */
HWND wingui_window_get_handle(zval** window TSRMLS_DC)
{
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(*window TSRMLS_CC);

	return window_object->window_handle;
}
/* }}} */

/* {{{ function to take a hash table and get title/unicode settings from it */
int wingui_window_get_text(HashTable *options, zend_bool *use_unicode, char ** title, wchar_t ** titlew TSRMLS_DC)
{
	zval **value;

	/* First check to see if forcing unicode is on */
	if (zend_hash_find(options, "use_unicode", sizeof("use_unicode"), (void **) &value) == SUCCESS) {
		convert_to_boolean(*value);
		*use_unicode = Z_BVAL_PP(value);
	}

	/* if force unicode is on OR our title is an instanceof unicode object, use wchar as title */
	if (zend_hash_find(options, "text", sizeof("text"), (void **) &value) == SUCCESS) {
		if (Z_TYPE_PP(value) == IS_OBJECT && instanceof_function(Z_OBJCE_PP(value), ce_winsystem_unicode TSRMLS_CC)) {
			*use_unicode = TRUE;
			*titlew = php_winsystem_unicode_get_wchar(value TSRMLS_CC);
		/* we assume if someone is forcing unicode, they're sending utf8, otherwise they need to use unicode object instead */
		} else if (TRUE == *use_unicode) {
			convert_to_string(*value);
			*titlew = php_winsystem_unicode_char_to_wchar(&Z_STRVAL_PP(value), CP_UTF8);
		} else {
			convert_to_string(*value);
			*title = Z_STRVAL_PP(value);
		}
	}

	return SUCCESS;
}
/* }}} */

/* {{{ function to figure the x,y,height and width set in a window */
int wingui_window_get_size(HashTable *options, long *x, long *y, long *width, long *height TSRMLS_DC)
{
	zval **value;
	if (SUCCESS == zend_hash_find(options, "x", sizeof("x"), (void **) &value)) {
		convert_to_long(*value);
		*x = Z_LVAL_PP(value);
	}
	if (SUCCESS == zend_hash_find(options, "y", sizeof("y"), (void **) &value)) {
		convert_to_long(*value);
		*y = Z_LVAL_PP(value);
	}
	if (SUCCESS == zend_hash_find(options, "width", sizeof("width"), (void **) &value)) {
		convert_to_long(*value);
		*width = Z_LVAL_PP(value);
	}
	if (SUCCESS == zend_hash_find(options, "height", sizeof("height"), (void **) &value)) {
		convert_to_long(*value);
		*height = Z_LVAL_PP(value);
	}
	return SUCCESS;
}
/* }}} */

/* {{{ manages the window style information */
int wingui_window_get_style(HashTable *options, long *style, long *extrastyle TSRMLS_DC)
{
	zval **value;
	if (SUCCESS == zend_hash_find(options, "disable", sizeof("disable"), (void **) &value)) {
		*style |= WS_DISABLED;
	}
	if (SUCCESS == zend_hash_find(options, "noactivate", sizeof("noactivate"), (void **) &value)) {
		/* only works on toplevel, so child and popup style cannot be set */
		if (!((*style & WS_CHILD) || (*style & WS_POPUP))) {
			*extrastyle |= WS_EX_NOACTIVATE;
		}
	}
	return SUCCESS;
}
/* }}} */

/* ----------------------------------------------------------------
    Win\Gui\Window class API
------------------------------------------------------------------*/

ZEND_BEGIN_ARG_INFO(WinGuiWindow___construct_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\Window::__construct([array options])
     Creates a new - has many different options available
	 type => enum for TOPLEVEL, POPUP or CHILD

     x and y are in client coordinates not screen coordinates,
     height and width are client area, not window area */
PHP_METHOD(WinGuiWindow, __construct)
{
	HashTable *options = NULL;

	/* Defaults for the window */
	zend_bool use_unicode = FALSE;
	char *title = "Window", *classname = "php_wingui_window_ansi";
	wchar_t *titlew = L"Window", *classnamew = L"php_wingui_window_unicode";
	long x = CW_USEDEFAULT, y = CW_USEDEFAULT, width = CW_USEDEFAULT, height = CW_USEDEFAULT,
		style = WS_OVERLAPPEDWINDOW, extrastyle = 0;
	zval *parent = NULL, *menu = NULL;
	HWND parent_handle = NULL, window_handle = NULL;
	HMENU menu_handle = NULL;
	wingui_window_object *window_object;

	PHP_WINGUI_EXCEPTIONS
	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "H", &options)) {
		return;
	}
	PHP_WINGUI_RESTORE_ERRORS

	window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	wingui_window_get_text(options, &use_unicode, &title, &titlew TSRMLS_CC);
	wingui_window_get_size(options, &x, &y, &width, &height TSRMLS_CC);
	wingui_window_get_style(options, &style, &extrastyle TSRMLS_CC);

	// TODO: rest of style and extrastyle parsing
	// TODO: parent
	// TODO: menu
	// TODO: classname

	/* parse options hash
	if (options) {
		zval **value;

		/* These are special settings that are only available through the constructor, not as properties because
		  they can be manipulated programatically after creating the window - they affect the "initial state" 
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
	} */

	if (TRUE == use_unicode) {
		window_handle = CreateWindowExW(
			extrastyle,
			classnamew, 
			titlew,
			style,
			x,
			y,
			width,
			height,
			parent_handle,
			menu_handle,
			GetModuleHandle(NULL),
			(LPVOID) window_object);
	} else {
		window_handle = CreateWindowExA(
			extrastyle,
			classname, 
			title,
			style,
			x,
			y,
			width,
			height,
			parent_handle,
			menu_handle,
			GetModuleHandle(NULL),
			(LPVOID) window_object);
	}

	if (!window_handle) {
		winsystem_create_error(GetLastError(), ce_winsystem_runtimeexception TSRMLS_CC);
		return;
	} else {
		window_object->window_handle = window_handle;
		window_object->is_constructed = TRUE;
		if (TRUE == use_unicode) {
			SetWindowLongPtrW(window_handle, GWLP_USERDATA, (LONG_PTR) window_object);
		} else {
			SetWindowLongPtrA(window_handle, GWLP_USERDATA, (LONG_PTR) window_object);
		}
	}
}
/* }}} */

/* ----------------------------------------------------------------
     Win\Gui\Window Object management
------------------------------------------------------------------*/
/* cleans up the handle object */
void wingui_window_object_free(void *object TSRMLS_DC)
{
	wingui_window_object *window_object = (wingui_window_object *)object;
	//HWND parent;

	/* if our window object was temporary, we have to deref it 
	if (window_object->parent_is_temp)
	{
		style = GetWindowLongPtr(window_object->window_handle, GWL_STYLE);
		/* Grab the stored parent handle 
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

	if(window_object->window_handle != NULL && IsWindow(window_object->window_handle)){
		DestroyWindow(window_object->window_handle);
	}

	/* Destroy window will still pump things into the proc! because of this, we don't destroy the callback
	  map until AFTER we have destroyed the window */
	//wingui_messaging_destructor_helper(window_object->registered_callbacks TSRMLS_CC);

	//zend_hash_destroy(window_object->registered_callbacks);
	//FREE_HASHTABLE(window_object->registered_callbacks);
	
	/* Finally kill the object and tell us it's not constructed */
	zend_object_std_dtor(&window_object->std TSRMLS_CC);
	window_object->is_constructed = FALSE;
	window_object->window_proc = NULL;

	efree(window_object);
}
/* }}} */

/* {{{ wingui_window_object_create
       creates a new wingui window object and takes care of the internal junk */
static zend_object_value wingui_window_object_create(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_window_object *window_object;

	window_object = ecalloc(1, sizeof(wingui_window_object));
	zend_object_std_init((zend_object *) window_object, ce TSRMLS_CC);

	window_object->is_constructed = FALSE;
	window_object->window_handle = NULL;
	window_object->window_proc = NULL;
#ifdef ZTS
	window_object->TSRMLS_C = TSRMLS_C;
#endif

	object_properties_init(&window_object->std, ce);

	//ALLOC_HASHTABLE(window_object->registered_callbacks);
	//zend_hash_init(window_object->registered_callbacks, 0, NULL, NULL, 0);

	retval.handle = zend_objects_store_put(window_object, (zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t) wingui_window_object_free, NULL TSRMLS_CC);
	retval.handlers = &wingui_window_object_handlers;
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
     Win\Gui\Window window message processing
------------------------------------------------------------------*/

LRESULT CALLBACK wingui_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (IsWindowUnicode(hwnd)) {
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	} else {
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
}

/* ----------------------------------------------------------------
    Win\Gui\Window Definition and registration
------------------------------------------------------------------*/

/* {{{ class methods */
static const zend_function_entry wingui_window_methods[] = {
	PHP_ME(WinGuiWindow, __construct, WinGuiWindow___construct_args, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)

	/* Windowing implementation */
	PHP_ME(WinGuiWindowing, isVisible, WinGuiWindowing_isVisible_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, show, WinGuiWindowing_show_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, hide, WinGuiWindowing_hide_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, getSize, WinGuiWindowing_getSize_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, getPos, WinGuiWindowing_getPos_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, getText, WinGuiWindowing_getText_args, ZEND_ACC_PUBLIC)

	/* Inputing implementation */
	PHP_ME(WinGuiInputing, enable, WinGuiInputing_enable_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, disable, WinGuiInputing_disable_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, isEnabled, WinGuiInputing_isEnabled_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, activate, WinGuiInputing_activate_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, deactivate, WinGuiInputing_deactivate_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, isActive, WinGuiInputing_isActive_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, focus, WinGuiInputing_focus_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, unfocus, WinGuiInputing_unfocus_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, hasFocus, WinGuiInputing_hasFocus_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, registerHotKey, WinGuiInputing_registerHotKey_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, unregisterHotKey, WinGuiInputing_unregisterHotKey_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, capture, WinGuiInputing_capture_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, release, WinGuiInputing_release_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, hasCapture, WinGuiInputing_hasCapture_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, trackMouse, WinGuiInputing_trackMouse_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, dragDetect, WinGuiInputing_dragDetect_args, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(wingui_window)
{
	WNDCLASSEXA wcx = { 0 };
	WNDCLASSEXW wcxw = { 0 };
	ATOM worked;
	zend_class_entry ce;

	/* set up two identical structs for registering default windows */
	wcx.cbSize = sizeof(wcx);
	wcxw.cbSize = sizeof(wcxw);
	wcx.cbClsExtra = 0;
	wcxw.cbClsExtra = 0;
	wcx.cbWndExtra = sizeof(wingui_window_object *);
	wcxw.cbWndExtra = sizeof(wingui_window_object *);
	wcx.hInstance = GetModuleHandleW(NULL);
	wcxw.hInstance = GetModuleHandleA(NULL);
	wcx.lpfnWndProc = wingui_proc_handler;
	wcxw.lpfnWndProc = wingui_proc_handler;
	wcx.lpszMenuName =  NULL;
	wcxw.lpszMenuName =  NULL;
	wcx.lpszClassName = "php_wingui_window_ansi";
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
	}

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Window", wingui_window_methods);
	ce_wingui_window = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(ce_wingui_window TSRMLS_CC, 1, ce_wingui_windowing);
	ce_wingui_window->create_object = wingui_window_object_create;

	memcpy(&wingui_window_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(wingui_window)
{
	BOOL worked;

	worked = UnregisterClassA("php_wingui_window_ansi", GetModuleHandle(NULL));
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
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
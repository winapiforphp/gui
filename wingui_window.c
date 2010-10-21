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
#include "zend_exceptions.h"

/* All the classes in this file */
zend_class_entry *ce_wingui_window;
zend_class_entry *ce_wingui_window_animate;

/* Custom Object Handler Items */
HashTable wingui_window_prop_handlers;
HashTable wingui_window_callback_map;

ZEND_DECLARE_MODULE_GLOBALS(wingui);

LRESULT CALLBACK wingui_window_object_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT wingui_window_messages_results(int msg, zval *return_value TSRMLS_DC);
void wingui_window_messages_packer(int msg, WPARAM *wParam, LPARAM *lParam, zval ***extra TSRMLS_DC);
zval ***wingui_window_messages_cracker(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC);

/* ----------------------------------------------------------------
  Win\Gui\Window Userland API                                                      
------------------------------------------------------------------*/

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_allowSetForeground_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, process_id)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\Window::__construct([type, text, parent, ][array options])
     Create a new Window - types are TOPLEVEL (default), POPUP or CHILD
     x and y are in client coordinates not screen coordinates,
     height and width are client area, not window area */
PHP_METHOD(WinGuiWindow, __construct)
{
	zend_error_handling error_handling;
	HWND hwnd, parent_handle = NULL; 
	HMENU menu = NULL;
	wingui_window_object *window_obj = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	wingui_window_object *parent_obj;
	wingui_menu_object *menu_object;

	zval *options = NULL, *parent = NULL;
	char *text = "Window Title";
	int style = WS_OVERLAPPEDWINDOW, extrastyle = 0, x = CW_USEDEFAULT, y = CW_USEDEFAULT, height = CW_USEDEFAULT, width = CW_USEDEFAULT;

	/* we want a nice exception here not a warning */
	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a", &options) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* parse out any options sent to the constructor */
	if(options && Z_TYPE_P(options) == IS_ARRAY) {
		zval **value;

		if (zend_hash_find(Z_ARRVAL_P(options), "x", sizeof("x"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_LONG TSRMLS_CC) == SUCCESS){
				x = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "y", sizeof("y"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_LONG TSRMLS_CC) == SUCCESS){
				y = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "width", sizeof("width"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_LONG TSRMLS_CC) == SUCCESS){
				width = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "height", sizeof("height"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_LONG TSRMLS_CC) == SUCCESS){
				height = Z_LVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "text", sizeof("text"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_STRING TSRMLS_CC) == SUCCESS){
				text = Z_STRVAL_PP(value);
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "menu", sizeof("menu"), (void **) &value) == SUCCESS &&
			Z_TYPE_PP(value) == IS_OBJECT &&
			instanceof_function(Z_OBJCE_PP(value), ce_wingui_menu TSRMLS_CC)) {
				menu_object = (wingui_menu_object*)wingui_menu_object_get(*value TSRMLS_CC);
				menu = menu_object->menu_handle;
		}

		/* These are special settings that are only available through the constructor, not as properties because
		  they can be manipulated programatically after creating the window - they affect the "initial state" */
		if (zend_hash_find(Z_ARRVAL_P(options), "maximize", sizeof("maximize"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				style |= WS_MAXIMIZE;
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "minimize", sizeof("minimize"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				style |= WS_MINIMIZE;
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "visible", sizeof("visible"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				style |= WS_VISIBLE;
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "disabled", sizeof("disabled"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				style |= WS_DISABLED;
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "deactivated", sizeof("deactivated"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				extrastyle |= WS_EX_NOACTIVATE;
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "topmost", sizeof("topmost"), (void **) &value) == SUCCESS) {
			if (wingui_juggle_type(*value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_LVAL_PP(value)){
				extrastyle |= WS_EX_TOPMOST;
			}
		}
	}

	if (parent) {
		Z_ADDREF_P(parent);
		window_obj->parent_is_temp = 1;
		parent_obj = (wingui_window_object *)zend_object_store_get_object(parent TSRMLS_CC);
		parent_handle = parent_obj->window_handle;
	}

	hwnd = CreateWindowExA(
		extrastyle,
        "php_wingui_window", 
        text,   
        style, 
        x,  
        y,  
        width, 
        height,
        parent_handle,
        menu,  
        WINGUI_G(hinstance),
        (LPVOID) NULL);

    if (!hwnd) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} else {
		window_obj->window_handle = hwnd;
		window_obj->object_zval = getThis();
		window_obj->window_proc = (WNDPROC) SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR) wingui_proc_handler);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) window_obj);
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

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l!", &process_id) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* If call works properly, return bool, otherwise throw exception */
	worked = AllowSetForegroundWindow(process_id);

	RETVAL_BOOL(worked);
	if (!worked) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
	}
}
/* }}} */

/* register Message methods */
static zend_function_entry wingui_window_functions[] = {
	PHP_ME(WinGuiWindow, __construct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, allowSetForeground, WinGuiWindow_allowSetForeground_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Window Custom Object magic                                                 
------------------------------------------------------------------*/

/* cleans up the handle object */
void wingui_window_object_destructor(void *object TSRMLS_DC)
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
	if (window_object->data.window.menu) {
		Z_DELREF_P(window_object->data.window.menu);
		window_object->data.window.menu = NULL;
	}
	if (window_object->data.window.image) {
		Z_DELREF_P(window_object->data.window.image);
		window_object->data.window.image = NULL;
	}

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

/* {{{ wingui_window_object_new
       creates a new winguiwindow object and takes care of the internal junk */
zend_object_value wingui_window_object_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_window_object *window_object;
	zval *tmp;

	window_object = emalloc(sizeof(wingui_window_object));
	window_object->std.ce = ce;
	window_object->std.guards = NULL;
	window_object->parent_is_temp = 0;
	window_object->window_handle = NULL;
	window_object->prop_handler = &wingui_window_prop_handlers;
	window_object->callback_map = &wingui_window_callback_map;
	window_object->window_proc = NULL;
	window_object->messages_results = wingui_window_messages_results;
	window_object->messages_cracker = wingui_window_messages_cracker;
	window_object->messages_packer = wingui_window_messages_packer;
	//window_object->menu = NULL;
	//window_object->image = NULL;
#ifdef ZTS
	window_object->TSRMLS_C = TSRMLS_C;
#endif

	ALLOC_HASHTABLE(window_object->std.properties);
	zend_hash_init(window_object->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(window_object->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
	ALLOC_HASHTABLE(window_object->registered_callbacks);
	zend_hash_init(window_object->registered_callbacks, 0, NULL, NULL, 0);

	retval.handle = zend_objects_store_put(window_object, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) wingui_window_object_destructor, NULL TSRMLS_CC);
	window_object->handle = retval.handle;
	retval.handlers = &wingui_object_handlers;
	return retval;
}
/* }}} */


/* put lparams and wparams into zvals for progressbar messages */
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

/* put cracked values into lparams, wparams before calling defaultproc */
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
			if(wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC) == SUCCESS) {
				*wParam = Z_LVAL_P(*extra[0]);
			}
			break;
		case WM_GETMINMAXINFO:
			/* maxsize */
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[0]), 0, &value) == SUCCESS) {
				if(wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
					((MINMAXINFO*)*lParam)->ptMaxSize.x = Z_LVAL_P(value);
				}
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[0]), 1, &value) == SUCCESS) {
				if(wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
					((MINMAXINFO*)*lParam)->ptMaxSize.y = Z_LVAL_P(value);
				}
			}
			
			/* maxposition */
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[1]), 0, &value) == SUCCESS) {
				if(wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
					((MINMAXINFO*)*lParam)->ptMaxPosition.x = Z_LVAL_P(value);
				}
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[1]), 1, &value) == SUCCESS) {
				wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMaxPosition.y = Z_LVAL_P(value);
			}

			/* mintracksize */
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[2]), 0, &value) == SUCCESS) {
				wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMinTrackSize.x = Z_LVAL_P(value);
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[2]), 1, &value) == SUCCESS) {
				wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMinTrackSize.y = Z_LVAL_P(value);
			}	

			/* maxtracksize */
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[3]), 0, &value) == SUCCESS) {
				wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMaxTrackSize.x = Z_LVAL_P(value);
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[3]), 1, &value) == SUCCESS) {
				wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMaxTrackSize.y = Z_LVAL_P(value);
			}
			break;
		case WM_INPUTLANGCHANGE:
		case WM_INPUTLANGCHANGEREQUEST:
			wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC);
			*wParam = Z_LVAL_P(*extra[0]);
			wingui_juggle_type(*extra[1], IS_LONG TSRMLS_CC);
			*lParam = Z_LVAL_P(*extra[1]);
			break;
		case WM_MOVE:
			wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC);
			wingui_juggle_type(*extra[1], IS_LONG TSRMLS_CC);
			*lParam = MAKELPARAM(*extra[0], *extra[1]);
			break;
		case WM_SIZE:
			wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC);
			*wParam = Z_LVAL_P(*extra[0]);
			wingui_juggle_type(*extra[1], IS_LONG TSRMLS_CC);
			wingui_juggle_type(*extra[2], IS_LONG TSRMLS_CC);
			*lParam = MAKELPARAM(*extra[1], *extra[2]);
			break;
		case WM_COMMAND:
			/* lparam is 0 we have Menu */
			if (*lParam == 0 && HIWORD(*wParam) == 0) {
				if(wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC) == SUCCESS) {
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


/* put zval return values from from messages into lresult vals */
LRESULT wingui_window_messages_results(int msg, zval *return_value TSRMLS_DC)
{
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
			if (wingui_juggle_type(return_value, IS_LONG TSRMLS_CC) == SUCCESS) {
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

/* ----------------------------------------------------------------
  Win\Gui\Window LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_window)
{
	zend_class_entry ce, animate_ce;
	
	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Window", wingui_window_functions);
	ce_wingui_window = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_window->create_object  = wingui_window_object_new;

	//zend_class_implements(ce_wingui_window TSRMLS_CC, 3, ce_wingui_messaging, ce_wingui_inputing, ce_wingui_windowing);
	zend_class_implements(ce_wingui_window TSRMLS_CC, 2, ce_wingui_windowing, ce_wingui_messaging);
	zend_hash_apply_with_arguments(&(ce_wingui_window->function_table) TSRMLS_CC, (apply_func_args_t) unset_abstract_flag, 1, ce_wingui_windowing);
	zend_hash_apply_with_arguments(&(ce_wingui_window->function_table) TSRMLS_CC, (apply_func_args_t) unset_abstract_flag, 1, ce_wingui_messaging);
	ce_wingui_window->ce_flags &= ~ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;

	/* Callback map for window */
	zend_hash_init(&wingui_window_callback_map, 0, NULL, NULL, 1);

	/* Callback map for properties */
	zend_hash_init(&wingui_window_prop_handlers, 0, NULL, NULL, 1);
	//wingui_register_prop_handler(&wingui_window_prop_handlers, ce_wingui_window, "type", wingui_window_read_type, wingui_window_write_type TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_window_prop_handlers, ce_wingui_window, "x", wingui_window_read_pos, wingui_window_write_size TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_window_prop_handlers, ce_wingui_window, "y", wingui_window_read_pos, wingui_window_write_size TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_window_prop_handlers, ce_wingui_window, "width", wingui_window_read_size, wingui_window_write_size TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_window_prop_handlers, ce_wingui_window, "height", wingui_window_read_size, wingui_window_write_size TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_window_prop_handlers, ce_wingui_window, "text", wingui_window_read_text, wingui_window_write_text TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_window_prop_handlers, ce_wingui_window, "parent", wingui_window_read_parent, wingui_window_write_parent TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_window_prop_handlers, ce_wingui_window, "menu", wingui_window_read_menu, wingui_window_write_menu TSRMLS_CC);

	/* Types of windows - notice this does not map to underlying winapi constants */
	zend_declare_class_constant_long(ce_wingui_window, "TOPLEVEL", sizeof("TOPLEVEL") - 1, 1 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_window, "POPUP", sizeof("POPUP") - 1, 2 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_window, "CHILD", sizeof("CHILD") - 1, 3 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_window, "MESSAGE", sizeof("MESSAGE") - 1, 3 TSRMLS_CC);

	/* Animate properties */
	INIT_NS_CLASS_ENTRY(animate_ce, PHP_WINGUI_NS, ZEND_NS_NAME("Window", "Animate"), NULL);
	ce_wingui_window_animate = zend_register_internal_class(&animate_ce TSRMLS_CC);
	ce_wingui_window_animate->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

	REGISTER_WINGUI_NAMED_CONST(ce_wingui_window_animate, SLIDE, AW_SLIDE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_window_animate, BLEND, AW_BLEND);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_window_animate, BOX, AW_CENTER);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_window_animate, HOR_POSITIVE, AW_HOR_POSITIVE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_window_animate, HOR_NEGATIVE, AW_HOR_NEGATIVE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_window_animate, VER_POSITIVE, AW_VER_POSITIVE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_window_animate, VER_NEGATIVE, AW_VER_NEGATIVE);

	/* Window Messages and Notifications */
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_ACTIVATEAPP,				&wingui_window_callback_map,	__onwmactivateapp);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_CANCELMODE,				&wingui_window_callback_map,	__onwmcancelmode);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_CHILDACTIVATE,			&wingui_window_callback_map,	__onwmchildactivate);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_CLOSE,					&wingui_window_callback_map,	__onwmclose);
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

	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_window,	WM_COMMAND,					&wingui_window_callback_map,	__onwmcommand);

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
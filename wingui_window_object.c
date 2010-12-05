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

zend_object_handlers wingui_window_object_handlers;

/* ----------------------------------------------------------------
  wingui_window_object basic functionality
------------------------------------------------------------------
/* cleans up the handle object 
void wingui_window_object_destructor(void *object TSRMLS_DC)
{
	wingui_window_object *window_object = (wingui_window_object *)object;
	HashTable **callback_table;
	HWND parent;
	int style;

	/* if our window object was temporary, we have to deref it 
	if (window_object->temporary_parent)
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
			if (parent_object->object) {
				Z_DELREF_P(parent_object->object);
			}
		}
	}

	if (window_object->menu) {
		Z_DELREF_P(window_object->menu);
		window_object->menu = NULL;
	}
	if (window_object->image) {
		Z_DELREF_P(window_object->image);
		window_object->image = NULL;
	}

	zend_hash_destroy(window_object->std.properties);
	FREE_HASHTABLE(window_object->std.properties);

	if(window_object->window_handle != NULL && IsWindow(window_object->window_handle)){
		DestroyWindow(window_object->window_handle);
	}

	/* Destroy window will still pump things into the proc! because of this, we don't destroy the callback
	  map until AFTER we have destroyed the window 
	for(zend_hash_internal_pointer_reset(window_object->registered_callbacks);
		zend_hash_get_current_data(window_object->registered_callbacks, (void**)&callback_table) == SUCCESS;
		zend_hash_move_forward(window_object->registered_callbacks)) {
		wingui_callback_t **callback;
		for(zend_hash_internal_pointer_reset(*callback_table);
			zend_hash_get_current_data(*callback_table, (void**)&callback) == SUCCESS;
			zend_hash_move_forward(*callback_table)) {
			if ((*callback)->callback_info.params) {
				zval ***params  = (*callback)->callback_info.params;
				int param_count = (*callback)->callback_info.param_count;

				while (--param_count) {
					// setMsgHandler setup two refcounts to avoid getting referenced 
					zval_ptr_dtor(params[param_count]);
					zval_ptr_dtor(params[param_count]);
				}
			}

			zval_ptr_dtor(&((*callback)->callback_info.function_name));
			if ((*callback)->callback_info.object_ptr) {
				zval_ptr_dtor(&((*callback)->callback_info.object_ptr));
			}
		}
	}

	zend_hash_destroy(window_object->registered_callbacks);
	FREE_HASHTABLE(window_object->registered_callbacks);
	
	efree(object);
}
/* }}} */

/* {{{ wingui_window_object_new
       creates a new wingui window object and takes care of the internal junk 
zend_object_value wingui_window_object_new(zend_class_entry *ce, HashTable *wingui_prop_handlers, HashTable *wingui_callback_map TSRMLS_DC)
{
	zend_object_value retval;
	wingui_window_object *window_object;
	zval *tmp;

	window_object = emalloc(sizeof(wingui_window_object));
	window_object->std.ce = ce;
	window_object->std.guards = NULL;
	window_object->temporary_parent = 0;
	window_object->window_handle = NULL;
	window_object->prop_handler = wingui_prop_handlers;
	window_object->callback_map = wingui_callback_map;
	window_object->window_proc = NULL;
	window_object->menu = NULL;
	window_object->image = NULL;
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

/* ----------------------------------------------------------------
  wingui_window_object window procedure functionality
------------------------------------------------------------------*/

/* Does the callback dispatching, a helper function used by the default window procedure
	The dispatch happens in two steps - first it calls any attached callbacks
	 in order (remember attached messages are pushed onto the top so FILO order)
	 and then any "default" registered handlers are called
	 
	 This can be short circuited any time by a "true" return from a callback, which
	 will halt ALL the bubbling.  Return values are available if a callback is connected
	 with the full variant or default callback ONLY */
zval* wingui_winproc_callback_dispatch(wingui_window_object *window_object, int msg, zval ***extra,
	int extra_count, zend_bool *stop_default TSRMLS_DC)
{
	zval *return_value = NULL, ***param_list = NULL;
	HashTable **callbacks = NULL;
	int param_list_count = extra_count + 1;
	const char *callback_method;

	/* Attached callbacks are used first */
	if((zend_hash_num_elements(window_object->registered_callbacks) > 0) &&
		(zend_hash_index_find(window_object->registered_callbacks, msg, (HashTable **) &callbacks) == SUCCESS)) {
		HashPosition pos;
		wingui_callback_t **callback;

		/* iterate BACKWARD and call and optionally bail if bubble ends up as true */ 
		for(zend_hash_internal_pointer_end_ex(*callbacks, &pos);
			zend_hash_get_current_data_ex(*callbacks, (wingui_callback_t **) &callback, &pos) == SUCCESS;
			zend_hash_move_backwards_ex(*callbacks, &pos)) {

			/* in loop declarations */
			zend_fcall_info *finfo = &((*callback)->callback_info);
			zval ***orig_params = finfo->params, ***params = orig_params;
			int orig_params_count = finfo->param_count, param_count = orig_params_count;
			zval *bubble_value = NULL;

			finfo->retval_ptr_ptr = &bubble_value;

			/* The parameters list is only created/allocated on demand but is used for all callbacks */
			if (!param_list) {
				param_list = safe_emalloc(param_list_count, sizeof(zval**), 0);
				param_list[0] = emalloc(sizeof(zval*));
				ALLOC_INIT_ZVAL(*param_list[0]);
				Z_ISREF_P(*param_list[0]);
				memcpy(param_list + 1, extra, extra_count * sizeof(zval**));
			}

			/* start the crazy args manipulation
			send_args is true, send_return is true, orig_params_count is > 0 */
			if ((*callback)->send_args && (*callback)->send_return && orig_params_count) {
				/* Two sets of parameters to mix with param_list version */
				param_count = param_list_count + orig_params_count;
				params = safe_emalloc(param_count, sizeof(zval**), 0);

				memcpy(params, param_list, param_list_count * sizeof(zval**));
				memcpy(params + param_list_count, orig_params, orig_params_count * sizeof(zval**));

			/* send_args is true, send_return is true, orig_params_count is 0 */
			} else if((*callback)->send_args && (*callback)->send_return) {
				param_count = param_list_count;
				params = param_list;
			/* send_args is true, send_return is false, orig_params_count > 0 */
			} else if((*callback)->send_args && orig_params_count && extra_count) {
				/* Two sets of parameters to mix with extra version */
				param_count = extra_count + orig_params_count;
				params = safe_emalloc(param_count, sizeof(zval**), 0);

				memcpy(params, extra, extra_count * sizeof(zval**));
				memcpy(params + extra_count, orig_params, orig_params_count * sizeof(zval**));
			/* send_args is true, send return is false, orig_params_count is 0 */
			} else if((*callback)->send_args) {
				param_count = extra_count;
				params = extra;
			/* send_args is false, send_return is false, use the original params is default */
			}

			/* Place params where they go */
			finfo->params = params;
			finfo->param_count = param_count;

			if (SUCCESS == zend_call_function(finfo, &((**callback).callback_cache) TSRMLS_CC)) {
				/* returning true will break the callback chain */
				if(bubble_value && Z_TYPE_P(bubble_value) == IS_BOOL && Z_BVAL_P(bubble_value))
				{
					*stop_default = TRUE;
					finfo->params = orig_params;
					finfo->param_count = orig_params_count;
					if (param_list_count && orig_params_count) {
						efree(params);
					}
					if (bubble_value) {
						zval_ptr_dtor(&bubble_value);
					}
					break;
				}
			} else {
				/* If we ever hit this, things are very wrong in Kansas, blow up nicely */
				php_error(E_RECOVERABLE_ERROR, "Error calling %s for msg:%d", finfo->function_name, msg);
			}

			finfo->params = orig_params;
			finfo->param_count = orig_params_count;

			if (param_list_count && orig_params_count) {
				efree(params);
			}
			if (bubble_value) {
				zval_ptr_dtor(&bubble_value);
			}

		}
	}

	/* first, is the stop_default off and do we have an item in the callback map that matches a method in the class - if so final callback time */
	if(*stop_default == FALSE && (zend_hash_index_find(window_object->callback_map, msg, (void **) &callback_method) == SUCCESS) && 
		zend_hash_num_elements(&Z_OBJCE_P(window_object->object_zval)->function_table) > 0 &&
		zend_hash_exists(&Z_OBJCE_P(window_object->object_zval)->function_table, callback_method, strlen(callback_method) + 1)) {
		
		zval *function;
		zval *bubble_value = NULL;

		MAKE_STD_ZVAL(function);
		ZVAL_STRING(function, callback_method, 1);
		
		/* The parameters list is only created/allocated on demand but is used for all callbacks */
		if (!param_list) {
			param_list = safe_emalloc(param_list_count, sizeof(zval**), 0);
			param_list[0] = emalloc(sizeof(zval*));
			ALLOC_INIT_ZVAL(*param_list[0]);
			Z_ISREF_P(*param_list[0]);
			memcpy(param_list + 1, extra, extra_count * sizeof(zval**));
		}

		if (call_user_function_ex(
					&Z_OBJCE_P(window_object->object_zval)->function_table,
					&window_object->object_zval,
					function,
					&bubble_value,
					param_list_count,
					param_list,
					0,
					NULL TSRMLS_CC
					)  == FAILURE) {
				/* If we ever hit this, things are very wrong in Kansas, blow up nicely */
				php_error(E_RECOVERABLE_ERROR, "Error calling %s for msg:%d", callback_method, msg);
		}

		zval_ptr_dtor(&function);
		/* returning true will break the callback chain */
		if(bubble_value && Z_TYPE_P(bubble_value) == IS_BOOL && Z_BVAL_P(bubble_value)) {
			*stop_default = TRUE;
		}
		if (bubble_value) {
			zval_ptr_dtor(&bubble_value);
		}
	}

	if (param_list) {
		return_value = *param_list[0];
		efree(param_list);
	}

	return return_value;
}
/* }}} */

/* Helper function that asks "is this a statusbar window?
   This is a workaround for statusbars not getting a SIZE posted
   when a parent is resized and is used by the default window procedure */
static BOOL CALLBACK wingui_check_for_statusbar(HWND hwndChild, LPARAM lParam) 
{ 
	char buf[20] = {0};

	if (!GetClassName(hwndChild, buf, 20)) {
		return FALSE;
	}

	if (strcmp(buf, STATUSCLASSNAME) == 0) {
		PostMessage(hwndChild, WM_SIZE, (WPARAM) 0, (LPARAM) 0);
	}

	return TRUE;
}

/* This is the window procedure used for all window classes and controls
   dispatches messages and uses four types of stored callbacks to process data */
LRESULT CALLBACK wingui_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	zval ***extra = NULL;
	int extra_count = 0;
	zend_bool stop_default = FALSE;
	zval *return_value = NULL;
#ifdef ZTS
	TSRMLS_D;
#endif;

	// TODO: do not do this if this is WM_CREATE, instead get window_object out of the CREATESTRUCT
	wingui_window_object *window_object = (wingui_window_object *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
#ifdef ZTS
	TSRMLS_C = window_object->TSRMLS_C;
#endif

	/* This is ugly as sin.  This says take all the child windows, if they are a statusbar send them a WM_SIZE
	   Why? because the default proc is too stupid to do this */
	if(msg == WM_SIZE) {
		EnumChildWindows(hwnd, wingui_check_for_statusbar, (LPARAM) NULL);
	}

	extra = window_object->messages_cracker(msg, &wParam, &lParam, &extra_count TSRMLS_CC);

	if(window_object) {
		return_value = wingui_winproc_callback_dispatch(window_object, msg, extra, extra_count, &stop_default TSRMLS_CC);
	}
	
	if (stop_default == FALSE) {
		window_object->messages_packer(msg, &wParam, &lParam, extra TSRMLS_CC);
	}

	if (extra) {
		wingui_callback_extra_zvals_dtor(extra_count, extra);
	}

	/* return is based on if we need to call the default proc handling, and if we have an overwritten
	   return value to use instead of the default value */
	if (stop_default && return_value) {
		return window_object->messages_results(msg, return_value TSRMLS_CC);
	} else if (stop_default) {
		return (LRESULT) NULL;
	} else if (return_value) {
		CallWindowProc(window_object->window_proc, hwnd, msg, wParam, lParam);
		return window_object->messages_results(msg, return_value TSRMLS_CC);
	} else {
		return CallWindowProc(window_object->window_proc, hwnd, msg, wParam, lParam);
	}
}
/* }}} */












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

/* Generic option parser for "base" options for windows 
int wingui_window_object_parse_options(zval *options, int *x, int *y, int *height, int *width, char **text, int *style, int *extrastyle TSRMLS_DC)
{
	zval **value;

	if (zend_hash_find(Z_ARRVAL_P(options), "x", sizeof("x"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
		*x = Z_LVAL_PP(value);
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "y", sizeof("y"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
		*y = Z_LVAL_PP(value);
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "width", sizeof("width"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
		*width = Z_LVAL_PP(value);
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "height", sizeof("height"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
		*height = Z_LVAL_PP(value);
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "text", sizeof("text"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_STRING TSRMLS_CC);
		*text = Z_STRVAL_PP(value);
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "type", sizeof("type"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
		
	}

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
	}

	/* These are special settings that are only available through the constructor, not as properties because
	  they can be manipulated programatically after creating the window - they affect the "initial state" 
	if (zend_hash_find(Z_ARRVAL_P(options), "maximize", sizeof("maximize"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_MAXIMIZE;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "minimize", sizeof("minimize"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_MINIMIZE;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "visible", sizeof("visible"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_VISIBLE;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "disabled", sizeof("disabled"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*style |= WS_DISABLED;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "deactivated", sizeof("deactivated"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_NOACTIVATE;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "topmost", sizeof("topmost"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_LVAL_PP(value)) {
			*extrastyle |= WS_EX_TOPMOST;
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
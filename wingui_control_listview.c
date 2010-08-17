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

/* $Id$ */

#include "php_wingui.h"
#include "zend_exceptions.h"

ZEND_DECLARE_MODULE_GLOBALS(wingui);

/* All the classes in this file */
zend_class_entry *ce_wingui_control_listview;

/* Custom Object Handler Items */
HashTable wingui_control_listview_callback_map;
HashTable wingui_control_listview_prop_handlers;

/* C API used here */
zval ***wingui_control_listview_messages_cracker(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC);
void wingui_control_listview_messages_packer(int msg, WPARAM *wParam, LPARAM *lParam, zval ***extra TSRMLS_DC);
LRESULT wingui_control_listview_messages_results(int msg, zval *return_value TSRMLS_DC);
void wingui_control_listview_messages_zvals(int msg, LRESULT lresult, zval *return_value TSRMLS_DC);
int wingui_control_listview_messages_dispatch(INTERNAL_FUNCTION_PARAMETERS, int msg, WPARAM *wParam, LPARAM *lParam);

static int wingui_control_listview_array_to_item(zval *item, LVITEM *item_struct TSRMLS_DC);
static int wingui_control_listview_array_to_column(zval *column, LVCOLUMN *column_struct TSRMLS_DC);

/* ----------------------------------------------------------------
  Win\Gui\Control\ListView Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiControlListView_approximateRect_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, items)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlListView_arrange_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, snap_to_grid)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlListView_setImageList_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, imagelist)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlListView_insertItem_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, item)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO_EX(WinGuiControlListView_subItemHitTest_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 3)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, y)
	ZEND_ARG_INFO(0, item)
	ZEND_ARG_INFO(0, settings)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlListView_update_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

/* {{{ __construct */
PHP_METHOD(WinGuiControlListView, __construct)
{
	zend_error_handling error_handling;
	HWND hwnd, parent_handle = NULL;
	wingui_window_object *parent_obj;
	wingui_window_object *status_obj = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	int x = 0, y = 0, height = 0, width = 0, style = WS_CHILD, id = 0, extrastyle = 0;
	char * text = "";
	zval *parent, *options = NULL; 

	/* Parse options from method call - parent is required */
	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|a", &parent, ce_wingui_windowing, &options) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	Z_ADDREF_P(parent);
    status_obj->parent_is_temp = 1;
    parent_obj = (wingui_window_object *)zend_object_store_get_object(parent TSRMLS_CC);
    parent_handle = parent_obj->window_handle;

	hwnd = CreateWindowEx(
		extrastyle,
        WC_LISTVIEW, 
        text,   
        WS_CHILD | LVS_ICON | LVS_EDITLABELS, 
        x,  
        y,  
        500, 
        500,
        parent_handle,
        (HMENU) id,  
        WINGUI_G(hinstance),
        (LPVOID) NULL);

    if (!hwnd) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} else {
		status_obj->window_handle = hwnd;
		status_obj->object_zval = getThis();
		status_obj->window_proc = (WNDPROC) SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR) wingui_proc_handler);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) status_obj);
	}
}
/* }}} */

/* {{{ proto array Win\Gui\Control\ListView->approximateRect([int items])
	Calculates the approximate width and height required to display a given number of items
	If you do not send a number of items, it will calculate based on the number of items
	currently in the listview.  returns a numeric array of width and height  */
PHP_METHOD(WinGuiControlListView, approximateRect)
{
	zend_error_handling error_handling;
	long items = -1;
	DWORD value;
	wingui_window_object *listview_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &items) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	value = ListView_ApproximateViewRect(listview_obj->window_handle, -1, -1, items);
	array_init(return_value);
	add_next_index_long(return_value, LOWORD(value));
	add_next_index_long(return_value, HIWORD(value));
}
/* }}} */

/* {{{ proto array Win\Gui\Control\ListView->arrange([bool snap_to_grid])
	Arranges items in icon view  */
PHP_METHOD(WinGuiControlListView, arrange)
{
	zend_error_handling error_handling;
	zend_bool snap_to_grid = FALSE;
	UINT code = LVA_DEFAULT;
	wingui_window_object *listview_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &snap_to_grid) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (snap_to_grid) {
		code = LVA_SNAPTOGRID;
	}

	RETURN_BOOL(ListView_Arrange(listview_obj->window_handle, code));
}
/* }}} */

/* {{{ proto void Win\Gui\Control\ListView->insertColumn(int index, Win\Gui\Control\ListView\Column column)
	Sets a column in a specific location */
PHP_METHOD(WinGuiControlListView, insertColumn)
{
	zend_error_handling error_handling;
	long index = 0;
	zval *column;
	LVCOLUMN column_struct;
	wingui_window_object *listview_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "la", &index, &column) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	if (wingui_control_listview_array_to_column(column, &column_struct TSRMLS_CC) == SUCCESS) {
		RETURN_BOOL(ListView_InsertColumn(listview_obj->window_handle, index, &column_struct));
	}
}
/* }}} */

/* {{{ proto void Win\Gui\Control\ListView->insertItem(array item)
	inserts an item */
PHP_METHOD(WinGuiControlListView, insertItem)
{
	zend_error_handling error_handling;
	zval *item;
	LVITEM item_struct;
	wingui_window_object *listview_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &item) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (wingui_control_listview_array_to_item(item, &item_struct TSRMLS_CC) == SUCCESS) {
		if (item_struct.iSubItem != 0) {
			zend_throw_exception(ce_wingui_exception, "Win\\Gui\\Control\\ListView->insertItem() can only insert items into the first column, col value must be zero", 0 TSRMLS_CC);
			return;
		}
		RETURN_LONG(ListView_InsertItem(listview_obj->window_handle, &item_struct));
	}
}
/* }}} */

/* {{{ proto void Win\Gui\Control\ListView->setImageList(Win\Gui\ImageList list[, int type])
	inserts an item */
PHP_METHOD(WinGuiControlListView, setImageList)
{
	zend_error_handling error_handling;
	long type = LVSIL_NORMAL;
	zval *imagelist;
	wingui_imagelist_object *imagelist_object;
	HIMAGELIST prev_list;
	wingui_window_object *listview_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|l", &imagelist, ce_wingui_imagelist, &type) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	imagelist_object = (wingui_imagelist_object*)wingui_imagelist_object_get(imagelist TSRMLS_CC);
	switch(type) {
		case LVSIL_NORMAL:
		case LVSIL_SMALL:
		case LVSIL_STATE:
		case LVSIL_GROUPHEADER:
		break;
		default:
			zend_throw_exception(ce_wingui_argexception, "Type must be one of Win\\Gui\\Control\\ListView::NORMAL, Win\\Gui\\Control\\ListView::SMALL, Win\\Gui\\Control\\ListView::LARGE, or Win\\Gui\\Control\\ListView::GROUPHEADER", 0 TSRMLS_CC);
	}
	/* TODO: check if we have an imagelist/type zval stored and delref it if we do?
	if (menu->uncheckbitmap_object) {
		Z_DELREF_P(menu->uncheckbitmap_object);
	} */

	prev_list = ListView_SetImageList(listview_object->window_handle, imagelist_object->imagelist, type);
	
	/* TODO addref and store new imagelist 
	listview_obj->data.imagelist = imagelist;
	Z_ADDREF_P(imagelist); and return old imagelist object
	*/
}
/* }}} */

/* {{{ proto void Win\Gui\Control\ListView->setItem(int index, Win\Gui\Control\ListView\Item item)
	inserts an item */
PHP_METHOD(WinGuiControlListView, setItem)
{
	zend_error_handling error_handling;
	zval *item;
	LVITEM item_struct;
	wingui_window_object *listview_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &item) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (wingui_control_listview_array_to_item(item, &item_struct TSRMLS_CC) == SUCCESS) {
		RETURN_LONG(ListView_SetItem(listview_obj->window_handle, &item_struct));
	}
}
/* }}} */
/*
BOOL ListView_SortItems(

    HWND hwnd,
    PFNLVCOMPARE pfnCompare,
    LPARAM lParamSort
);
ListView_SortItemsEx(*/

/* {{{ proto void Win\Gui\Control\ListView->sortItemsById(mixed callback[, mixed arg1, mixed arg2])
	Uses an application-defined comparison function to sort the items of a list-view control. The index of each item changes to reflect the new sequence
	This differs from sortItems because the callback receives the index id of the items, instead of the
	item value */
int CALLBACK wingui_sort_items_ex(LPARAM index1, LPARAM index2, LPARAM data){
	//zval *return_value = NULL;
	//int value;
	//wingui_callback_t *store = (wingui_callback_t *)data;
	//zend_fcall_info *finfo = &(store->callback_info);
	
	// TODO fix this so we get index1 and index 2 and pop the suckers on the stack
	//finfo->retval_ptr_ptr = &return_value;
	//finfo->params =	store->callback_info.params;
	//finfo->param_count = store->callback_info.param_count;

	//if (FAILURE == zend_call_function(finfo, &(store->callback_cache) TSRMLS_CC)) {
	//	php_error(E_RECOVERABLE_ERROR, "Error calling %s for Win\\Gui\\Control\\ListView->sortItemsById()", finfo->function_name);
	//}

	//if (bubble_value) {
	//	zval_ptr_dtor(&bubble_value);
	//}
}
PHP_METHOD(WinGuiControlListView, sortItemsById)
{
	zend_error_handling error_handling;
	wingui_callback_t *store;
	zend_fcall_info finfo;
	zend_fcall_info_cache fcache;
	zval ***args = NULL;
	int argc = 0, i;
	wingui_window_object *listview_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f*", &finfo, &fcache, &args, &argc) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* walk the arrrrgs and addref em - weee! - I need a spot to free this or they'll leak like hell! */
	if (argc) {
		for(i = 0; i < argc; i++) {
				zval **newppzval = emalloc(sizeof(zval*));
				MAKE_STD_ZVAL(*newppzval);
				ZVAL_ZVAL(*newppzval, *args[i], 1, 0);
				Z_SET_REFCOUNT_PP(newppzval, 2);
				args[i] = newppzval;
		}
	}
	store = emalloc(sizeof(wingui_callback_t));

	store->callback_info = finfo;
	if (store->callback_info.object_ptr) {
			zval_add_ref(&(store->callback_info.object_ptr));
	}
	zval_add_ref(&(store->callback_info.function_name));
	store->callback_cache = fcache;

	store->src_filename = estrdup(zend_get_executed_filename(TSRMLS_C));
	store->src_lineno = zend_get_executed_lineno(TSRMLS_C);
	store->callback_info.params      = args;
	store->callback_info.param_count = argc;

	RETURN_BOOL(ListView_SortItemsEx(listview_obj->window_handle, wingui_sort_items_ex, (LPARAM) store));
}
/* }}} */

/* {{{ proto void Win\Gui\Control\ListView->subItemHitTest(int x, int y, int item[, array settings])
	Updates a list-view item */
PHP_METHOD(WinGuiControlListView, subItemHitTest)
{
	zend_error_handling error_handling;
	long x, y, item;
	int location;
	zval *settings = NULL, **value;
	LVHITTESTINFO hittestinfo;
	wingui_window_object *listview_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll|a", &x, &y, &item, &settings) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	hittestinfo.pt.x = x;
	hittestinfo.pt.y = y;
	hittestinfo.iItem = item;
	hittestinfo.flags = 0;

	if (settings && Z_TYPE_P(settings) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(settings), "above", sizeof("above"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_ABOVE;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "below", sizeof("below"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_BELOW;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "nowhere", sizeof("nowhere"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_NOWHERE;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "icon", sizeof("icon"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_ONITEMICON;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "text", sizeof("text"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_ONITEMLABEL;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "state", sizeof("state"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_ONITEMSTATEICON;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "left", sizeof("left"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_TOLEFT;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "right", sizeof("right"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_TORIGHT;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "groupheader", sizeof("groupheader"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_EX_GROUP_HEADER;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "groupfooter", sizeof("groupfooter"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_EX_GROUP_FOOTER;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "groupcollapse", sizeof("groupcollapse"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_EX_GROUP_COLLAPSE;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "groupbackground", sizeof("groupbackground"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_EX_GROUP_BACKGROUND;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "groupicon", sizeof("groupicon"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_EX_GROUP_STATEICON;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "grouplink", sizeof("grouplink"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_EX_GROUP_SUBSETLINK;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "group", sizeof("group"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_EX_GROUP;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "contents", sizeof("contents"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_EX_ONCONTENTS;
		}
		if (zend_hash_find(Z_ARRVAL_P(settings), "footer", sizeof("footer"), (void **) &value) == SUCCESS) {
			hittestinfo.flags |= LVHT_EX_FOOTER;
		}
	}

	if (wingui_is_vista(TSRMLS_C)) {
		location = ListView_SubItemHitTest(listview_obj->window_handle, &hittestinfo);
	} else {
		location = ListView_SubItemHitTestEx(listview_obj->window_handle, &hittestinfo);
	}

	if (location == -1) {
		RETURN_FALSE
	}
	array_init(return_value);
	add_assoc_long(return_value, "item", hittestinfo.iItem);
	add_assoc_long(return_value, "subitem", hittestinfo.iSubItem);
	add_assoc_long(return_value, "group", hittestinfo.iGroup);

}
/* }}} */

/* {{{ proto void Win\Gui\Control\ListView->update(int index)
	Updates a list-view item */
PHP_METHOD(WinGuiControlListView, update)
{
	zend_error_handling error_handling;
	long index;
	wingui_window_object *listview_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(ListView_Update(listview_obj->window_handle, index));
}
/* }}} */

/* register listview methods */
static zend_function_entry wingui_control_listview_functions[] = {
	PHP_ME(WinGuiControlListView, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiControlListView, approximateRect, WinGuiControlListView_approximateRect_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiControlListView, arrange, WinGuiControlListView_arrange_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiControlListView, insertColumn, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiControlListView, insertItem, WinGuiControlListView_insertItem_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiControlListView, setImageList, WinGuiControlListView_setImageList_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiControlListView, setItem, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiControlListView, subItemHitTest, WinGuiControlListView_subItemHitTest_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiControlListView, update, WinGuiControlListView_update_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Control\ListView Custom Object magic                                                 
------------------------------------------------------------------*/
/* {{{ window object with control/listview specific internal stuff */
zend_object_value wingui_control_listview_object_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_window_object *window_object;
	zval *tmp;

	window_object = emalloc(sizeof(wingui_window_object));
	window_object->std.ce = ce;
	window_object->std.guards = NULL;
	window_object->parent_is_temp = 0;
	window_object->window_handle = NULL;
	window_object->prop_handler = &wingui_control_listview_prop_handlers;
	window_object->callback_map = &wingui_control_listview_callback_map;
	window_object->window_proc = NULL;
	window_object->messages_results = wingui_control_listview_messages_results;
	window_object->messages_cracker = wingui_control_listview_messages_cracker;
	window_object->messages_packer = wingui_control_listview_messages_packer;
	window_object->messages_zvals = wingui_control_listview_messages_zvals;
	window_object->messages_dispatch = wingui_control_listview_messages_dispatch;
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
zval ***wingui_control_listview_messages_cracker(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC)
{
	zval ***extra = NULL;

	switch(msg) {
		default:
			return extra;
			//return wingui_window_messages_cracker(msg, wParam,lParam, extra_count TSRMLS_CC);
	}
}
/* }}} */

/* {{{ put cracked values into lparams, wparams before calling defaultproc */
void wingui_control_listview_messages_packer(int msg, WPARAM *wParam, LPARAM *lParam, zval ***extra TSRMLS_DC)
{
	switch(msg) {
	}
}
/* }}} */

/* {{{ put zval return values from from messages into lresult vals */
LRESULT wingui_control_listview_messages_results(int msg, zval *return_value TSRMLS_DC)
{
	switch(msg) {
		default:
			//return wingui_window_messages_results(msg, return_value TSRMLS_CC);
			return (LRESULT) NULL;
	}
}
/* }}} */

/* {{{ put lresult vals into zval return values */
void wingui_control_listview_messages_zvals(int msg, LRESULT lresult, zval *return_value TSRMLS_DC)
{
	switch(msg) {
		default:
			//return wingui_window_messages_zvals(msg, return_value TSRMLS_CC);
			return;
	}
}
/* }}} */

/* {{{ parse args from send/post/etc and shove into lparam and wparams */
int wingui_control_listview_messages_dispatch(INTERNAL_FUNCTION_PARAMETERS, int item, WPARAM *wParam, LPARAM *lParam)
{
	zend_error_handling error_handling;
	int retval = SUCCESS;
	/* window handle */
	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	switch(item) {
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	return retval;
}
/* }}} */

/* Translates a PHP assoc array into information for an LVITEM struct */
static int wingui_control_listview_array_to_item(zval *item, LVITEM *item_struct TSRMLS_DC)
{
	zval **value;
	memset(item_struct, 0, sizeof(LVITEM));
	//LVITEM struct_data;

	if (zend_hash_find(Z_ARRVAL_P(item), "text", sizeof("text"), (void **) &value) == SUCCESS) {
		if (wingui_juggle_type(*value, IS_STRING TSRMLS_CC) == SUCCESS) {
			item_struct->pszText = estrdup(Z_STRVAL_PP(value));
			item_struct->mask |= LVIF_TEXT;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(item), "col", sizeof("col"), (void **) &value) == SUCCESS) {
		if (wingui_juggle_type(*value, IS_LONG TSRMLS_CC) == SUCCESS) {
			item_struct->iSubItem = Z_LVAL_PP(value);
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(item), "row", sizeof("row"), (void **) &value) == SUCCESS) {
		if (wingui_juggle_type(*value, IS_LONG TSRMLS_CC) == SUCCESS) {
			item_struct->iItem = Z_LVAL_PP(value);
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(item), "image", sizeof("image"), (void **) &value) == SUCCESS) {
		if (wingui_juggle_type(*value, IS_LONG TSRMLS_CC) == SUCCESS) {
			item_struct->iImage = Z_LVAL_PP(value);
			item_struct->mask |= LVIF_IMAGE;
		}
	}
	return SUCCESS;
}
/* }}} */

/* Translates a PHP assoc array into information for an LVCOLUMN struct */
static int wingui_control_listview_array_to_column(zval *column, LVCOLUMN *column_struct TSRMLS_DC)
{
	zval **value;
	memset(column_struct, 0, sizeof(LVCOLUMN));
	//LVITEM struct_data;

	if (zend_hash_find(Z_ARRVAL_P(column), "text", sizeof("text"), (void **) &value) == SUCCESS) {
		if (wingui_juggle_type(*value, IS_STRING TSRMLS_CC) == SUCCESS) {
			column_struct->pszText = estrdup(Z_STRVAL_PP(value));
			column_struct->mask |= LVCF_TEXT;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(column), "width", sizeof("width"), (void **) &value) == SUCCESS) {
		if (wingui_juggle_type(*value, IS_LONG TSRMLS_CC) == SUCCESS) {
			column_struct->cx = Z_LVAL_PP(value);
			column_struct->mask |= LVCF_WIDTH;	
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(column), "leftalign", sizeof("leftalign"), (void **) &value) == SUCCESS) {
		column_struct->fmt = LVCFMT_LEFT;
		column_struct->mask |= LVCF_FMT;
	}
	return SUCCESS;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Control\ListView LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_control_listview)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS_NAME("Control", "ListView"), wingui_control_listview_functions);
	ce_wingui_control_listview = zend_register_internal_class_ex(&ce, ce_wingui_control, ZEND_NS_NAME(PHP_WINGUI_NS, "Control") TSRMLS_CC);
	ce_wingui_control_listview->create_object  = wingui_control_listview_object_new;

	/* property handlers for "fake" properties - control styles */
	zend_hash_init(&wingui_control_listview_prop_handlers, 0, NULL, NULL, 1);
	//wingui_register_prop_handler(&wingui_control_statusbar_prop_handlers, ce_wingui_control_statusbar, "tooltips", wingui_statusbar_read_style, wingui_statusbar_write_style TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_control_statusbar_prop_handlers, ce_wingui_control_statusbar, "sizegrip", wingui_statusbar_read_style, wingui_statusbar_write_style TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_control_statusbar_prop_handlers, ce_wingui_control_statusbar, "id", wingui_statusbar_read_id, wingui_statusbar_write_id TSRMLS_CC);
	//zend_hash_merge(&wingui_control_statusbar_prop_handlers, &wingui_control_prop_handlers, NULL, NULL, sizeof(wingui_prop_handler), 0);

	/* Statusbar notifications hook up to */
	zend_hash_init(&wingui_control_listview_callback_map, 0, NULL, NULL, 1);
	//REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, NM_CLICK,				&wingui_control_statusbar_callback_map, onnmclick);
	//REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, NM_DBLCLK,			&wingui_control_statusbar_callback_map, onnmdblclck);
	//REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, NM_RCLICK,			&wingui_control_statusbar_callback_map, onnmrclick);
	//REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, NM_RDBLCLK,			&wingui_control_statusbar_callback_map, onnmrdblclk);
	//REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SBN_SIMPLEMODECHANGE,	&wingui_control_statusbar_callback_map, onsbnsimplemodechange);
	/* Statusbar messages to hook up to */
	//REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETBORDERS,		&wingui_control_statusbar_callback_map, onsbgetborders);
	//REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETICON,			&wingui_control_statusbar_callback_map, onsbgeticon);
	//REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETPARTS,			&wingui_control_statusbar_callback_map, onsbgetparts);
	/* copy in all the control ones 
	zend_hash_merge(&wingui_statusbar_callback_map, &wingui_control_callback_map, NULL, NULL, sizeof(char *), 0);*/
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION(wingui_control_listview) */
PHP_MSHUTDOWN_FUNCTION(wingui_control_listview)
{
	zend_hash_destroy(&wingui_control_listview_prop_handlers);
	zend_hash_destroy(&wingui_control_listview_callback_map);
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
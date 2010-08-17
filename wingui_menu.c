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
zend_class_entry *ce_wingui_menu;
HashTable wingui_menu_prop_handlers;

/* ----------------------------------------------------------------
  Win\Gui\Menu Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiMenu___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, vertical)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMenu_insert_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, position)
	ZEND_ARG_OBJ_INFO(0, menuitem, Win\\Gui\\MenuItem, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMenu_append_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_OBJ_INFO(0, menuitem, Win\\Gui\\MenuItem, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Win\Gui\Menu->__construct([bool vertical])
       Creates a new menu */
PHP_METHOD(WinGuiMenu, __construct)
{
	zend_error_handling error_handling;
	zend_bool vertical = 0;
	HMENU hmenu;
	wingui_menu_object *menu_object = (wingui_menu_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b",  &vertical) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (vertical) {
		hmenu = CreatePopupMenu();
	} else {
		hmenu = CreateMenu();
	}

	if (hmenu == NULL) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}
	
	menu_object->menu_handle = hmenu;
}
/* }}} */

/* {{{ proto bool Win\Gui\Menu->insert(int position, Win\Gui\MenuItem menuitem)
       inserts a menu item at a specific position (0 indexed) */
PHP_METHOD(WinGuiMenu, insert)
{
	zend_error_handling error_handling;
	int position = 0;
	zval *menuitem;
	//wingui_menuitem_object *menuitem_object;
	wingui_menu_object *menu_object = (wingui_menu_object*)wingui_menu_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	//if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lO", &position, &menuitem, ce_wingui_menuitem) == FAILURE) {
	//	return;
	//}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	//menuitem_object = (wingui_menuitem_object*)wingui_menuitem_object_get(menuitem TSRMLS_CC);

	//RETVAL_BOOL(InsertMenuItem(menu_object->menu_handle, position, TRUE, (LPCMENUITEMINFO) menuitem_object->menuiteminfo));

	//if (menu_object->window_handle) {
	//	DrawMenuBar(menu_object->window_handle);
	//}
}
/* }}} */

/* {{{ proto bool Win\Gui\Menu->append(Win\Gui\MenuItem menuitem)
       appends a menu item to a menu */
PHP_METHOD(WinGuiMenu, append)
{
	/*
	zend_error_handling error_handling;
	zval *menuitem;
	wingui_menuitem_object *menuitem_object;
	wingui_menu_object *menu_object = (wingui_menu_object*)wingui_menu_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &menuitem, ce_wingui_menuitem) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	menuitem_object = (wingui_menuitem_object*)wingui_menuitem_object_get(menuitem TSRMLS_CC);
	menuitem_object->parent_menu = menu_object->menu_handle;
	RETVAL_BOOL(InsertMenuItem(menu_object->menu_handle, 0, FALSE, (LPCMENUITEMINFO) menuitem_object->menuiteminfo));
	if (menu_object->window_handle) {
		DrawMenuBar(menu_object->window_handle);
	} */
}
/* }}} */

/* register Menu methods */
static zend_function_entry wingui_menu_functions[] = {
	PHP_ME(WinGuiMenu, __construct, WinGuiMenu___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiMenu, insert, WinGuiMenu_insert_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMenu, append, WinGuiMenu_append_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win::Gui::Menu Custom Object magic                                                 
------------------------------------------------------------------*/
/* cleans up the object */
void wingui_menu_destructor(void *object TSRMLS_DC)
{
	wingui_menu_object *menu_object = (wingui_menu_object *)object;

	zend_hash_destroy(menu_object->std.properties);
	FREE_HASHTABLE(menu_object->std.properties);
	
	if(menu_object->menu_handle){
		DestroyMenu(menu_object->menu_handle);
	}
	efree(object);
}
/* }}} */

/* {{{ wingui_menu_object_new
       creates a new menu object */
zend_object_value wingui_menu_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_menu_object *menu_object;
	zval *tmp;

	menu_object = ecalloc(1, sizeof(wingui_menu_object));
	menu_object->std.ce = ce;
	menu_object->std.guards = NULL;
	menu_object->prop_handler = &wingui_menu_prop_handlers;
	menu_object->menu_handle = NULL;

	ALLOC_HASHTABLE(menu_object->std.properties);
	zend_hash_init(menu_object->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(menu_object->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(menu_object, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) wingui_menu_destructor, NULL TSRMLS_CC);
	menu_object->handle = retval.handle;
	retval.handlers = &wingui_object_handlers;
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Menu LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_menu)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Menu", wingui_menu_functions);
	ce_wingui_menu = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_menu->create_object  = wingui_menu_new;

	/* Callback map for properties */
	//zend_hash_init(&wingui_menu_prop_handlers, 0, NULL, NULL, 1);

	return SUCCESS;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
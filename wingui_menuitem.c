/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2009 The PHP Group                                |
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
zend_class_entry *ce_wingui_menuitem;
HashTable wingui_menuitem_prop_handlers;

/* ----------------------------------------------------------------
  Win\Gui\MenuItem Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiMenuItem___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
	ZEND_ARG_INFO(0, id)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\MenuItem->__construct(text, id)
     Create a new Menu */
PHP_METHOD(WinGuiMenuItem, __construct)
{
	zend_error_handling error_handling;
	char * text = NULL;
	int id, text_len;
	zval *menu = NULL, *bitmap = NULL;
	wingui_menu_object *submenu_object;
	wingui_menuitem_object *menu_object = (wingui_menuitem_object*)zend_objects_get_address(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	//if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|O!O",  &id, &text, &text_len, &bitmap, ce_wingui_resource_bitmap, &menu, ce_wingui_menu) == FAILURE) {
	//	return;
	//}
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|O",  &id, &text, &text_len, &menu, ce_wingui_menu) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	
	menu_object->menuiteminfo = ecalloc(1, sizeof(MENUITEMINFO));
	menu_object->menuiteminfo->cbSize = sizeof(MENUITEMINFO);
	menu_object->menuiteminfo->fType |= MFT_STRING;
	menu_object->menuiteminfo->dwTypeData = estrdup(text);
	menu_object->menuiteminfo->cch = text_len;
	menu_object->menuiteminfo->wID = id;
	menu_object->menuiteminfo->fMask = MIIM_STATE | MIIM_STRING | MIIM_FTYPE | MIIM_ID | MIIM_SUBMENU | MIIM_BITMAP | MIIM_CHECKMARKS | MIIM_DATA;

	/*if (bitmap) {
		wingui_resource_object *bitmap_object = (wingui_resource_object *)wingui_resource_object_get(bitmap TSRMLS_CC);
		menu_object->menuiteminfo->hbmpItem = bitmap_object->resource_handle;
		Z_ADDREF_P(bitmap);
		menu_object->bitmap_object = bitmap;
	}*/
	if (menu) {
		submenu_object = (wingui_menu_object*)zend_object_store_get_object(menu TSRMLS_CC);
		menu_object->menuiteminfo->hSubMenu = submenu_object->menu_handle;
	}
}
/* }}} */

/* {{{ proto void Win\Gui\MenuItem::getCheckmarkSize()
     retrieve default height and width for checkmark bitmaps for a menuitem */
PHP_METHOD(WinGuiMenuItem, getCheckmarkSize)
{
	zend_error_handling error_handling;
	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	array_init(return_value);
	add_next_index_long(return_value, GetSystemMetrics(SM_CXMENUCHECK));
	add_next_index_long(return_value, GetSystemMetrics(SM_CYMENUCHECK));
}
/* }}} */

/* register Menu methods */
static zend_function_entry wingui_menuitem_functions[] = {
	PHP_ME(WinGuiMenuItem, __construct, WinGuiMenuItem___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiMenuItem, getCheckmarkSize, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\MenuItem Custom Object magic                                                 
------------------------------------------------------------------*/

/* cleans up the object */
void wingui_menuitem_destructor(void *object TSRMLS_DC)
{
	wingui_menuitem_object *menu_object = (wingui_menuitem_object *)object;

	if(menu_object->bitmap_object) {
		Z_DELREF_P(menu_object->bitmap_object);
	}

	zend_hash_destroy(menu_object->std.properties);
	FREE_HASHTABLE(menu_object->std.properties);

	if(menu_object->menuiteminfo) {
		efree(menu_object->menuiteminfo);
		menu_object->menuiteminfo = NULL;
	}
	
	efree(object);
}

/* {{{ wingui_menuitem_object_new
       creates a new menuitem object */
zend_object_value wingui_menuitem_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_menuitem_object *menuitem_object;
	zval *tmp;

	menuitem_object = ecalloc(1, sizeof(wingui_menuitem_object));
	menuitem_object->std.ce = ce;
	menuitem_object->std.guards = NULL;
	menuitem_object->prop_handler = &wingui_menuitem_prop_handlers;
	menuitem_object->menuiteminfo = NULL;
	menuitem_object->parent_menu = NULL;
	menuitem_object->bitmap_object = NULL;
	menuitem_object->checkbitmap_object = NULL;
	menuitem_object->uncheckbitmap_object = NULL;

	ALLOC_HASHTABLE(menuitem_object->std.properties);
	zend_hash_init(menuitem_object->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(menuitem_object->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(menuitem_object, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) wingui_menuitem_destructor, NULL TSRMLS_CC);
	menuitem_object->handle = retval.handle;
	retval.handlers = &wingui_object_handlers;
	return retval;
}
/* }}} */

/* changes the struct pointer and modifies the item */
int wingui_menuitem_write_struct(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	wingui_menuitem_object *menu = wingui_menuitem_object_juggle(object);
	int oldid = menu->menuiteminfo->wID;

	if(strcmp(Z_STRVAL_P(member), "id") == 0) {
		if (wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
			menu->menuiteminfo->wID = Z_LVAL_P(value);
		}
	} else if(strcmp(Z_STRVAL_P(member), "text") == 0) {
		if (wingui_juggle_type(value, IS_STRING TSRMLS_CC) == SUCCESS) {
			menu->menuiteminfo->dwTypeData = estrdup(Z_STRVAL_P(value));
			menu->menuiteminfo->cch = Z_STRLEN_P(value);
		}
	} else if(strcmp(Z_STRVAL_P(member), "enabled") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				menu->menuiteminfo->fState &= ~MFS_DISABLED;
			} else {
				menu->menuiteminfo->fState |= MFS_DISABLED;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "hilite") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				menu->menuiteminfo->fState |= MFS_HILITE;
			} else {
				menu->menuiteminfo->fState &= ~MFS_HILITE;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "checked") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				menu->menuiteminfo->fState |= MFS_CHECKED;
			} else {
				menu->menuiteminfo->fState &= ~MFS_CHECKED;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "default") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				menu->menuiteminfo->fState |= MFS_DEFAULT;
			} else {
				menu->menuiteminfo->fState &= ~MFS_DEFAULT;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "radio") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				menu->menuiteminfo->fType |= MFT_RADIOCHECK;
			} else {
				menu->menuiteminfo->fType &= ~MFT_RADIOCHECK;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "image") == 0) {
		/*
		if(Z_TYPE_P(value) == IS_NULL) {
			if (menu->bitmap_object) {
				Z_DELREF_P(menu->bitmap_object);
			}
			menu->bitmap_object = NULL;
			menu->menuiteminfo->hbmpItem = NULL; 
		} else if(Z_TYPE_P(value) == IS_OBJECT &&
			instanceof_function(Z_OBJCE_P(value), ce_wingui_resource_bitmap TSRMLS_CC)) {
			wingui_resource_object *bitmap_object = (wingui_resource_object *)wingui_resource_object_get(value TSRMLS_CC);
			if (menu->bitmap_object) {
				Z_DELREF_P(menu->bitmap_object);
			}
			menu->menuiteminfo->hbmpItem = bitmap_object->resource_handle;
			Z_ADDREF_P(value);
			menu->bitmap_object = value;
		}*/
	} else if(strcmp(Z_STRVAL_P(member), "checkbitmap") == 0) {
		/*
		if(Z_TYPE_P(value) == IS_NULL) {
			if (menu->checkbitmap_object) {
				Z_DELREF_P(menu->checkbitmap_object);
			}
			menu->checkbitmap_object = NULL;
			menu->menuiteminfo->hbmpChecked = NULL;
		} else if(Z_TYPE_P(value) == IS_OBJECT &&
			instanceof_function(Z_OBJCE_P(value), ce_wingui_resource_bitmap TSRMLS_CC)) {
			wingui_resource_object *bitmap_object = (wingui_resource_object *)wingui_resource_object_get(value TSRMLS_CC);
			if (menu->checkbitmap_object) {
				Z_DELREF_P(menu->checkbitmap_object);
			}
			menu->menuiteminfo->hbmpChecked = bitmap_object->resource_handle;
			Z_ADDREF_P(value);
			menu->checkbitmap_object = value; 
		}
	} else if(strcmp(Z_STRVAL_P(member), "uncheckbitmap") == 0) {
		if(Z_TYPE_P(value) == IS_NULL) {
			if (menu->uncheckbitmap_object) {
				Z_DELREF_P(menu->uncheckbitmap_object);
			}
			menu->uncheckbitmap_object = NULL;
			menu->menuiteminfo->hbmpUnchecked = NULL;
		} else if(Z_TYPE_P(value) == IS_OBJECT &&
			instanceof_function(Z_OBJCE_P(value), ce_wingui_resource_bitmap TSRMLS_CC)) {
			wingui_resource_object *bitmap_object = (wingui_resource_object *)wingui_resource_object_get(value TSRMLS_CC);
			if (menu->uncheckbitmap_object) {
				Z_DELREF_P(menu->uncheckbitmap_object);
			}
			menu->menuiteminfo->hbmpUnchecked = bitmap_object->resource_handle;
			Z_ADDREF_P(value);
			menu->uncheckbitmap_object = value;
		}*/
	}
	
	if (menu->parent_menu) {
		SetMenuItemInfo(menu->parent_menu, oldid, FALSE, menu->menuiteminfo);
		//DrawMenuBar(menu->parent_menu);
	}
	return SUCCESS;
}
/* }}} */

/* Grabs values out of struct pointer - misses the whole C call to fill the struct */
int wingui_menuitem_read_struct(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	wingui_menuitem_object *menu = wingui_menuitem_object_juggle(object);

	/* more complicated versions */
	if (strcmp(Z_STRVAL_P(member), "id") == 0) {
		ZVAL_LONG(*retval, menu->menuiteminfo->wID);
		return SUCCESS;
	} else if(strcmp(Z_STRVAL_P(member), "text") == 0) {
		ZVAL_STRINGL(*retval, (char *) menu->menuiteminfo->dwTypeData, menu->menuiteminfo->cch, 1);
		return SUCCESS;
	} else if(strcmp(Z_STRVAL_P(member), "enabled") == 0) {
		if(menu->menuiteminfo->fState & MFS_DISABLED) {
			ZVAL_BOOL(*retval, FALSE);
		} else {
			ZVAL_BOOL(*retval, TRUE);
		}
		return SUCCESS;
	} else if(strcmp(Z_STRVAL_P(member), "hilite") == 0) {
		if(menu->menuiteminfo->fState & MFS_HILITE) {
			ZVAL_BOOL(*retval, TRUE);
		} else {
			ZVAL_BOOL(*retval, FALSE);
		}
		return SUCCESS;
	} else if(strcmp(Z_STRVAL_P(member), "checked") == 0) {
		if(menu->menuiteminfo->fState & MFS_CHECKED) {
			ZVAL_BOOL(*retval, TRUE);
		} else {
			ZVAL_BOOL(*retval, FALSE);
		}
		return SUCCESS;
	} else if(strcmp(Z_STRVAL_P(member), "default") == 0) {
		if(menu->menuiteminfo->fState & MFS_DEFAULT) {
			ZVAL_BOOL(*retval, TRUE);
		} else {
			ZVAL_BOOL(*retval, FALSE);
		}
		return SUCCESS;
	} else if(strcmp(Z_STRVAL_P(member), "radio") == 0) {
		if(menu->menuiteminfo->fType & MFT_RADIOCHECK) {
			ZVAL_BOOL(*retval, TRUE);
		} else {
			ZVAL_BOOL(*retval, FALSE);
		}
		return SUCCESS;
	} else if(strcmp(Z_STRVAL_P(member), "image") == 0) {
		if (menu->bitmap_object) {
			ZVAL_ZVAL(*retval, menu->bitmap_object, 1, NULL);
		} else {
			ZVAL_NULL(*retval);
		}
		return SUCCESS;
	} else if(strcmp(Z_STRVAL_P(member), "checkbitmap") == 0) {
		if (menu->checkbitmap_object) {
			ZVAL_ZVAL(*retval, menu->checkbitmap_object, 1, NULL);
		} else {
			ZVAL_NULL(*retval);
		}
		return SUCCESS;
	} else if(strcmp(Z_STRVAL_P(member), "uncheckbitmap") == 0) {
		if (menu->uncheckbitmap_object) {
			ZVAL_ZVAL(*retval, menu->uncheckbitmap_object, 1, NULL);
		} else {
			ZVAL_NULL(*retval);
		}
		return SUCCESS;
	}
	return FAILURE;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Menu LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_menuitem)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "MenuItem", wingui_menuitem_functions);
	ce_wingui_menuitem = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_menuitem->create_object  = wingui_menuitem_new;

	/* Callback map for properties */
	zend_hash_init(&wingui_menuitem_prop_handlers, 0, NULL, NULL, 1);
	wingui_register_prop_handler(&wingui_menuitem_prop_handlers, ce_wingui_menuitem, "id", wingui_menuitem_read_struct, wingui_menuitem_write_struct TSRMLS_CC);
	wingui_register_prop_handler(&wingui_menuitem_prop_handlers, ce_wingui_menuitem, "text", wingui_menuitem_read_struct, wingui_menuitem_write_struct TSRMLS_CC);
	wingui_register_prop_handler(&wingui_menuitem_prop_handlers, ce_wingui_menuitem, "enabled", wingui_menuitem_read_struct, wingui_menuitem_write_struct TSRMLS_CC);
	wingui_register_prop_handler(&wingui_menuitem_prop_handlers, ce_wingui_menuitem, "hilite", wingui_menuitem_read_struct, wingui_menuitem_write_struct TSRMLS_CC);
	wingui_register_prop_handler(&wingui_menuitem_prop_handlers, ce_wingui_menuitem, "checked", wingui_menuitem_read_struct, wingui_menuitem_write_struct TSRMLS_CC);
	wingui_register_prop_handler(&wingui_menuitem_prop_handlers, ce_wingui_menuitem, "default", wingui_menuitem_read_struct, wingui_menuitem_write_struct TSRMLS_CC);
	wingui_register_prop_handler(&wingui_menuitem_prop_handlers, ce_wingui_menuitem, "radio", wingui_menuitem_read_struct, wingui_menuitem_write_struct TSRMLS_CC);
	wingui_register_prop_handler(&wingui_menuitem_prop_handlers, ce_wingui_menuitem, "image", wingui_menuitem_read_struct, wingui_menuitem_write_struct TSRMLS_CC);

	// TODO: 3 types of menuitems, image, separator, and normal
	// TODO: submenu (store the popup menu attached)
	// TODO: break and barbreak properties
	// TODO: right justify and rightorder

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION(wingui_menuitem)
       frees the property handler hash */
PHP_MSHUTDOWN_FUNCTION(wingui_menuitem)
{
	zend_hash_destroy(&wingui_menuitem_prop_handlers);
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
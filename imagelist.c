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

/* $Id$ */

#include "php_wingui.h"
#include "zend_exceptions.h"

ZEND_DECLARE_MODULE_GLOBALS(wingui);

/* All the classes in this file */
zend_class_entry *ce_wingui_imagelist;

/* ----------------------------------------------------------------
  Win\Gui\ImageList Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiImageList___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 4)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, y)
	ZEND_ARG_INFO(0, size)
	ZEND_ARG_INFO(0, max_size)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiImageList_add_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, bitmap)
	ZEND_ARG_INFO(0, mask)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiImageList_addMasked_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
	ZEND_ARG_INFO(0, bitmap)
	ZEND_ARG_INFO(0, color_or_red)
	ZEND_ARG_INFO(0, green)
	ZEND_ARG_INFO(0, blue)
ZEND_END_ARG_INFO()

/* {{{ proto bool Win\Gui\ImageList->__construct(x, y)
       Creates a new imagelist */
PHP_METHOD(WinGuiImageList, __construct)
{
	zend_error_handling error_handling;
	HIMAGELIST imagelist;
	long x, y, flags = 0, initial, maxsize;
	zval *options = NULL, **value;
	wingui_imagelist_object *imagelist_object = (wingui_imagelist_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll|a",  &x, &y, &initial, &maxsize, &options) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	maxsize = MAX(initial, maxsize);

	/* parse options array */
	if(options && Z_TYPE_P(options) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(options), "mirror", sizeof("mirror"), (void **) &value) == SUCCESS) {
			flags |= ILC_MIRROR;
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "peritemmirror", sizeof("peritemmirror"), (void **) &value) == SUCCESS) {
			flags |= ILC_MIRROR | ILC_PERITEMMIRROR;
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "colo", sizeof("color"), (void **) &value) == SUCCESS) {
			wingui_juggle_type(*value, IS_STRING TSRMLS_CC);
			if (_stricmp(Z_STRVAL_PP(value), "color4") == 0) {
				flags |= ILC_COLOR4;
			} else if (_stricmp(Z_STRVAL_PP(value), "color8") == 0) {
				flags |= ILC_COLOR8;
			} else if (_stricmp(Z_STRVAL_PP(value), "color16") == 0) {
				flags |= ILC_COLOR16;
			} else if (_stricmp(Z_STRVAL_PP(value), "color24") == 0) {
				flags |= ILC_COLOR24;
			} else if (_stricmp(Z_STRVAL_PP(value), "color32") == 0) {
				flags |= ILC_COLOR32;
			} else if (_stricmp(Z_STRVAL_PP(value), "colorddb") == 0) {
				flags |= ILC_COLORDDB;
			} else {
				flags |= ILC_COLOR;
			}
		} else {
			flags |= ILC_COLOR;
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "mask", sizeof("mask"), (void **) &value) == SUCCESS) {
			flags |= ILC_MASK;
		}
	}

	imagelist = ImageList_Create( x, y, flags, initial, maxsize - initial);

	if (imagelist == NULL) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}
	
	imagelist_object->imagelist = imagelist;
}
/* }}} */

/* {{{ proto bool Win\Gui\ImageList->add(Win\Gui\Resource\Bitmap bitmap[, Win\Gui\Resource\Bitmap mask])
       Adds an image or images to an image list */
PHP_METHOD(WinGuiImageList, add)
{
	zend_error_handling error_handling;
	zval *bitmap, *mask = NULL;
	HBITMAP bitmap_handle, mask_handle = NULL;
	wingui_resource_object *bitmap_object, *mask_object;
	wingui_imagelist_object *imagelist_object = (wingui_imagelist_object*)wingui_imagelist_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|O", &bitmap, ce_wingui_resource_bitmap, &mask, ce_wingui_resource_bitmap) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	bitmap_object = (wingui_resource_object *)wingui_resource_object_get(bitmap TSRMLS_CC);
	bitmap_handle = bitmap_object->resource_handle;

	if (mask) {
		mask_object = (wingui_resource_object *)wingui_resource_object_get(mask TSRMLS_CC);
		mask_handle = mask_object->resource_handle;
	}

	RETURN_LONG(ImageList_Add(imagelist_object->imagelist, bitmap_handle, mask_handle));
}
/* }}} */

/* {{{ proto bool Win\Gui\ImageList->addMasked(Win\Gui\Resource\Bitmap bitmap, int color[, int green, int blue])
       Adds an image or images to an image list, generating a mask using the color specified */
PHP_METHOD(WinGuiImageList, addMasked)
{
}
/* }}} */

/* register methods */
static zend_function_entry wingui_imagelist_functions[] = {
	PHP_ME(WinGuiImageList, __construct, WinGuiImageList___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiImageList, add, WinGuiImageList_add_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiImageList, addMasked, WinGuiImageList_addMasked_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, beginDrag, WinGuiImageList_beginDrag_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, copy, WinGuiImageList_copy_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, dragEnter, WinGuiImageList_dragEnter_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, dragLeave, WinGuiImageList_dragLeave_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, dragMove, WinGuiImageList_dragMove_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, dragShowNoLock, WinGuiImageList_dragShowNoLock_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, draw, WinGuiImageList_draw_args, ZEND_ACC_PUBLIC) // drawex and drawindirect
	// ImageList_Duplicate is for __clone
	//PHP_ME(WinGuiImageList, endDrag, WinGuiImageList_endDrag_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, getBkColor, WinGuiImageList_getBkColor_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, getDragImage, WinGuiImageList_getDragImage_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, getIcon, WinGuiImageList_getIcon_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, getIconSize, WinGuiImageList_getIconSize_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, getImageCount, WinGuiImageList_getImageCount_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, getImageInfo, WinGuiImageList_getImageInfo_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, loadImage, WinGuiImageList_loadImage_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	//PHP_ME(WinGuiImageList, merge, WinGuiImageList_merge_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, read, WinGuiImageList_read_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, remove, WinGuiImageList_remove_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, replace, WinGuiImageList_replace_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, replaceIcon, WinGuiImageList_replaceIcon_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, setBkColor, WinGuiImageList_setBkColor_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, setColorTable, WinGuiImageList_setColorTable_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, setDragCursorImage, WinGuiImageList_setDragCursorImage_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, setIconSize, WinGuiImageList_setIconSize_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, setImageCount, WinGuiImageList_setImageCount_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, setOverlayImage, WinGuiImageList_setOverlayImage_args, ZEND_ACC_PUBLIC)
	//PHP_ME(WinGuiImageList, write, WinGuiImageList_write_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\ImageList Custom Object magic                                                 
------------------------------------------------------------------*/
/* {{{ cleans up the object */
void wingui_imagelist_destructor(void *object TSRMLS_DC)
{
	wingui_imagelist_object *imagelist_object = (wingui_imagelist_object *)object;

	zend_hash_destroy(imagelist_object->std.properties);
	FREE_HASHTABLE(imagelist_object->std.properties);
	
	if(imagelist_object->imagelist){
		 ImageList_Destroy(imagelist_object->imagelist);
	}
	efree(object);
}
/* }}} */

/* {{{ creates a object */
zend_object_value wingui_imagelist_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_imagelist_object *imagelist_object;
	zval *tmp;

	imagelist_object = ecalloc(1, sizeof(wingui_imagelist_object));
	imagelist_object->std.ce = ce;
	imagelist_object->std.guards = NULL;
	imagelist_object->imagelist = NULL;

	ALLOC_HASHTABLE(imagelist_object->std.properties);
	zend_hash_init(imagelist_object->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(imagelist_object->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(imagelist_object, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) wingui_imagelist_destructor, NULL TSRMLS_CC);
	imagelist_object->handle = retval.handle;
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\ImageList LifeCycle Functions                                                    
------------------------------------------------------------------*/
/* {{{ register imagelist class */
PHP_MINIT_FUNCTION(wingui_imagelist)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "ImageList", wingui_imagelist_functions);
	ce_wingui_imagelist = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_imagelist->create_object  = wingui_imagelist_new;

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
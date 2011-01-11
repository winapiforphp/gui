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
zend_class_entry *ce_wingui_resource_bitmap;
zend_class_entry *ce_wingui_resource_image;

/* ----------------------------------------------------------------
  Win\Gui\Resource\Bitmap Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiBitmap___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, filename)
	ZEND_ARG_INFO(0, width)
	ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(WinGuiBitmap_load_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, bitmap)
	ZEND_ARG_INFO(0, width)
	ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO();

/* {{{ proto bool Win\Gui\Bitmap->__construct(string filename[, int width, int height])
       Loads in a new bitmap from file  - notice this does NOT support PHP streams and only supports
	   .bmp files - limitations of Windows API */
PHP_METHOD(WinGuiBitmap, __construct)
{
	zend_error_handling error_handling;
	HANDLE bitmap;
	char * filename;
	long width = 0, height = 0;
	int type = LR_LOADFROMFILE, str_len;
	wingui_resource_object *bitmap_object = (wingui_resource_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &filename, &str_len, &width, &height) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* if width and height are 0, we assume you want default */
	if (width == 0 && height == 0) {
		type |= LR_DEFAULTSIZE;
	}
	bitmap = LoadImage(NULL, (LPCTSTR) filename, IMAGE_BITMAP, width, height, type);
	if (bitmap == NULL) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} 
	bitmap_object->resource_handle = bitmap;
}
/* }}} */

/* {{{ proto bool Win\Gui\Bitmap::load(int bitmap[, int width, int height])
       Loads in a new system bitmap */
PHP_METHOD(WinGuiBitmap, load)
{
	zend_error_handling error_handling;
	HANDLE bitmap;
	long bitmapint, width = 0, height = 0;
	int type = LR_SHARED;
	wingui_resource_object *bitmap_object;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|ll", &bitmapint, &width, &height) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* if width and height are 0, we assume you want default */
	if (width == 0 && height == 0) {
		type |= LR_DEFAULTSIZE;
	}
	bitmap = LoadImage(NULL, MAKEINTRESOURCE(bitmapint), IMAGE_BITMAP, width, height, type);
	if (bitmap == NULL) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} 

	object_init_ex(return_value, ce_wingui_resource_bitmap);
 	bitmap_object = (wingui_resource_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	bitmap_object->resource_handle = bitmap;
	bitmap_object->shared = TRUE;
}
/* }}} */

/* register Bitmap methods */
static zend_function_entry wingui_bitmap_functions[] = {
	PHP_ME(WinGuiBitmap, __construct, WinGuiBitmap___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiBitmap, load, WinGuiBitmap_load_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Resource\Image Userland API                                                      
------------------------------------------------------------------*/

/* {{{ proto void Win\Gui\Resource\Image->__construct()
     this will gripe if you attempt to extend Image and use it, you must extend
	 one of cursor, bitmap or icon sub-classes instead */
PHP_METHOD(WinGuiResourceImage, __construct)
{
	zend_throw_exception(ce_wingui_exception, ZEND_NS_NAME(PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Image")) " cannot be extended, extend a child control instead", 0 TSRMLS_CC);
	return;
}
/* }}} */

/* register Theme methods */
static zend_function_entry wingui_resource_image_functions[] = {
	PHP_ME(WinGuiResourceImage, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Resource\Bitmap Custom Object magic                                                 
------------------------------------------------------------------*/
/* cleans up the object */
void wingui_bitmap_destructor(void *object TSRMLS_DC)
{
	wingui_resource_object *bitmap_object = (wingui_resource_object *)object;

	zend_hash_destroy(bitmap_object->std.properties);
	FREE_HASHTABLE(bitmap_object->std.properties);
	
	if (bitmap_object->resource_handle && !bitmap_object->shared) {
		DeleteObject(bitmap_object->resource_handle);
	}
	efree(object);
}
/* }}} */

/* {{{ wingui_bitmap_object_new
       creates a new bitmap object */
zend_object_value wingui_bitmap_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_resource_object *bitmap_object;
	zval *tmp;

	bitmap_object = ecalloc(1, sizeof(wingui_resource_object));
	bitmap_object->std.ce = ce;
	bitmap_object->std.guards = NULL;
	bitmap_object->resource_handle = NULL;
	bitmap_object->shared = FALSE;

	ALLOC_HASHTABLE(bitmap_object->std.properties);
	zend_hash_init(bitmap_object->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(bitmap_object->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(bitmap_object, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) wingui_bitmap_destructor, NULL TSRMLS_CC);
	bitmap_object->handle = retval.handle;
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Resource\Bitmap LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_resource_bitmap)
{
	zend_class_entry ce, image_ce;

	INIT_NS_CLASS_ENTRY(image_ce, PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Image"), wingui_resource_image_functions);
	ce_wingui_resource_image = zend_register_internal_class(&image_ce TSRMLS_CC); // TODO: make extend resource base class

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Bitmap"), wingui_bitmap_functions);
	ce_wingui_resource_bitmap = zend_register_internal_class_ex(&ce, ce_wingui_resource_image, ZEND_NS_NAME(PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Image")) TSRMLS_CC);
	ce_wingui_resource_bitmap->create_object  = wingui_bitmap_new;

	/* OBM bitmap constants */
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_CLOSE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_UPARROW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_DNARROW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_RGARROW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_LFARROW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_REDUCE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_ZOOM);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_RESTORE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_REDUCED);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_ZOOMD);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_RESTORED);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_UPARROWD);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_DNARROWD);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_RGARROWD);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_LFARROWD);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_MNARROW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_COMBO);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_UPARROWI);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_DNARROWI);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_RGARROWI);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_LFARROWI);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_OLD_CLOSE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_SIZE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_OLD_UPARROW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_OLD_DNARROW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_OLD_RGARROW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_OLD_LFARROW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_BTSIZE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_CHECK);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_CHECKBOXES);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_BTNCORNERS);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_OLD_REDUCE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_OLD_ZOOM);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_bitmap,		OBM_OLD_RESTORE);

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
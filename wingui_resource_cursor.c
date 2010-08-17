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
zend_class_entry *ce_wingui_resource_cursor;

/* ----------------------------------------------------------------
  Win\Gui\Resource\Cursor Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiCursor___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, filename)
	ZEND_ARG_INFO(0, width)
	ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(WinGuiCursor_load_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, cursor)
	ZEND_ARG_INFO(0, width)
	ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO();

/* {{{ proto bool Win\Gui\Resource\Cursor->__construct(string filename[, int width, int height])
       Loads in a new cursor from file  - notice this does NOT support PHP streams and only supports
	   .ani files - limitations of Windows API */
PHP_METHOD(WinGuiCursor, __construct)
{
	zend_error_handling error_handling;
	HANDLE cursor;
	char * filename;
	long width = 0, height = 0;
	int type = LR_LOADFROMFILE, str_len;
	wingui_resource_object *cursor_object = (wingui_resource_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &filename, &str_len, &width, &height) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* if width and height are 0, we assume you want default */
	if (width == 0 && height == 0) {
		type |= LR_DEFAULTSIZE;
	}
	cursor = LoadImage(NULL, (LPCTSTR) filename, IMAGE_CURSOR, width, height, type);
	if (cursor == NULL) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} 
	cursor_object->resource_handle = cursor;
}
/* }}} */

/* {{{ proto bool Win\Gui\Cursor::load(int cursor[, int width, int height])
       Loads in a new system cursor */
PHP_METHOD(WinGuiCursor, load)
{
	zend_error_handling error_handling;
	HANDLE cursor;
	long cursorint, width = 0, height = 0;
	int type = LR_SHARED;
	wingui_resource_object *cursor_object;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|ll", &cursorint, &width, &height) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* if width and height are 0, we assume you want default */
	if (width == 0 && height == 0) {
		type |= LR_DEFAULTSIZE;
	}
	cursor = LoadImage(NULL, MAKEINTRESOURCE(cursorint), IMAGE_CURSOR, width, height, type);
	if (cursor == NULL) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} 

	object_init_ex(return_value, ce_wingui_resource_cursor);
 	cursor_object = (wingui_resource_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	cursor_object->resource_handle = cursor;
	cursor_object->shared = TRUE;
}
/* }}} */

/* register Cursor methods */
static zend_function_entry wingui_cursor_functions[] = {
	PHP_ME(WinGuiCursor, __construct, WinGuiCursor___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiCursor, load, WinGuiCursor_load_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Resource\Cursor Custom Object magic                                                 
------------------------------------------------------------------*/
/* cleans up the object */
void wingui_cursor_destructor(void *object TSRMLS_DC)
{
	wingui_resource_object *cursor_object = (wingui_resource_object *)object;

	zend_hash_destroy(cursor_object->std.properties);
	FREE_HASHTABLE(cursor_object->std.properties);
	
	if (cursor_object->resource_handle && !cursor_object->shared) {
		DestroyCursor(cursor_object->resource_handle);
	}
	efree(object);
}
/* }}} */

/* {{{ wingui_cursor_object_new
       creates a new cursor object */
zend_object_value wingui_cursor_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_resource_object *cursor_object;
	zval *tmp;

	cursor_object = ecalloc(1, sizeof(wingui_resource_object));
	cursor_object->std.ce = ce;
	cursor_object->std.guards = NULL;
	cursor_object->resource_handle = NULL;
	cursor_object->shared = FALSE;

	ALLOC_HASHTABLE(cursor_object->std.properties);
	zend_hash_init(cursor_object->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(cursor_object->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(cursor_object, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) wingui_cursor_destructor, NULL TSRMLS_CC);
	cursor_object->handle = retval.handle;
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Resource\Cursor LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_resource_cursor)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Cursor"), wingui_cursor_functions);
	ce_wingui_resource_cursor = zend_register_internal_class_ex(&ce, ce_wingui_resource_image, ZEND_NS_NAME(PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Image")) TSRMLS_CC);
	ce_wingui_resource_cursor->create_object  = wingui_cursor_new;

	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_NORMAL);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_IBEAM);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_WAIT);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_CROSS);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_UP);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_SIZE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_ICON);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_SIZENWSE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_SIZENESW);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_SIZEWE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_SIZENS);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_SIZEALL);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_ICOCUR);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_NO);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_HAND);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_cursor,		OCR_APPSTARTING);
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
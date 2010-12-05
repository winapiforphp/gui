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
#include "php_winsystem_api.h"
#include "zend_exceptions.h"

zend_class_entry *ce_wingui_resource_cursor;
static zend_object_handlers wingui_resource_cursor_object_handlers;
static zend_function wingui_resource_cursor_constructor_wrapper;

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

/* {{{ proto bool Win\Gui\Resource\Cursor->__construct(string|object filename[, int width, int height])
       Loads in a new cursor from file  - notice this does NOT support PHP streams and only supports
	   .ani files - limitations of Windows API */
PHP_METHOD(WinGuiCursor, __construct)
{
	/* Used for regular string */
	char * name = NULL;
	int name_length, type = LR_LOADFROMFILE;

	/* Used for wchar string */
	zval *unicode = NULL;
	winsystem_unicode_object *unicode_object = NULL;
	int use_unicode = 0;

	zend_error_handling error_handling;
	HANDLE cursor;
	long width = 0, height = 0;
	wingui_resource_object *cursor_object = (wingui_resource_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &name, &name_length, &width, &height) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* if width and height are 0, we assume you want default */
	if (width == 0 && height == 0) {
		type |= LR_DEFAULTSIZE;
	}
	cursor = LoadImage(NULL, (LPCTSTR) name, IMAGE_CURSOR, width, height, type);
	if (cursor == NULL) {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
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
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} 

	object_init_ex(return_value, ce_wingui_resource_cursor);
 	cursor_object = (wingui_resource_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	cursor_object->resource_handle = cursor;
	cursor_object->shared = TRUE;
}
/* }}} */

/* register Cursor methods */
static zend_function_entry wingui_resource_cursor_functions[] = {
	//PHP_ME(WinGuiCursor, __construct, WinGuiCursor___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	//PHP_ME(WinGuiCursor, load, WinGuiCursor_load_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Resource\Cursor Custom Object magic
------------------------------------------------------------------*/

/* {{{ wingui_resource_cursor_get_constructor
       gets the constructor for the class  */
static zend_function *wingui_resource_cursor_get_constructor(zval *object TSRMLS_DC)
{
	if (Z_OBJCE_P(object) == ce_wingui_resource_cursor)
		return zend_get_std_object_handlers()->
			get_constructor(object TSRMLS_CC);
	else
		return &wingui_resource_cursor_constructor_wrapper;
}
/* }}} */

/* {{{ wingui_resource_cursor_object_free
       frees the cursor*/
static void wingui_resource_cursor_object_free(void *object TSRMLS_DC)
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

/* {{{ wingui_resource_cursor_object_create
       creates a new cursor object */
zend_object_value wingui_resource_cursor_object_create(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_resource_object *cursor_object;

	cursor_object = ecalloc(1, sizeof(wingui_resource_object));
	zend_object_std_init((zend_object *) cursor_object, ce TSRMLS_CC);
	cursor_object->resource_handle = NULL;
	cursor_object->shared = FALSE;

	zend_hash_copy(cursor_object->std.properties, &(ce->default_properties),
		(copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval*));

	retval.handle = zend_objects_store_put(cursor_object, (zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t) wingui_resource_cursor_object_free, NULL TSRMLS_CC);
	retval.handlers = &wingui_resource_cursor_object_handlers;
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Resource\Cursor LifeCycle Functions
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_resource_cursor)
{
	zend_class_entry ce;

	memcpy(&wingui_resource_cursor_object_handlers, zend_get_std_object_handlers(),
		sizeof wingui_resource_cursor_object_handlers);
	wingui_resource_cursor_object_handlers.get_constructor = wingui_resource_cursor_get_constructor;
	// TODO: clone and var_dump
	//wingui_resource_cursor_object_handlers.clone_obj = clone;
	//wingui_resource_cursor_object_handlers.get_debug_info = debug_info;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Cursor"), wingui_resource_cursor_functions);
	//ce_wingui_resource_cursor = zend_register_internal_class_ex(&ce, ce_wingui_resource_image, ZEND_NS_NAME(PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Image")) TSRMLS_CC);
	ce_wingui_resource_cursor = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_resource_cursor->create_object = wingui_resource_cursor_object_create;

	wingui_resource_cursor_constructor_wrapper.type = ZEND_INTERNAL_FUNCTION;
	wingui_resource_cursor_constructor_wrapper.common.function_name = "internal_construction_wrapper";
	wingui_resource_cursor_constructor_wrapper.common.scope = ce_wingui_resource_cursor;
	wingui_resource_cursor_constructor_wrapper.common.fn_flags = ZEND_ACC_PROTECTED;
	wingui_resource_cursor_constructor_wrapper.common.prototype = NULL;
	wingui_resource_cursor_constructor_wrapper.common.required_num_args = 0;
	wingui_resource_cursor_constructor_wrapper.common.arg_info = NULL;
	wingui_resource_cursor_constructor_wrapper.common.pass_rest_by_reference = 0;
	wingui_resource_cursor_constructor_wrapper.common.return_reference = 0;
	wingui_resource_cursor_constructor_wrapper.internal_function.handler = wingui_resource_construction_wrapper;
	wingui_resource_cursor_constructor_wrapper.internal_function.module = EG(current_module);

	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_NORMAL",		sizeof("OCR_NORMAL") - 1,		OCR_NORMAL		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_IBEAM",		sizeof("OCR_IBEAM") - 1,		OCR_IBEAM		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_WAIT",			sizeof("OCR_WAIT") - 1,			OCR_WAIT		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_CROSS",		sizeof("OCR_CROSS") - 1,		OCR_NORMAL		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_UP",			sizeof("OCR_UP") - 1,			OCR_UP			TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_SIZE",			sizeof("OCR_SIZE") - 1,			OCR_SIZE		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_ICON",			sizeof("OCR_ICON") - 1,			OCR_ICON		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_SIZENWSE",		sizeof("OCR_SIZENWSE") - 1,		OCR_SIZENWSE	TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_SIZENESW",		sizeof("OCR_SIZENESW") - 1,		OCR_SIZENESW	TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_SIZEWE",		sizeof("OCR_SIZEWE") - 1,		OCR_SIZEWE		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_SIZENS",		sizeof("OCR_SIZENS") - 1,		OCR_SIZENS		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_SIZEALL",		sizeof("OCR_SIZEALL") - 1,		OCR_SIZEALL		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_ICOCUR",		sizeof("OCR_ICOCUR") - 1,		OCR_ICOCUR		TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_NO",			sizeof("OCR_NO") - 1,			OCR_NO			TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_APPSTARTING",	sizeof("OCR_APPSTARTING") - 1,	OCR_APPSTARTING	TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_HAND",			sizeof("OCR_HAND") - 1,			OCR_HAND		TSRMLS_CC);
	/* although not defined in OCR constants, is defined as IDC constants */
	zend_declare_class_constant_long(ce_wingui_resource_cursor, "OCR_HELP",			sizeof("OCR_HELP") - 1,			32651			TSRMLS_CC);

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
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
zend_class_entry *ce_wingui_resource_icon;

/* ----------------------------------------------------------------
  Win\Gui\Resource\Icon Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiIcon___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, filename)
	ZEND_ARG_INFO(0, width)
	ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(WinGuiIcon_load_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, icon)
	ZEND_ARG_INFO(0, width)
	ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO();

/* {{{ proto bool Win\Gui\Resource\Icon->__construct(string filename[, int width, int height])
       Loads in a new icon from file  - notice this does NOT support PHP streams and only supports
	   .ico files - limitations of Windows API
	   TODO: hook createicon for streams/string support */
PHP_METHOD(WinGuiIcon, __construct)
{
	zend_error_handling error_handling;
	HANDLE icon;
	char * filename;
	long width = 0, height = 0;
	int type = LR_LOADFROMFILE, str_len;
	wingui_resource_object *icon_object = (wingui_resource_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &filename, &str_len, &width, &height) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* if width and height are 0, we assume you want default */
	if (width == 0 && height == 0) {
		type |= LR_DEFAULTSIZE;
	}
	icon = LoadImage(NULL, (LPCTSTR) filename, IMAGE_ICON, width, height, type);
	if (icon == NULL) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} 
	icon_object->resource_handle = icon;
}
/* }}} */

/* {{{ proto bool Win\Gui\Icon::load(int icon[, int width, int height])
       Loads in a new system icon */
PHP_METHOD(WinGuiIcon, load)
{
	zend_error_handling error_handling;
	HICON icon;
	long iconint, width = 0, height = 0;
	int type = LR_SHARED | LR_DEFAULTCOLOR;
	wingui_resource_object *icon_object;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|ll", &iconint, &width, &height) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

    /* if width and height are 0, we assume you want default */
	if (width == 0 && height == 0) {
		type |= LR_DEFAULTSIZE;
	}

	/* Different code paths for vista+ */
	if(wingui_is_vista(TSRMLS_C)) {
		typedef HRESULT (WINAPI *LoadIconWithScaleDownFunc)(HINSTANCE hinst, LPSTR pszName, int cx, int cy, HICON *phico);
		LoadIconWithScaleDownFunc LoadIconWithScaleDownFunchandle;
		HMODULE module = LoadLibrary("comctl32.dll");

		LoadIconWithScaleDownFunchandle = (LoadIconWithScaleDownFunc)GetProcAddress(module, "LoadIconWithScaleDown");
		if (LoadIconWithScaleDownFunchandle == NULL) {
			icon = LoadImage(NULL, MAKEINTRESOURCE(iconint), IMAGE_ICON, width, height, type);
		} else {
			LoadIconWithScaleDownFunchandle(NULL, MAKEINTRESOURCE(iconint), width, height, &icon);
		}
		FreeLibrary(module);
		
	} else {
		icon = LoadImage(NULL, MAKEINTRESOURCE(iconint), IMAGE_ICON, width, height, type);
	}

	if (icon == NULL) {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	} 

	object_init_ex(return_value, ce_wingui_resource_icon);
 	icon_object = (wingui_resource_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	icon_object->resource_handle = icon;
	icon_object->shared = TRUE;
}
/* }}} */

/* register Image methods */
static zend_function_entry wingui_icon_functions[] = {
	PHP_ME(WinGuiIcon, __construct, WinGuiIcon___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiIcon, load, WinGuiIcon_load_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Resource\Icon Custom Object magic                                                 
------------------------------------------------------------------*/
/* cleans up the object */
void wingui_icon_destructor(void *object TSRMLS_DC)
{
	wingui_resource_object *icon_object = (wingui_resource_object *)object;

	zend_hash_destroy(icon_object->std.properties);
	FREE_HASHTABLE(icon_object->std.properties);
	
	if (icon_object->resource_handle && !icon_object->shared) {
		DestroyIcon(icon_object->resource_handle);
	}
	efree(object);
}
/* }}} */

/* {{{ wingui_icon_object_new
       creates a new icon object */
zend_object_value wingui_icon_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_resource_object *icon_object;
	zval *tmp;

	icon_object = ecalloc(1, sizeof(wingui_resource_object));
	icon_object->std.ce = ce;
	icon_object->std.guards = NULL;
	icon_object->resource_handle = NULL;
	icon_object->shared = FALSE;

	ALLOC_HASHTABLE(icon_object->std.properties);
	zend_hash_init(icon_object->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(icon_object->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(icon_object, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) wingui_icon_destructor, NULL TSRMLS_CC);
	icon_object->handle = retval.handle;
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Resource\Icon LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_resource_icon)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Icon"), wingui_icon_functions);
	ce_wingui_resource_icon = zend_register_internal_class_ex(&ce, ce_wingui_resource_image, ZEND_NS_NAME(PHP_WINGUI_NS, ZEND_NS_NAME("Resource", "Image")) TSRMLS_CC);
	ce_wingui_resource_icon->create_object  = wingui_icon_new;

	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_SAMPLE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_HAND);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_QUES);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_BANG);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_NOTE);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_WINLOGO);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_WARNING);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_ERROR);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_INFORMATION);
	REGISTER_WINGUI_CONSTANT(ce_wingui_resource_icon,		OIC_SHIELD);

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
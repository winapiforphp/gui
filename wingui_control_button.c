/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2007-2008 Elizabeth M. Smith, Sara Golemon, Tom Rogers |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Elizabeth M. Smith <auroraeosrose@php.net>                   |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#include "php_wingui.h"
#include "wingui_control.h"
#include "zend_exceptions.h"

ZEND_DECLARE_MODULE_GLOBALS(wingui);

/* Extends ce_WinUiClass */
zend_class_entry *ce_WinGuiButton;
zend_class_entry *ce_WinUiButtonRadio;
zend_class_entry *ce_WinUiButtonCheckBox;
zend_class_entry *ce_WinUiButtonFieldset;

/* Custom Object Handler Items */
zend_object_handlers wingui_button_object_handlers;
HashTable wingui_button_prop_handlers;
HashTable wingui_button_callback_map;
zend_object_value wingui_button_object_new(zend_class_entry *class_type TSRMLS_DC);

/* we "subclass" the button class with our own custom proc */
LRESULT CALLBACK wingui_button_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	zval ***extra = NULL;
	int extra_count = 0;
	zend_bool stop_default = FALSE;
	zval *return_value = NULL;
#ifdef ZTS
	TSRMLS_D;
#endif;

	wingui_window_object *button_object = (wingui_window_object *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
#ifdef ZTS
	TSRMLS_C = button_object->TSRMLS_C;
#endif

	switch(msg) {
		break;
	}

	if(button_object) {
		return_value = wingui_winproc_callback_dispatch(button_object, msg, extra, extra_count, &stop_default TSRMLS_CC);
	}

	if (extra) {
		wingui_callback_extra_zvals_dtor(extra_count, extra);
	}

	/* return is based on if we need to call the default proc handling, and if we have an overwritten
	   return value to use instead of the default value */
	if (stop_default && return_value) {
		//return wingui_progressbar_messages_results(msg, return_value TSRMLS_CC);
	} else if (stop_default) {
		return (LRESULT) NULL;
	} else if (return_value) {
		CallWindowProc(button_object->window_proc, hwnd, msg, wParam, lParam);
		//return wingui_progressbar_messages_results(msg, return_value TSRMLS_CC);
	} else {
		return CallWindowProc(button_object->window_proc, hwnd, msg, wParam, lParam); 
	}
}

/* ----------------------------------------------------------------
  Win::Ui::Button Custom Object magic                                                 
------------------------------------------------------------------*/

/* {{{ wingui_button_object_new
       creates a new winguibutton object and takes care of the internal junk */
zend_object_value wingui_button_object_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_window_object *button_object;
	zval *tmp;

	button_object = emalloc(sizeof(wingui_window_object));
	button_object->std.ce = ce;
	button_object->std.guards = NULL;
	button_object->window_handle = NULL;
	button_object->prop_handler = &wingui_button_prop_handlers;
	button_object->callback_map = &wingui_button_callback_map;
	button_object->window_proc = NULL;
#ifdef ZTS
	button_object->TSRMLS_C = TSRMLS_C;
#endif

	ALLOC_HASHTABLE(button_object->std.properties);
	zend_hash_init(button_object->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(button_object->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
	ALLOC_HASHTABLE(button_object->registered_callbacks);
	zend_hash_init(button_object->registered_callbacks, 0, NULL, NULL, 0);

	retval.handle = zend_objects_store_put(button_object, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) wingui_generic_destructor, wingui_generic_clone TSRMLS_CC);
	button_object->handle = retval.handle;
	retval.handlers = &wingui_button_object_handlers;
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
  Win::Ui::Button User API                                                      
------------------------------------------------------------------*/

/* Methods for the class */
PHP_METHOD(WinUiButton, __construct);
PHP_METHOD(WinUiButtonCheckBox, __construct);
PHP_METHOD(WinUiButtonRadio, __construct);

/* arginfo for the class */
ZEND_BEGIN_ARG_INFO_EX(arginfo_wingui_button___construct, 0, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_OBJ_INFO(0, parent, Win::Gui::Container, 1)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO();

/* regular Window methods */
static const zend_function_entry wingui_button_functions[] = {
	PHP_ME(WinUiButton, __construct, arginfo_wingui_button___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};
static const zend_function_entry wingui_button_checkbox_functions[] = {
	PHP_ME(WinUiButtonCheckBox, __construct, arginfo_wingui_button___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};
static const zend_function_entry wingui_button_radio_functions[] = {
	PHP_ME(WinUiButtonRadio, __construct, arginfo_wingui_button___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};

/* {{{ __construct */
PHP_METHOD(WinUiButton, __construct)
{
	char *text = "Button";
	int x = 0, y = 0, width = 100, height = 100, style = WS_VISIBLE | BS_DEFPUSHBUTTON, extrastyle = 0;
	zval *parent, *options = NULL;
	RECT client;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O!|a", &parent, ce_WinGuiContainer, &options) == FAILURE) {
		return;
	}

	if(options && Z_TYPE_P(options) == IS_ARRAY)
	{
		/* Basic window options */
		if(wingui_generic_parse_options(options, &x, &y, &height, &width, &text, &style, &extrastyle TSRMLS_CC) == FAILURE)
		{
			return;
		}
		/* Button specific options */
	}

	/* Style needs some convoluted checking */
	if (((style & WS_POPUP) == WS_POPUP) && ((style & WS_CHILD) == WS_CHILD))
	{
		zend_throw_exception(wingui_exception_get_ce(TSRMLS_C), "The POPUP and CHILD styles cannot be used together", 0 TSRMLS_CC);
	}
	/* A child window MUST have a parent zval */
		if (((style & WS_CHILD) == WS_CHILD) && !parent)
	{
		zend_throw_exception(wingui_exception_get_ce(TSRMLS_C), "The CHILD style requires a parent window", 0 TSRMLS_CC);
	}
	/* a window with a parent must have either popup or child set */
	if (parent && !((style & WS_POPUP) == WS_POPUP))
	{
		style |= WS_CHILD;
	}
	client.top = 0;
	client.left = 0;
	client.bottom = height;
	client.right = width;
	AdjustWindowRectEx(&client, style, 0, extrastyle);

	wingui_generic_constructor("Button", text, x, y, client.right - client.left, client.bottom - client.top, style, extrastyle, parent, NULL, getThis(), wingui_button_proc_handler TSRMLS_CC);
	return;
}
/* }}} */

/* {{{ __construct */
PHP_METHOD(WinUiButtonCheckBox, __construct)
{
	char *title = "CheckBox";
	int x = 0, y = 0, width = 100, height = 100, style = 0;
	zval *parent, *options = NULL, **value;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|a", &parent, ce_WinGuiContainer, &options) == FAILURE) {
		return;
	}

	if(options && Z_TYPE_P(options) == IS_ARRAY)
	{
		if (zend_hash_find(Z_ARRVAL_P(options), "x", sizeof("x"), (void **) &value) == SUCCESS) {
			x = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "y", sizeof("y"), (void **) &value) == SUCCESS) {
			y = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "style", sizeof("style"), (void **) &value) == SUCCESS) {
			style = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "width", sizeof("width"), (void **) &value) == SUCCESS) {
			width = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "height", sizeof("height"), (void **) &value) == SUCCESS) {
			height = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "text", sizeof("text"), (void **) &value) == SUCCESS) {
			title = Z_STRVAL_PP(value);
		}
	}

	wingui_generic_constructor("Button", title, x, y, width, height, style | BS_CHECKBOX, 0, parent, NULL, getThis(), wingui_button_proc_handler TSRMLS_CC);
	return;
}
/* }}} */

/* {{{ __construct */
PHP_METHOD(WinUiButtonRadio, __construct)
{
	char *title = "Radio";
	int x = 0, y = 0, width = 100, height = 100, style = 0;
	zval *parent, *options = NULL, **value;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|a", &parent, ce_WinGuiContainer, &options) == FAILURE) {
		return;
	}

	if(options && Z_TYPE_P(options) == IS_ARRAY)
	{
		if (zend_hash_find(Z_ARRVAL_P(options), "x", sizeof("x"), (void **) &value) == SUCCESS) {
			x = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "y", sizeof("y"), (void **) &value) == SUCCESS) {
			y = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "style", sizeof("style"), (void **) &value) == SUCCESS) {
			style = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "width", sizeof("width"), (void **) &value) == SUCCESS) {
			width = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "height", sizeof("height"), (void **) &value) == SUCCESS) {
			height = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "text", sizeof("text"), (void **) &value) == SUCCESS) {
			title = Z_STRVAL_PP(value);
		}
	}

	wingui_generic_constructor("Button", title, x, y, width, height, style | BS_AUTORADIOBUTTON, 0, parent, NULL, getThis(), wingui_button_proc_handler TSRMLS_CC);
	return;
}
/* }}} */

PHP_MINIT_FUNCTION(wingui_button)
{
	zend_class_entry ce;

	// handler and callback initalization
	memcpy(&wingui_button_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	wingui_button_object_handlers.unset_property = wingui_delete_property;
	wingui_button_object_handlers.write_property = wingui_write_property;
	wingui_button_object_handlers.read_property = wingui_read_property;
	wingui_button_object_handlers.get_property_ptr_ptr = wingui_get_property_ptr_ptr;
	wingui_button_object_handlers.has_property = wingui_property_exists;
	wingui_button_object_handlers.get_debug_info = wingui_get_debug_info;

	/* Button Classes */
	INIT_CLASS_ENTRY(ce, "Win::Gui::Control::Button", wingui_button_functions);
	ce_WinGuiButton = zend_register_internal_class_ex(&ce, ce_WinGuiControl, "Win::Gui::Control" TSRMLS_CC);
	ce_WinGuiButton->create_object  = wingui_button_object_new;

	/* property handlers for "fake" properties - window styles and extra styles */
	zend_hash_init(&wingui_button_prop_handlers, 0, NULL, NULL, 1);
	memcpy(&wingui_button_prop_handlers, &wingui_window_prop_handlers, sizeof(wingui_window_prop_handlers));


	

	INIT_CLASS_ENTRY(ce, "Win::Ui::Button::Radio", wingui_button_radio_functions);
	ce_WinUiButtonRadio = zend_register_internal_class_ex(&ce, ce_WinGuiButton, "Win::Ui::Button" TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "Win::Ui::Button::CheckBox", wingui_button_checkbox_functions);
	ce_WinUiButtonCheckBox = zend_register_internal_class_ex(&ce, ce_WinGuiButton, "Win::Ui::Button" TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "Win::Ui::Button::Fieldset", NULL);
	ce_WinUiButtonFieldset = zend_register_internal_class_ex(&ce, ce_WinGuiButton, "Win::Ui::Button" TSRMLS_CC);

	zend_hash_init(&wingui_button_callback_map, 0, NULL, NULL, 1);
	
	//WINUI_DECLARE_MESSAGE_CONSTANT(ce_WinUiButtonMessages, HOTITEMCHANGE,   BCN_HOTITEMCHANGE,   &wingui_button_callback_map, onhotitemchange);
	WINGUI_DECLARE_MESSAGE_CONSTANT(ce_WinGuiButton, BN_CLICKED,				&wingui_button_callback_map, onbnclicked);

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

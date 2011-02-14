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

#include "php_wingui.h"
#include "php_winsystem_api.h"
#include "zend_exceptions.h"

/* implements inputing, windowing, messaging and control */
#include "implement_windowing.h"
//#include "implement_inputing.h"
#include "implement_messaging.h"
#include "implement_control.h"

/* Static controls - text, image and frame */
zend_class_entry *ce_wingui_control_statictext;
zend_class_entry *ce_wingui_control_staticimage;
zend_class_entry *ce_wingui_control_staticframe;

/* class properties */
HashTable wingui_control_statictext_prop_handlers;
HashTable wingui_control_staticimage_prop_handlers;
HashTable wingui_control_staticframe_prop_handlers;

/* callbacks are shared */
HashTable wingui_control_static_callback_map;

/* C API used here */
zval ***wingui_control_static_messages_cracker(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC);
void wingui_control_static_messages_packer(int msg, WPARAM *wParam, LPARAM *lParam, zval ***extra TSRMLS_DC);
LRESULT wingui_control_static_messages_results(int msg, zval *return_value TSRMLS_DC);
void wingui_control_static_parse_options(zval *options, int *style TSRMLS_DC);
void wingui_control_static_property_values(zval *object, int *style TSRMLS_DC);

/* ----------------------------------------------------------------
  Win\Gui\Control\Text Userland API
------------------------------------------------------------------*/

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlText___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
	ZEND_ARG_INFO(0, parent)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\Text::__construct(Win\Gui\Window parent, string|object text[, array options])
     Create a new text area static control */
PHP_METHOD(WinGuiControlText, __construct)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object;
	char *text = "";
	int text_len;
	zval *parent = NULL, *options = NULL;
	long x = 0, y = 0, width = 400, height = 400, style = WS_CHILD, extrastyle = 0;

	/* Populate data from properties - notice we send NULL to text (we're ignoring it in favor of the non-options array variable) */
	//wingui_window_object_property_values(getThis(), &x, &y, &height, &width, NULL, &style, &extrastyle TSRMLS_CC);
	//wingui_control_static_property_values(getThis(), &style TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	//if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Os|a", &parent, ce_wingui_window, &text, &text_len, &options) == FAILURE) {
	//	return;
	//}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if(options && Z_TYPE_P(options) == IS_ARRAY) {
		//wingui_window_object_parse_options(options, &x, &y, &height, &width, NULL, &style, &extrastyle TSRMLS_CC);
		//wingui_control_static_parse_options(options, &style TSRMLS_CC);	
	}

	//window_object = wingui_window_object_constructor("Static", text, x, y, width, height, style, extrastyle, parent, NULL, getThis(), wingui_control_static_proc_handler TSRMLS_CC);
}
/* }}} */

/* register text methods */
static zend_function_entry wingui_control_statictext_functions[] = {
	PHP_ME(WinGuiControlText, __construct, WinGuiControlText___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Control\Image Userland API
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiControlImage___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
	ZEND_ARG_INFO(0, parent)
	ZEND_ARG_INFO(0, image)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlImage_setImage_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, image)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\Control\Image::__construct(Win\Gui\Window parent, Win\Gui\Resource\Image resource)
     Create a new static control with a bitmap or an icon or a cursor */
PHP_METHOD(WinGuiControlImage, __construct)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object;
	zval *image = NULL;
	wingui_resource_object *bitmap_object;
	zval *parent = NULL, *options = NULL;
	int image_type = IMAGE_BITMAP;
	long x = 0, y = 0, width = 0, height = 0, style = WS_CHILD, extrastyle = 0;

	/* Populate data from properties - notice we send NULL to text (we're ignoring it) */
	wingui_window_object_property_values(getThis(), &x, &y, &height, &width, NULL, &style, &extrastyle TSRMLS_CC);
	wingui_control_static_property_values(getThis(), &style TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OO|a", &parent, ce_wingui_window, &image, ce_wingui_resource_image, &options) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if(instanceof_function(Z_OBJCE_P(image), ce_wingui_resource_cursor TSRMLS_CC)) {
		image_type = IMAGE_CURSOR;
		style |= SS_ICON;
	} else if(instanceof_function(Z_OBJCE_P(image), ce_wingui_resource_icon TSRMLS_CC)) {
		image_type = IMAGE_ICON;
		style |= SS_ICON;
	} else {
		style |= SS_BITMAP;
	}
	
	if(options && Z_TYPE_P(options) == IS_ARRAY) {
		wingui_window_object_parse_options(options, &x, &y, &height, &width, NULL, &style, &extrastyle TSRMLS_CC);
		wingui_control_static_parse_options(options, &style TSRMLS_CC);	
	}

	window_object = wingui_window_object_constructor("Static", "", x, y, width, height, style, extrastyle, parent, NULL, getThis(), wingui_control_static_proc_handler TSRMLS_CC);
	/* Send the message to set the image right - constructor will want a resource id - we don't do those */
	if (image) {
		Z_ADDREF_P(image);
		window_object->image = image;
		bitmap_object = (wingui_resource_object *)wingui_resource_object_get(image TSRMLS_CC);
		SendMessage(window_object->window_handle, STM_SETIMAGE, (WPARAM) image_type, (LPARAM) bitmap_object->resource_handle);
	}
}
/* }}} */

/* {{{ proto  Win\Gui\Resource\Image Win\Gui\Control\Image->setImage(Win\Gui\Resource\Image image);
     set a new bitmap/cursor/icon image */
PHP_METHOD(WinGuiControlImage, setImage)
{
	zend_error_handling error_handling;
	zval *image = NULL;
	LRESULT oldimage;
	int image_type = IMAGE_BITMAP, style;
	HANDLE image_resource = NULL;
	wingui_resource_object *bitmap_object;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);
	style = GetWindowLongPtr(window_object->window_handle, GWL_STYLE);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &image, ce_wingui_resource_image) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if(instanceof_function(Z_OBJCE_P(image), ce_wingui_resource_cursor TSRMLS_CC)) {
		image_type = IMAGE_CURSOR;
		style &= ~SS_BITMAP;
		style |= SS_ICON;
		SetWindowLongPtr(window_object->window_handle, GWL_STYLE, style);
	} else if(instanceof_function(Z_OBJCE_P(image), ce_wingui_resource_icon TSRMLS_CC)) {
		image_type = IMAGE_ICON;
		style &= ~SS_BITMAP;
		style |= SS_ICON;
		SetWindowLongPtr(window_object->window_handle, GWL_STYLE, style);
	} else {
		style &= ~SS_ICON;
		style |= SS_BITMAP;
		SetWindowLongPtr(window_object->window_handle, GWL_STYLE, style);
	}

	/* if image is not null, grab the handle */
	if (image != NULL) {
		bitmap_object = (wingui_resource_object *)wingui_resource_object_get(image TSRMLS_CC);
		image_resource = bitmap_object->resource_handle;
	}

	/* send the message to do the change */
	oldimage = SendMessage(window_object->window_handle, STM_SETIMAGE, (WPARAM) image_type, (LPARAM) image_resource);

	/* If there was an image resource previously, return that, delete a ref and unstore it */
	if (window_object != NULL && window_object->image != NULL) {
		zval_dtor(return_value);
		*return_value = *window_object->image;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
		Z_DELREF_P(window_object->image);
		window_object->image = NULL;
	}

	/* if there is a new image, up the ref and store it */
	if (image != NULL) {
		Z_ADDREF_P(image);
		window_object->image = image;
	}
}
/* }}} */

/* {{{ proto Win\Gui\Resource\Image Win\Gui\Control\Image->getImage();
     gets the bitmap/cursor/icon image */
PHP_METHOD(WinGuiControlImage, getImage)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	
	if (window_object == NULL || window_object->image == NULL) {
		return;
	} else {
		zval_dtor(return_value);
		*return_value = *window_object->image;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
	}
}
/* }}} */

/* register Image methods */
static zend_function_entry wingui_control_staticimage_functions[] = {
	PHP_ME(WinGuiControlImage, __construct, WinGuiControlImage___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiControlImage, setImage, WinGuiControlImage_setImage_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiControlImage, getImage, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Control\Frame Userland API
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO_EX(WinGuiControlFrame___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, parent)
	ZEND_ARG_INFO(0, filled)
	ZEND_ARG_INFO(0, color)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\Control\Frame::__construct([array options])
     Create a new static frame control */
PHP_METHOD(WinGuiControlFrame, __construct)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object;
	zval *parent = NULL, *options = NULL, *member = NULL, *temp = NULL;
	zend_bool filled = FALSE;
	long x = 0, y = 0, width = 400, height = 400, color = 1, style = WS_CHILD, extrastyle = 0;

	/* Populate data from properties - notice we send NULL to text (we're ignoring it) */
	wingui_window_object_property_values(getThis(), &x, &y, &height, &width, NULL, &style, &extrastyle TSRMLS_CC);
	wingui_control_static_property_values(getThis(), &style TSRMLS_CC);

	/* Populate class specific data */
	MAKE_STD_ZVAL(member);
	MAKE_STD_ZVAL(temp);
	

	ZVAL_STRING(member, "filled", 0);
	if (std_object_handlers.has_property(getThis(), member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(getThis(), member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		filled = Z_BVAL_P(temp);
	}

	ZVAL_STRING(member, "color", 0);
	if (std_object_handlers.has_property(getThis(), member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(getThis(), member, 0 TSRMLS_CC), IS_LONG TSRMLS_CC);
		color = Z_LVAL_P(temp);
	}

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|bla", &parent, ce_wingui_window, &filled, &color, &options) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* Figure out the flags to set for frame colors and options */
	if (color == 5) {
		style |= SS_ETCHEDHORZ;
	} else if (color == 6) {
		style |= SS_ETCHEDVERT;
	} else if (color == 4) {
		style |= SS_ETCHEDFRAME;
	} else if (filled == TRUE) {
		if (color == 2) {
			style |= SS_WHITERECT;
		} else if (color == 3) {
			style |= SS_GRAYRECT;
		} else { /* Color is 1 */
			style |= SS_BLACKRECT;
		}
	} else {
		if (color == 2) {
			style |= SS_WHITEFRAME;
		} else if (color == 3) {
			style |= SS_GRAYFRAME;
		} else { /* Color is 1 */
			style |= SS_BLACKFRAME;
		}
	}

	if(options && Z_TYPE_P(options) == IS_ARRAY) {
		wingui_window_object_parse_options(options, &x, &y, &height, &width, NULL, &style, &extrastyle TSRMLS_CC);
		wingui_control_static_parse_options(options, &style TSRMLS_CC);	
	}

	window_object = wingui_window_object_constructor("Static", NULL, x, y, width, height, style, extrastyle, parent, NULL, getThis(), wingui_control_static_proc_handler TSRMLS_CC);
}
/* }}} */

/* register Frame methods */
static zend_function_entry wingui_control_staticframe_functions[] = {
	PHP_ME(WinGuiControlFrame, __construct, WinGuiControlFrame___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Control\Frame/Text/Image Custom Object magic                                                 
------------------------------------------------------------------*/

/* {{{ wingui_control_statictext_new
       creates a new wingui_window_object and takes care of the internal junk */
zend_object_value wingui_control_statictext_new(zend_class_entry *ce TSRMLS_DC)
{
	return wingui_window_object_new(ce, &wingui_control_statictext_prop_handlers, &wingui_control_static_callback_map TSRMLS_CC);
}
/* }}} */

/* {{{ wingui_control_staticimage_new
       creates a new wingui_window_object and takes care of the internal junk */
zend_object_value wingui_control_staticimage_new(zend_class_entry *ce TSRMLS_DC)
{
	return wingui_window_object_new(ce, &wingui_control_staticimage_prop_handlers, &wingui_control_static_callback_map TSRMLS_CC);
}
/* }}} */


/* {{{ wingui_control_staticframe_new
       creates a new wingui_window_object and takes care of the internal junk */
zend_object_value wingui_control_staticframe_new(zend_class_entry *ce TSRMLS_DC)
{
	return wingui_window_object_new(ce, &wingui_control_staticframe_prop_handlers, &wingui_control_static_callback_map TSRMLS_CC);
}
/* }}} */

/* shared properties propogation */
void wingui_control_static_property_values(zval *object, int *style TSRMLS_DC)
{
	zval *member, *temp;

	MAKE_STD_ZVAL(member);
	
	ZVAL_STRING(member, "notify", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if(Z_BVAL_P(temp)) {
			*style |= SS_NOTIFY;
		}
	}
	ZVAL_STRING(member, "sunken", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(object, member, 0 TSRMLS_CC), IS_BOOL TSRMLS_CC);
		if(Z_BVAL_P(temp)) {
			*style |= SS_SUNKEN;
		}
	}
}
/* }}} */

/* shared options parser */
void wingui_control_static_parse_options(zval *options, int *style TSRMLS_DC)
{
	zval **value;

	if (zend_hash_find(Z_ARRVAL_P(options), "notify", sizeof("notify"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if(Z_BVAL_PP(value)) {
			*style |= SS_NOTIFY;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "sunken", sizeof("sunken"), (void **) &value) == SUCCESS) {
		*value = wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if(Z_BVAL_PP(value)) {
			*style |= SS_SUNKEN;
		}
	}
}
/* }}} */

/* turn rect to frames and off again */
int wingui_control_static_write_filled(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	long style;
	wingui_window_object *window = wingui_window_object_juggle(object);

	value = wingui_juggle_type(value, IS_BOOL TSRMLS_CC);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);

	if (Z_BVAL_P(value) == TRUE) {
		/* figure out our current color and change if we're frame not rect */
		if ((style & SS_WHITEFRAME) == SS_WHITEFRAME) {
			style &= ~SS_WHITEFRAME;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_GRAYFRAME;
			style &= ~SS_GRAYRECT;
			style &= ~SS_BLACKRECT;
			style |= SS_WHITERECT;
		} else if ((style & SS_GRAYFRAME) == SS_GRAYFRAME) {
			style &= ~SS_WHITEFRAME;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_GRAYFRAME;
			style &= ~SS_WHITERECT;
			style &= ~SS_BLACKRECT;
			style |= SS_GRAYRECT;
		} else if ((style & SS_BLACKFRAME) == SS_BLACKFRAME) {
			style &= ~SS_WHITEFRAME;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_GRAYFRAME;
			style &= ~SS_WHITERECT;
			style &= ~SS_GRAYRECT;
			style |= SS_BLACKRECT;
		} else {
			/* Otherwise, we're already set good */
			return SUCCESS;
		}
	} else {
		/* figure out our current color and change if we're rect not frame */
		if ((style & SS_WHITERECT) == SS_WHITERECT) {
			style |= SS_WHITEFRAME;
			style &= ~SS_WHITERECT;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_GRAYFRAME;
			style &= ~SS_BLACKRECT;
			style &= ~SS_GRAYRECT;
		} else if (style & SS_GRAYRECT) {
			style |= SS_GRAYFRAME;
			style &= ~SS_WHITEFRAME;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_BLACKRECT;
			style &= ~SS_WHITERECT;
			style &= ~SS_GRAYRECT;
		} else if ((style & SS_BLACKRECT) == SS_BLACKRECT) {
			style |= SS_BLACKFRAME;
			style &= ~SS_WHITEFRAME;
			style &= ~SS_GRAYFRAME;
			style &= ~SS_BLACKRECT;
			style &= ~SS_WHITERECT;
			style &= ~SS_GRAYRECT;
		} else {
		/* Otherwise, we're already set good */
			return SUCCESS;
		}
	}
	SetWindowLongPtr(window->window_handle, GWL_STYLE, style);
	SetWindowPos(window->window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return SUCCESS;
}
/* }}} */

/* Grabs current window style and tells whether it is filled or not */
int wingui_control_static_read_filled(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	long style;
	wingui_window_object *window = wingui_window_object_juggle(object);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);
	if (((style & SS_GRAYFRAME) == SS_GRAYFRAME) ||
		((style & SS_WHITEFRAME) == SS_WHITEFRAME) ||
		((style & SS_BLACKFRAME) == SS_BLACKFRAME) ||
		((style & SS_ETCHEDFRAME) == SS_ETCHEDFRAME) ||
		((style & SS_ETCHEDHORZ) == SS_ETCHEDHORZ) ||
		((style & SS_ETCHEDVERT) == SS_ETCHEDVERT)) {
		ZVAL_BOOL(*retval, 0);
	} else {
		ZVAL_BOOL(*retval, 1);
	}
	return SUCCESS;
}
/* }}} */

/* change color */
int wingui_control_static_write_color(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	long style;
	wingui_window_object *window = wingui_window_object_juggle(object);

	value = wingui_juggle_type(value, IS_LONG TSRMLS_CC);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);

	 if (Z_LVAL_P(value) == 6) {
		style &= ~SS_WHITEFRAME;
		style &= ~SS_BLACKFRAME;
		style &= ~SS_GRAYFRAME;
		style &= ~SS_WHITERECT;
		style &= ~SS_BLACKRECT;
		style &= ~SS_GRAYRECT;
		style &= ~SS_ETCHEDHORZ;
		style &= ~SS_ETCHEDFRAME;
		style |= SS_ETCHEDVERT;
	 } else if (Z_LVAL_P(value) == 5) {
		style &= ~SS_WHITEFRAME;
		style &= ~SS_BLACKFRAME;
		style &= ~SS_GRAYFRAME;
		style &= ~SS_WHITERECT;
		style &= ~SS_BLACKRECT;
		style &= ~SS_GRAYRECT;
		style &= ~SS_ETCHEDFRAME;
		style &= ~SS_ETCHEDVERT;
		style |= SS_ETCHEDHORZ;
	} else if (Z_LVAL_P(value) == 4) {
		/* we must be ETCHEDFRAME only */
		style &= ~SS_WHITEFRAME;
		style &= ~SS_BLACKFRAME;
		style &= ~SS_GRAYFRAME;
		style &= ~SS_WHITERECT;
		style &= ~SS_BLACKRECT;
		style &= ~SS_GRAYRECT;
		style &= ~SS_ETCHEDHORZ;
		style &= ~SS_ETCHEDVERT;
		style |= SS_ETCHEDFRAME;
	} else if (Z_LVAL_P(value) == 3) {
		/* if we are not gray, change to gray */
		if ((style & SS_WHITEFRAME) == SS_WHITEFRAME || (style & SS_BLACKFRAME) == SS_BLACKFRAME) {
			style &= ~SS_WHITEFRAME;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_GRAYRECT;
			style &= ~SS_WHITERECT;
			style &= ~SS_BLACKRECT;
			style &= ~SS_ETCHEDHORZ;
			style &= ~SS_ETCHEDVERT;
			style &= ~SS_ETCHEDFRAME;
			style |= SS_GRAYFRAME;
		} else {
			style &= ~SS_WHITEFRAME;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_GRAYFRAME;
			style &= ~SS_WHITERECT;
			style &= ~SS_BLACKRECT;
			style &= ~SS_ETCHEDHORZ;
			style &= ~SS_ETCHEDVERT;
			style &= ~SS_ETCHEDFRAME;
			style |= SS_GRAYRECT;
		}
	} else if (Z_LVAL_P(value) == 2) {
		/* if we are not white, change to white */
		if ((style & SS_GRAYFRAME) == SS_GRAYFRAME || (style & SS_BLACKFRAME) == SS_BLACKFRAME) {
			style &= ~SS_GRAYFRAME;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_GRAYRECT;
			style &= ~SS_WHITERECT;
			style &= ~SS_BLACKRECT;
			style &= ~SS_ETCHEDHORZ;
			style &= ~SS_ETCHEDVERT;
			style &= ~SS_ETCHEDFRAME;
			style |= SS_WHITEFRAME;
		} else {
			style &= ~SS_WHITEFRAME;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_GRAYFRAME;
			style &= ~SS_BLACKRECT;
			style &= ~SS_GRAYRECT;
			style &= ~SS_ETCHEDHORZ;
			style &= ~SS_ETCHEDVERT;
			style &= ~SS_ETCHEDFRAME;
			style |= SS_WHITERECT;
		}
	} else {
		/* if we are not black, change to black */
		if ((style & SS_GRAYFRAME) == SS_GRAYFRAME || (style & SS_WHITEFRAME) == SS_WHITEFRAME) {
			style &= ~SS_GRAYFRAME;
			style &= ~SS_WHITEFRAME;
			style &= ~SS_GRAYRECT;
			style &= ~SS_WHITERECT;
			style &= ~SS_BLACKRECT;
			style &= ~SS_ETCHEDHORZ;
			style &= ~SS_ETCHEDVERT;
			style &= ~SS_ETCHEDFRAME;
			style |= SS_BLACKFRAME;
		} else {
			style &= ~SS_WHITEFRAME;
			style &= ~SS_BLACKFRAME;
			style &= ~SS_GRAYFRAME;
			style &= ~SS_WHITERECT;
			style &= ~SS_GRAYRECT;
			style &= ~SS_ETCHEDHORZ;
			style &= ~SS_ETCHEDVERT;
			style &= ~SS_ETCHEDFRAME;
			style |= SS_BLACKRECT;
		}
	}
	SetWindowLongPtr(window->window_handle, GWL_STYLE, style);
	SetWindowPos(window->window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return SUCCESS;
}
/* }}} */

/* Grabs current window style and tells if it's black (frame) gray (window) or white(background) */
int wingui_control_static_read_color(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	long style;
	wingui_window_object *window = wingui_window_object_juggle(object);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);
	if ((style & SS_ETCHEDFRAME) == SS_ETCHEDFRAME) {
		ZVAL_LONG(*retval, 4); /* must be before etchedhorz */
	} else if ((style & SS_ETCHEDVERT) == SS_ETCHEDVERT) {
		ZVAL_LONG(*retval, 6);
	} else if ((style & SS_ETCHEDHORZ) == SS_ETCHEDHORZ) {
		ZVAL_LONG(*retval, 5);
	/* These must retain this order for the check */
	} else if ((style & SS_WHITEFRAME) == SS_WHITEFRAME) { /* Must be before grayframe check */
		ZVAL_LONG(*retval, 2);
	} else if ((style & SS_GRAYFRAME) == SS_GRAYFRAME) {
		ZVAL_LONG(*retval, 3);
	} else if ((style & SS_BLACKFRAME) == SS_BLACKFRAME) { /* must come before blackrect check */
		ZVAL_LONG(*retval, 1);
	} else if ((style & SS_WHITERECT) == SS_WHITERECT) { /* must come before blackrect check */
		ZVAL_LONG(*retval, 2);
	} else if ((style & SS_GRAYRECT) == SS_GRAYRECT) { /* must come before blackrect check */
		ZVAL_LONG(*retval, 3);
	} else if ((style & SS_BLACKRECT) == SS_BLACKRECT) {
		ZVAL_LONG(*retval, 1);
	} else {
		return FAILURE;
	}
	return SUCCESS;
}
/* }}} */

/* turn SS_NOTIFY on and off, notice we don't bother "moving" the window, since this is not a cosmetic change */
int wingui_control_static_write_notify(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	long style;
	wingui_window_object *window = wingui_window_object_juggle(object);

	value = wingui_juggle_type(value, IS_BOOL TSRMLS_CC);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);

	if (Z_BVAL_P(value) == TRUE) {
		style |= SS_NOTIFY;
	} else {
		style &= ~SS_NOTIFY;
	}
	SetWindowLongPtr(window->window_handle, GWL_STYLE, style);
	return SUCCESS;
}
/* }}} */

/* Grabs current window style and tells whether SS_NOTIFY is on or off */
int wingui_control_static_read_notify(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	long style;
	wingui_window_object *window = wingui_window_object_juggle(object);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);
	if (((style & SS_NOTIFY) == SS_NOTIFY)) {
		ZVAL_BOOL(*retval, 1);
	} else {
		ZVAL_BOOL(*retval, 0);
	}
	return SUCCESS;
}
/* }}} */


/* turn SS_SUNKEN on and off, this changes frame so we do a windowpos */
int wingui_control_static_write_sunken(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	long style;
	wingui_window_object *window = wingui_window_object_juggle(object);

	value = wingui_juggle_type(value, IS_BOOL TSRMLS_CC);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);

	if (Z_BVAL_P(value) == TRUE) {
		style |= SS_SUNKEN;
	} else {
		style &= ~SS_SUNKEN;
	}
	SetWindowLongPtr(window->window_handle, GWL_STYLE, style);
	SetWindowPos(window->window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return SUCCESS;
}
/* }}} */

/* Grabs current window style and tells whether SS_SUNKEN is on or off */
int wingui_control_static_read_sunken(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	long style;
	wingui_window_object *window = wingui_window_object_juggle(object);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);
	if (((style & SS_SUNKEN) == SS_SUNKEN)) {
		ZVAL_BOOL(*retval, 1);
	} else {
		ZVAL_BOOL(*retval, 0);
	}
	return SUCCESS;
}
/* }}} */

/* generic Proc handler for windows - will call any on{Message} methods available, and any registered callbacks per message */
LRESULT CALLBACK wingui_control_static_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
zval ***extra = NULL;
	int extra_count = 0;
	zend_bool stop_default = FALSE;
	zval *return_value = NULL;
#ifdef ZTS
	TSRMLS_D;
#endif;

	wingui_window_object *window_object = (wingui_window_object *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
#ifdef ZTS
	TSRMLS_C = window_object->TSRMLS_C;
#endif

	extra = wingui_control_static_messages_cracker(msg, &wParam, &lParam, &extra_count TSRMLS_CC);

	if(window_object) {
		return_value = wingui_winproc_callback_dispatch(window_object, msg, extra, extra_count, &stop_default TSRMLS_CC);
	}
	
	if (stop_default == FALSE) {
		wingui_control_static_messages_packer(msg, &wParam, &lParam, extra TSRMLS_CC);
	}

	if (extra) {
		wingui_callback_extra_zvals_dtor(extra_count, extra);
	}

	/* return is based on if we need to call the default proc handling, and if we have an overwritten
	   return value to use instead of the default value */
	if (stop_default && return_value) {
		return wingui_control_static_messages_results(msg, return_value TSRMLS_CC);
	} else if (stop_default) {
		return (LRESULT) NULL;
	} else if (return_value) {
		CallWindowProc(window_object->window_proc, hwnd, msg, wParam, lParam);
		return wingui_control_static_messages_results(msg, return_value TSRMLS_CC);
	} else {
		return CallWindowProc(window_object->window_proc, hwnd, msg, wParam, lParam);
	}
}
/* }}} */

/* put lparams and wparams into zvals for progressbar messages */
zval ***wingui_control_static_messages_cracker(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC)
{
	zval ***extra = NULL;

	switch(msg) {
		default:
			extra = wingui_window_messages_cracker(msg, wParam, lParam, extra_count TSRMLS_CC);
	}
	return extra;
}
/* }}} */

/* put cracked values into lparams, wparams before calling defaultproc */
void wingui_control_static_messages_packer(int msg, WPARAM *wParam, LPARAM *lParam, zval ***extra TSRMLS_DC)
{
	zval **value = NULL;

	switch(msg) {
		default:
			wingui_window_messages_packer(msg, wParam, lParam, extra TSRMLS_CC);
	}
}
/* }}} */

/* put zval return values from from messages into lresult vals */
LRESULT wingui_control_static_messages_results(int msg, zval *return_value TSRMLS_DC)
{
	switch(msg) {
		default:
			return wingui_window_messages_results(msg, return_value TSRMLS_CC);
	}
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Control\Text/Frame/Image LifeCycle Functions
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_control_static)
{
	zend_class_entry ce, image_ce, frame_ce;
	// all static controls can have SS_SUNKEN giving it a "sunk in" edge

	//SS_CENTER is for text/label only
	// SS_CENTERIMAGE is for image only
	// SS_EDITCONTROL is text only
	//SS_ENDELLIPSIS is text only


	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS_NAME("Control", "Text"), wingui_control_statictext_functions);
	ce_wingui_control_statictext = zend_register_internal_class_ex(&ce, ce_wingui_control, ZEND_NS_NAME(PHP_WINGUI_NS, "Control") TSRMLS_CC);
	ce_wingui_control_statictext->create_object  = wingui_control_statictext_new;

	INIT_NS_CLASS_ENTRY(image_ce, PHP_WINGUI_NS, ZEND_NS_NAME("Control", "Image"), wingui_control_staticimage_functions);
	ce_wingui_control_staticimage = zend_register_internal_class_ex(&image_ce, ce_wingui_control, ZEND_NS_NAME(PHP_WINGUI_NS, "Control") TSRMLS_CC);
	ce_wingui_control_staticimage->create_object  = wingui_control_staticimage_new;

	INIT_NS_CLASS_ENTRY(frame_ce, PHP_WINGUI_NS, ZEND_NS_NAME("Control", "Frame"), wingui_control_staticframe_functions);
	ce_wingui_control_staticframe = zend_register_internal_class_ex(&frame_ce, ce_wingui_control, ZEND_NS_NAME(PHP_WINGUI_NS, "Control") TSRMLS_CC);
	ce_wingui_control_staticframe->create_object  = wingui_control_staticframe_new;

	/* Color Constants */
	zend_declare_class_constant_long(ce_wingui_control_staticframe, "FRAME_COLOR", sizeof("FRAME_COLOR") - 1, 1 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_control_staticframe, "WINDOW_COLOR", sizeof("WINDOW_COLOR") - 1, 2 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_control_staticframe, "BACKGROUND_COLOR", sizeof("BACKGROUND_COLOR") - 1, 3 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_control_staticframe, "ETCHED", sizeof("ETCHED") - 1, 4 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_control_staticframe, "ETCHED_HORZ", sizeof("ETCHED_HORZ") - 1, 5 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_control_staticframe, "ETCHED_VERT", sizeof("ETCHED_VERT") - 1, 6 TSRMLS_CC);

	/* Callback map for all static controls */
	zend_hash_init(&wingui_control_static_callback_map, 0, NULL, NULL, 1);
	zend_hash_merge(&wingui_control_static_callback_map, &wingui_control_callback_map, NULL, NULL, sizeof(char *), 0);

	/* Callback map for properties for label */
	zend_hash_init(&wingui_control_statictext_prop_handlers, 0, NULL, NULL, 1);
	wingui_register_prop_handler(&wingui_control_statictext_prop_handlers, ce_wingui_control_statictext, "sunken", wingui_control_static_read_sunken, wingui_control_static_write_sunken TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_statictext_prop_handlers, ce_wingui_control_statictext, "notify", wingui_control_static_read_notify, wingui_control_static_write_notify TSRMLS_CC);
	zend_hash_merge(&wingui_control_statictext_prop_handlers, &wingui_control_prop_handlers, NULL, NULL, sizeof(wingui_prop_handler), 0);
	
	/* Callback map for properties for image */
	zend_hash_init(&wingui_control_staticimage_prop_handlers, 0, NULL, NULL, 1);
	wingui_register_prop_handler(&wingui_control_staticimage_prop_handlers, ce_wingui_control_staticimage, "sunken", wingui_control_static_read_sunken, wingui_control_static_write_sunken TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_staticimage_prop_handlers, ce_wingui_control_staticimage, "notify", wingui_control_static_read_notify, wingui_control_static_write_notify TSRMLS_CC);
	zend_hash_merge(&wingui_control_staticimage_prop_handlers, &wingui_control_prop_handlers, NULL, NULL, sizeof(wingui_prop_handler), 0);

	/* Callback map for properties for frame */
	zend_hash_init(&wingui_control_staticframe_prop_handlers, 0, NULL, NULL, 1);
	wingui_register_prop_handler(&wingui_control_staticframe_prop_handlers, ce_wingui_control_staticframe, "sunken", wingui_control_static_read_sunken, wingui_control_static_write_sunken TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_staticframe_prop_handlers, ce_wingui_control_staticframe, "notify", wingui_control_static_read_notify, wingui_control_static_write_notify TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_staticframe_prop_handlers, ce_wingui_control_staticframe, "filled", wingui_control_static_read_filled, wingui_control_static_write_filled TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_staticframe_prop_handlers, ce_wingui_control_staticframe, "color", wingui_control_static_read_color, wingui_control_static_write_color TSRMLS_CC);
	zend_hash_merge(&wingui_control_staticframe_prop_handlers, &wingui_control_prop_handlers, NULL, NULL, sizeof(wingui_prop_handler), 0);
	
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
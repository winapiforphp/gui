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
zend_class_entry *ce_wingui_control_statusbar;

/* Custom Object Handler Items */
HashTable wingui_control_statusbar_callback_map;
HashTable wingui_control_statusbar_prop_handlers;

/* C API used here */
zval ***wingui_control_statusbar_messages_cracker(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC);
void wingui_control_statusbar_messages_packer(int msg, WPARAM *wParam, LPARAM *lParam, zval ***extra TSRMLS_DC);
LRESULT wingui_control_statusbar_messages_results(int msg, zval *return_value TSRMLS_DC);
void wingui_control_statusbar_messages_zvals(int msg, LRESULT lresult, zval *return_value TSRMLS_DC);
int wingui_control_statusbar_messages_dispatch(INTERNAL_FUNCTION_PARAMETERS, int msg, WPARAM *wParam, LPARAM *lParam);

static inline long wingui_control_statusbar_checkparts(long part, HWND statusbar_handle, int defaultid) {
	long part_count;

	/* autodetect for simple statusbar */
	if (part == 0 && SendMessage(statusbar_handle, (UINT) SB_ISSIMPLE, (WPARAM) 0, (LPARAM) 0) > 0) {
		return defaultid;
	}

	/* get number of parts and max it */
	part_count = SendMessage(statusbar_handle, (UINT) SB_GETPARTS, (WPARAM) 0, (LPARAM) 0) - 1;

	return MAX(0, MIN(part, part_count));
}

/* ----------------------------------------------------------------
  Win\Gui\Control\Statusbar Userland API                                                      
------------------------------------------------------------------*/

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_drawText_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 3)
	ZEND_ARG_INFO(0, windowing)
	ZEND_ARG_INFO(0, rect)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlStatusbar_getBorders_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_getIcon_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, part)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlStatusbar_getParts_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_getRect_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, part)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_getText_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, part)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_getTextLength_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, part)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_getTipText_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, part)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlStatusbar_getUnicodeFormat_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlStatusbar_isSimple_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_setBkColor_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, color_or_red)
	ZEND_ARG_INFO(0, green)
	ZEND_ARG_INFO(0, blue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_setIcon_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, icon)
	ZEND_ARG_INFO(0, part)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlStatusbar_setMinHeight_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlStatusbar_setParts_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, parts)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_setText_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, part)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlStatusbar_setTipText_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, part)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlStatusbar_setUnicodeFormat_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlStatusbar_setSimple_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, simple)
ZEND_END_ARG_INFO()

/* {{{ __construct */
PHP_METHOD(WinGuiStatusbar, __construct)
{
	zend_error_handling error_handling;
	HWND hwnd, parent_handle = NULL;
	wingui_window_object *parent_obj;
	wingui_window_object *status_obj = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	int x = 0, y = 0, height = 0, width = 0, style = WS_CHILD, id = 0, extrastyle = 0;
	char * text = "";
	zval *parent, *options = NULL, *member, *temp, **value, *object = getThis(); 

/*
	/* Populate generic window data from properties 
	//if(wingui_generic_property_values(object, &x, &y, &height, &width, &text, &style, &extrastyle TSRMLS_CC) == FAILURE) {
	//	return;
	//}
	/* Populate generic control data from properties 
	//if(wingui_control_property_values(object, &style TSRMLS_CC) == FAILURE) {
	//	return;
	//}

	/* statusbar only property population - id, sizegrip, tooltips */
	MAKE_STD_ZVAL(member);
	ZVAL_STRING(member, "id", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_LONG TSRMLS_CC);
		id = Z_LVAL_P(temp);
	}
	ZVAL_STRING(member, "tooltips", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp) == TRUE) {
			style |= SBARS_TOOLTIPS | SBT_TOOLTIPS;
		}
	}
	ZVAL_STRING(member, "sizegrip", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp) == TRUE) {
			style |= SBARS_SIZEGRIP;
		}
	}

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

	if(options && Z_TYPE_P(options) == IS_ARRAY) {
		/* Parse generic window options 
		if(wingui_generic_parse_options(options, &x, &y, &height, &width, &text, &style, &extrastyle TSRMLS_CC) == FAILURE) {
			return;
		}

		/* Parse generic statusbar options 
		if(wingui_control_parse_options(options, &style TSRMLS_CC) == FAILURE) {
			return;
		}

		/* Custom items - id, sizegrip, tooltips */
		if (zend_hash_find(Z_ARRVAL_P(options), "id", sizeof("id"), (void **) &value) == SUCCESS) {
			wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
			id = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "tooltips", sizeof("tooltips"), (void **) &value) == SUCCESS) {
			wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
			if (Z_BVAL_PP(value) == TRUE) {
				style |= SBARS_TOOLTIPS | SBT_TOOLTIPS;
			}
		}
		if (zend_hash_find(Z_ARRVAL_P(options), "sizegrip", sizeof("sizegrip"), (void **) &value) == SUCCESS) {
			wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
			if (Z_BVAL_PP(value) == TRUE) {
				style |= SBARS_SIZEGRIP;
			}
		}
	}

	hwnd = CreateWindowEx(
		extrastyle,
        STATUSCLASSNAME, 
        text,   
        style, 
        x,  
        y,  
        width, 
        height,
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

/* {{{ proto void Win\Gui\Control\Statusbar::drawText(Win\Gui\Windowing object, array rect, string text [, int flags])
	draws the specified text in the style of a status window with border */
PHP_METHOD(WinGuiStatusbar, drawText)
{
	zend_error_handling error_handling;
	char * string;
	int string_length;
	long flags = 0;
	wingui_window_object *window_obj;
	zval *window, *rect_data = NULL;
	RECT rect;
	HDC dc;

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Oas|l", &window, ce_wingui_windowing, &rect_data, &string, &string_length, &flags) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	window_obj = (wingui_window_object*) wingui_window_object_get(window TSRMLS_CC);

	/* Find items in assoc array and put inside struct */
	rect.top = 0;
	rect.bottom = 0;
	rect.right = 0;
	rect.left = 0;

	if (rect_data) {
		HashTable *ht = Z_ARRVAL_P(rect_data);
		zval **tmp;

		if (zend_hash_find(ht, "top", sizeof("top"), (void**)&tmp) == SUCCESS &&
			wingui_juggle_type(*tmp, IS_LONG TSRMLS_CC) == SUCCESS) {
				rect.top = Z_LVAL_PP(tmp);
		}
		if (zend_hash_find(ht, "bottom", sizeof("bottom"), (void**)&tmp) == SUCCESS &&
			wingui_juggle_type(*tmp, IS_LONG TSRMLS_CC) == SUCCESS) {
				rect.bottom = Z_LVAL_PP(tmp);
		}
		if (zend_hash_find(ht, "left", sizeof("left"), (void**)&tmp) == SUCCESS &&
			wingui_juggle_type(*tmp, IS_LONG TSRMLS_CC) == SUCCESS) {
				rect.left = Z_LVAL_PP(tmp);
		}
		if (zend_hash_find(ht, "right", sizeof("right"), (void**)&tmp) == SUCCESS &&
			wingui_juggle_type(*tmp, IS_LONG TSRMLS_CC) == SUCCESS) {
				rect.bottom = Z_LVAL_PP(tmp);
		}
	}
	dc = GetDC(window_obj->window_handle);

	DrawStatusText(dc, &rect, (LPCTSTR) string, flags);

	ReleaseDC(window_obj->window_handle, dc);
}
/* }}} */

/* {{{ proto array Win\Gui\Control\Statusbar->getBorders()
	returns an array with vertical, horizontal, and border sizes */
PHP_METHOD(WinGuiStatusbar, getBorders)
{
	zend_error_handling error_handling;
	int border_array [3];

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (SendMessage(statusbar_obj->window_handle, (UINT) SB_GETBORDERS, (WPARAM) 0, (LPARAM) (LPINT) &border_array)) {
		array_init(return_value);
		add_next_index_long(return_value, border_array[0]);
		add_next_index_long(return_value, border_array[1]);
		add_next_index_long(return_value, border_array[2]);
	}
}
/* }}} */

/* {{{ proto object Win\Gui\Control\Statusbar->getIcon()
	returns an icon object associated with the statusbar or null
	note - this will not return the */
PHP_METHOD(WinGuiStatusbar, getIcon)
{
	zend_error_handling error_handling;
    long part = 0, position = 0;
	HICON icon_handle;
	wingui_resource_object *icon_object;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &part) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	part = wingui_control_statusbar_checkparts(part, statusbar_obj->window_handle, -1);
	if (part == -1) {
		position = 255;
	} else {
		position = part;
	}

	/* let's actually get the icon handle */
	icon_handle = (HICON) SendMessage(statusbar_obj->window_handle, (UINT) SB_GETICON, (WPARAM) part, (LPARAM) 0);

	/* if icon_handle is null, then we're all good */
	if(!icon_handle) {
		return;
	}

	/* if we have an icon handle, and we have a stored icon object, and their handles match, return */
	if (statusbar_obj->data.statusbar.icon_zval[position]) {
		icon_object = (wingui_resource_object *)zend_object_store_get_object(statusbar_obj->data.statusbar.icon_zval[position] TSRMLS_CC);
		if(icon_object->resource_handle == icon_handle) {
			MAKE_COPY_ZVAL(&statusbar_obj->data.statusbar.icon_zval[position], return_value)
			return;
		}
	}

	/* we either don't have an icon stored properly, or the handles didn't match */
	object_init_ex(return_value, ce_wingui_resource_icon);
 	icon_object = (wingui_resource_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	icon_object->resource_handle = CopyIcon((HICON)icon_handle);
}
/* }}} */

/* {{{ proto array Win\Gui\Control\Statusbar->getParts()
	returns an array with id corresponding to part offset coordinates
	A value of -1 in the final coordinate means the last part will expand rightwards to fill the statusbar */
PHP_METHOD(WinGuiStatusbar, getParts)
{
	zend_error_handling error_handling;
	int width_array [256];
    int count, i;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	count = (int) SendMessage(statusbar_obj->window_handle, SB_GETPARTS, (WPARAM) 256, (LPARAM) width_array);
	array_init(return_value);
	for(i = 0; i < count; i++) {
        add_next_index_long(return_value, width_array[i]);
    }
}
/* }}} */

/* {{{ proto array Win\Gui\Control\Statusbar->getRect([int part])
	Retrieves the bounding rectangle of a part in a status window
	array returns left, top, right, bottom associative
	If call fails, returns false */
PHP_METHOD(WinGuiStatusbar, getRect)
{
	zend_error_handling error_handling;
    long part = 0;
	RECT rect;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &part) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	part = wingui_control_statusbar_checkparts(part, statusbar_obj->window_handle, SB_SIMPLEID);

	if (SendMessage(statusbar_obj->window_handle, SB_GETRECT, (WPARAM) part, (LPARAM) &rect)) {

		array_init(return_value);
		add_assoc_long(return_value, "left", rect.left);
		add_assoc_long(return_value, "right", rect.right);
		add_assoc_long(return_value, "bottom", rect.bottom);
		add_assoc_long(return_value, "top", rect.top);
	} else {
		RETURN_FALSE
	}

}
/* }}} */

/* {{{ proto string Win\Gui\Control\Statusbar->getText([int part])
	Retrieves the text from the specified part of a status window */
PHP_METHOD(WinGuiStatusbar, getText)
{
	zend_error_handling error_handling;
    long part = 0;
	int textlength;
	char * string;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &part) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	part = wingui_control_statusbar_checkparts(part, statusbar_obj->window_handle, 0);

	textlength = LOWORD((DWORD) SendMessage(statusbar_obj->window_handle, SB_GETTEXTLENGTH, (WPARAM) part, (LPARAM) 0));

	string = emalloc(textlength);

	textlength = SendMessage(statusbar_obj->window_handle, SB_GETTEXT, (WPARAM) part, (LPARAM) string);

	RETVAL_STRINGL(string, textlength, 1);
	efree(string);
}
/* }}} */

/* {{{ proto array Win\Gui\Control\Statusbar->getTextLength([int part])
	Retrieves the text length from the specified part of a status window
	In array context, returns an array (LENGTH,STYLE) */
PHP_METHOD(WinGuiStatusbar, getTextLength)
{
	zend_error_handling error_handling;
    long part = 0;
	DWORD textlength;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &part) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	part = wingui_control_statusbar_checkparts(part, statusbar_obj->window_handle, 0);

	textlength = (DWORD) SendMessage(statusbar_obj->window_handle, SB_GETTEXTLENGTH, (WPARAM) part, (LPARAM) 0);

	array_init(return_value);
	add_next_index_long(return_value, (short) LOWORD(textlength));
	add_next_index_long(return_value, (short) HIWORD(textlength));
}
/* }}} */

/* {{{ proto string Win\Gui\Control\Statusbar->getTipText([int part])
	Retrieves the tooltip text for a part in a status bar, the tooltips flag must be on for the statusbar */
PHP_METHOD(WinGuiStatusbar, getTipText)
{
	zend_error_handling error_handling;
    long part = 0;
	char * string;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &part) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	part = wingui_control_statusbar_checkparts(part, statusbar_obj->window_handle, SB_SIMPLEID);

	string = emalloc(statusbar_obj->data.statusbar.tip_text_len[part]);

	SendMessage(statusbar_obj->window_handle, SB_GETTIPTEXT, (WPARAM) MAKEWPARAM (part, statusbar_obj->data.statusbar.tip_text_len[part]),
		(LPARAM) string);

	RETVAL_STRINGL(string, statusbar_obj->data.statusbar.tip_text_len[part], 1);
	efree(string);
}
/* }}} */

/* {{{ proto bool Win\Gui\Control\Statusbar->getUnicodeFormat()
	Retrieves the Unicode character format flag */
PHP_METHOD(WinGuiStatusbar, getUnicodeFormat)
{
	zend_error_handling error_handling;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SendMessage((HWND) statusbar_obj->window_handle, (UINT) SB_GETUNICODEFORMAT,
		(WPARAM) 0, (LPARAM) 0));
}
/* }}} */

/* {{{ proto bool Win\Gui\Control\Statusbar->isSimple()
	is the status bar in simple mode */
PHP_METHOD(WinGuiStatusbar, isSimple)
{
	zend_error_handling error_handling;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SendMessage((HWND) statusbar_obj->window_handle, (UINT) SB_ISSIMPLE,
		(WPARAM) 0, (LPARAM) 0));
}
/* }}} */

/* {{{ proto int Win\Gui\Control\Statusbar->setBkColor(int red, int blue, int green)
       proto int Win\Gui\Control\Statusbar->setBkColor(string hex color)
	   proto int Win\Gui\Control\Statusbar->setBkColor(int hex color)
	  sets a background color in RGB format or hex string or hex integer 
	  if nothing is passed, the color is set to default */
PHP_METHOD(WinGuiStatusbar, setBkColor)
{
	zend_error_handling error_handling;
	zval *colval = NULL;
	long red=0, green=0, blue=0;
	long hex;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|zll", &colval, &green, &blue) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if(colval) {
		switch(Z_TYPE_P(colval)) {
			case IS_STRING:
				if(Z_STRLEN_P(colval) == 7) {
					char hash;
					sscanf_s(Z_STRVAL_P(colval),"%1c%2x%2x%2x",  &hash, sizeof(char), &red, &green, &blue);
				} else if(Z_STRLEN_P(colval) == 6) {
					sscanf_s(Z_STRVAL_P(colval),"%2x%2x%2x", &red, &green, &blue);
				} else if(Z_STRLEN_P(colval) == 4) {
					char hash;
					sscanf_s(Z_STRVAL_P(colval),"%1c%1x%1x%1x", &hash, sizeof(char), &red, &green, &blue);
					red <<= 8;
					green <<= 8;
					blue <<= 8;
				} else if (Z_STRLEN_P(colval) == 3) {
					sscanf_s(Z_STRVAL_P(colval), "%1x%1x%1x", &red, &green, &blue);
					red <<= 8;
					green <<= 8;
					blue <<= 8;
				} else {
					zend_throw_exception(ce_wingui_argexception, "Win\\Gui\\Control\\Statusbar::setBkColor() expects parameter 1 string in RGB or RRGGBB format, with our without a # prefix", 0 TSRMLS_CC);
				}
			break;
			case IS_LONG:
				if(ZEND_NUM_ARGS() > 1) {
					red = Z_LVAL_P(colval);
				} else {
					red = GetRValue(Z_LVAL_P(colval));
					green = GetGValue(Z_LVAL_P(colval));
					blue = GetBValue(Z_LVAL_P(colval));
				}
			break;
			default:
				zend_throw_exception(ce_wingui_argexception, "Win\\Gui\\Control\\Statusbar::setBkColor() expects parameter 1 to be long or parseable hex string", 0 TSRMLS_CC);

		}
		/* rgb sanity checking */
		red = MAX(0,MIN(255,red));
		green = MAX(0,MIN(255,green));
		blue = MAX(0,MIN(255,blue));
		hex = RGB(red,  green,  blue);
	} else {
		hex = CLR_DEFAULT;
	}

	hex = SendMessage((HWND) statusbar_obj->window_handle, (UINT) SB_SETBKCOLOR,
		(WPARAM) 0, (LPARAM) hex);

	if (hex != CLR_DEFAULT) {
		RETURN_LONG(hex)
	}
}
/* }}} */

/* {{{ proto bool Win\Gui\Control\Statusbar->setIcon(Win\Gui\Resource\Icon icon[, int part])
	Sets an icon object as the image for a statusbar */
PHP_METHOD(WinGuiStatusbar, setIcon)
{
	zend_error_handling error_handling;
	zval *icon = NULL;
	HICON icon_resource = NULL;
	long part = 0, position = 0;
	wingui_resource_object *icon_object;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O!|l", &icon, ce_wingui_resource_icon, &part) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* stupid thing wants -1 not SB_SIMPLEID just to REALLY drive me crazy */
	part = wingui_control_statusbar_checkparts(part, statusbar_obj->window_handle, -1);
	
	if (part == -1) {
		position = 255;
	} else {
		position = part;
	}

	/* if icon is not null, grab the handle */
	if (icon != NULL) {
		icon_object = (wingui_resource_object *)wingui_resource_object_get(icon TSRMLS_CC);
		icon_resource = icon_object->resource_handle;
	}

	/* do we have an icon zval stored */
	if (statusbar_obj->data.statusbar.icon_zval && statusbar_obj->data.statusbar.icon_zval[position]) {
		Z_DELREF_P(statusbar_obj->data.statusbar.icon_zval[position]);
	}
	statusbar_obj->data.statusbar.icon_zval[part] = NULL;

	RETVAL_BOOL(SendMessage((HWND) statusbar_obj->window_handle, (UINT) SB_SETICON,
		(WPARAM) part, (LPARAM) icon_resource));

	/* do we have an icon zval stored */
	if (icon != NULL) {
		statusbar_obj->data.statusbar.icon_zval[position] = icon;
		Z_ADDREF_P(statusbar_obj->data.statusbar.icon_zval[position]);
	}
}
/* }}} */

/* {{{ proto void Win\Gui\Control\Statusbar->setMinHeight(int height)
	Sets the minimum height of a status window's drawing area */
PHP_METHOD(WinGuiStatusbar, setMinHeight)
{
	zend_error_handling error_handling;
	long height = 0;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &height) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	SendMessage((HWND) statusbar_obj->window_handle, (UINT) SB_SETMINHEIGHT,
		(WPARAM) height, (LPARAM) 0);
}
/* }}} */

/* {{{ proto bool Win\Gui\Control\Statusbar->setParts(array parts)
	send an array with part to offset coordinates
	Sets the number of parts in a statusbar and the coordinate of the right edge of each part */
PHP_METHOD(WinGuiStatusbar, setParts)
{
	zend_error_handling error_handling;
	int width_array [256];
	int i = 0;
	HashTable *arr_hash;
    HashPosition pointer;
	zval *ints_array, **data; 

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &ints_array) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	arr_hash = Z_ARRVAL_P(ints_array);

	for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer);
		zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS;
		zend_hash_move_forward_ex(arr_hash, &pointer)) {

		if (wingui_juggle_type(*data, IS_LONG TSRMLS_CC) == SUCCESS) {
			width_array[i] = Z_LVAL_PP(data);
			i++;
		} else {
			zend_throw_exception(ce_wingui_argexception, "Array should hold numeric widths for parts", 0 TSRMLS_CC);
			return;
		}
		if (i == 256)
			break;
    }

	if (SendMessage(statusbar_obj->window_handle, SB_SETPARTS, (WPARAM) i, (LPARAM) width_array)) {
		RETURN_TRUE
	} else {
		RETURN_FALSE
	}

}
/* }}} */

/* {{{ proto bool Win\Gui\Control\Statusbar->setText(string text [, int part, int type])
	sets the text in the specified part of a status window */
PHP_METHOD(WinGuiStatusbar, setText)
{
	zend_error_handling error_handling;
	char *string;
	long part = 0, type = 0;
	int str_len;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &string, &str_len, &part, &type) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	part = wingui_control_statusbar_checkparts(part, statusbar_obj->window_handle, SB_SIMPLEID);

	if (SendMessage(statusbar_obj->window_handle, SB_SETTEXT, (WPARAM) part | type, (LPARAM) string)) {
		RETURN_TRUE
	} else {
		RETURN_FALSE
	}

}
/* }}} */

/* {{{ proto void Win\Gui\Control\Statusbar->setTipText(string text [, int part])
	sets tooltip text for a statusbar - only works if the tooltip flag is on for the statusbar */
PHP_METHOD(WinGuiStatusbar, setTipText)
{
	zend_error_handling error_handling;
	char *string;
	long part = 0;
	int str_len;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &string, &str_len, &part) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	part = wingui_control_statusbar_checkparts(part, statusbar_obj->window_handle, SB_SIMPLEID);

	SendMessage(statusbar_obj->window_handle, SB_SETTIPTEXT, (WPARAM) part, (LPARAM) string);
	statusbar_obj->data.statusbar.tip_text_len[part] = str_len;
}
/* }}} */

/* {{{ proto bool Win\Gui\Control\Statusbar->setUnicodeFormat(bool format)
	sets the unicode character format flag */
PHP_METHOD(WinGuiStatusbar, setUnicodeFormat)
{
	zend_error_handling error_handling;
	zend_bool format = 0;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &format) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SendMessage((HWND) statusbar_obj->window_handle, (UINT) SB_SETUNICODEFORMAT,
		(WPARAM) format, (LPARAM) 0));
}
/* }}} */

/* {{{ proto bool Win\Gui\Control\Statusbar->setSimple([bool simple])
	set the statubar to simple mode and return new status */
PHP_METHOD(WinGuiStatusbar, setSimple)
{
	zend_error_handling error_handling;
	zend_bool simple = 0;

	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &simple) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	SendMessage((HWND) statusbar_obj->window_handle, (UINT) SB_SIMPLE,
		(WPARAM) (BOOL) simple, (LPARAM) 0);

	RETURN_BOOL(SendMessage((HWND) statusbar_obj->window_handle, (UINT) SB_ISSIMPLE,
		(WPARAM) 0, (LPARAM) 0));
}
/* }}} */

/* register statusbar methods */
static zend_function_entry wingui_control_statusbar_functions[] = {
	PHP_ME(WinGuiStatusbar, __construct, WinGuiControlStatusbar___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiStatusbar, drawText, WinGuiControlStatusbar_drawText_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiStatusbar, getBorders, WinGuiControlStatusbar_getBorders_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, getIcon, WinGuiControlStatusbar_getIcon_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, getParts, WinGuiControlStatusbar_getParts_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, getRect, WinGuiControlStatusbar_getRect_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, getText, WinGuiControlStatusbar_getText_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, getTextLength, WinGuiControlStatusbar_getTextLength_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, getTipText, WinGuiControlStatusbar_getText_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, getUnicodeFormat, WinGuiControlStatusbar_getUnicodeFormat_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, isSimple, WinGuiControlStatusbar_isSimple_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, setBkColor, WinGuiControlStatusbar_setBkColor_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, setIcon, WinGuiControlStatusbar_setIcon_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, setMinHeight, WinGuiControlStatusbar_setMinHeight_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, setParts, WinGuiControlStatusbar_setParts_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, setText, WinGuiControlStatusbar_setText_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, setTipText, WinGuiControlStatusbar_setTipText_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, setUnicodeFormat, WinGuiControlStatusbar_setUnicodeFormat_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiStatusbar, setSimple, WinGuiControlStatusbar_setSimple_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Control\Statusbar Custom Object magic                                                 
------------------------------------------------------------------*/
/* {{{ window proc for the statusbar */
LRESULT CALLBACK wingui_control_statusbar_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef ZTS
	TSRMLS_D;
#endif;
	zval ***extra = NULL;
	zend_bool stop_default = FALSE;
	int extra_count = 0, handle_default = 1;
	zval *return_value;

	wingui_window_object *statusbar_object = (wingui_window_object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
#ifdef ZTS
	TSRMLS_C = statusbar_object->TSRMLS_C;
#endif

	// enter giantic ugly switch
	switch(msg) {
		/* Must be first! some messages don't like to be forwarded normally, notice no break */
		//case WINGUI_INTERNAL_MESSAGE:
		//{
		//	msg = (UINT) wParam;
		//	handle_default = 0;
		//}
		case NM_CLICK:
		case NM_DBLCLK:
		case NM_RCLICK:
		case NM_RDBLCLK:
		{
			/* mouse clicks get three extra params passed */
			extra_count = 3;
	
			extra = wingui_callback_extra_zvals_ctor(extra_count);
			ZVAL_LONG(*extra[0], ((LPNMMOUSE) lParam)->pt.x); /* screen x */
			ZVAL_LONG(*extra[1], ((LPNMMOUSE) lParam)->pt.y); /* screen y */
			ZVAL_LONG(*extra[2], ((LPNMMOUSE) lParam)->dwItemSpec); /* index of the section that was clicked */
	
			break;
		}
		/* No data cracker needed
		   SBN_SIMPLEMODECHANGE 
		   SB_GETBORDERS */
	}

	if(statusbar_object) {
		return_value = wingui_winproc_callback_dispatch(statusbar_object, msg, extra, extra_count, &stop_default TSRMLS_CC);
	}

	if (extra) {
		wingui_callback_extra_zvals_dtor(extra_count, extra);
	}

	if (return_value) {
		/* handle special return value meanings
		   no handling needed for:
		   SBN_SIMPLEMODECHANGE */
		switch(msg) {
			case NM_CLICK:
			case NM_DBLCLK:
			case NM_RCLICK:
			case NM_RDBLCLK:
			{
				if (wingui_juggle_type(return_value, IS_BOOL TSRMLS_CC) == SUCCESS && Z_BVAL_P(return_value) == 1) {
					return (LRESULT) TRUE;
				} else {
					return (LRESULT) FALSE;
				}
			}
			/* SB_GETBORDERS change return value to array */
			case SB_GETBORDERS:
			{
				if (Z_TYPE_P(return_value) == IS_ARRAY)
				{
					int *border_array = (void *) (lParam);
					zval **vertical, **horizontal, **border;

					if ((zend_hash_index_find(Z_ARRVAL_P(return_value), 0, (void **)&vertical) == SUCCESS) &&
						(zend_hash_index_find(Z_ARRVAL_P(return_value), 1, (void **)&horizontal) == SUCCESS) &&
						(zend_hash_index_find(Z_ARRVAL_P(return_value), 2, (void **)&border) == SUCCESS)) {
						if (wingui_juggle_type(*vertical, IS_LONG TSRMLS_CC) == SUCCESS &&
						wingui_juggle_type(*horizontal, IS_LONG TSRMLS_CC) == SUCCESS &&
						wingui_juggle_type(*border, IS_LONG TSRMLS_CC) == SUCCESS) {
							border_array[0] = Z_LVAL_PP(vertical);
							border_array[1] = Z_LVAL_PP(horizontal);
							border_array[2] = Z_LVAL_PP(border);
						}
						return (LRESULT) TRUE;
					}
					return (LRESULT) FALSE;
				}
			}
			break;
		}
	}

	if (handle_default == 1) {
		return CallWindowProc(statusbar_object->window_proc, hwnd, msg, wParam, lParam); 
	} else {
		return (LRESULT) NULL;
	}
}
/* }}} */

/* {{{ window object with control/statusbar specific internal stuff */
zend_object_value wingui_control_statusbar_object_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	wingui_window_object *window_object;
	zval *tmp;

	window_object = emalloc(sizeof(wingui_window_object));
	window_object->std.ce = ce;
	window_object->std.guards = NULL;
	window_object->parent_is_temp = 0;
	window_object->window_handle = NULL;
	window_object->prop_handler = &wingui_control_statusbar_prop_handlers;
	window_object->callback_map = &wingui_control_statusbar_callback_map;
	window_object->window_proc = NULL;
	window_object->messages_results = wingui_control_statusbar_messages_results;
	window_object->messages_cracker = wingui_control_statusbar_messages_cracker;
	window_object->messages_packer = wingui_control_statusbar_messages_packer;
	window_object->messages_zvals = wingui_control_statusbar_messages_zvals;
	window_object->messages_dispatch = wingui_control_statusbar_messages_dispatch;
	memset(window_object->data.statusbar.tip_text_len, 0, sizeof(window_object->data.statusbar.tip_text_len));
	memset(window_object->data.statusbar.icon_zval, 0, sizeof(window_object->data.statusbar.icon_zval));
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

/* {{{ adds or subtracts different style flags */	
int wingui_statusbar_write_style(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	wingui_window_object *statusbar = wingui_window_object_juggle(object);

	int style = GetWindowLongPtr(statusbar->window_handle, GWL_STYLE);
	wingui_juggle_type(value, IS_BOOL TSRMLS_CC);

	if(strcmp(Z_STRVAL_P(member), "tooltips") == 0) {
		if (Z_BVAL_P(value) == TRUE) {
			style |= SBARS_TOOLTIPS | SBT_TOOLTIPS;
		} else {
			style &= ~SBARS_TOOLTIPS;
			style &= ~SBT_TOOLTIPS;
		}
	} else if(strcmp(Z_STRVAL_P(member), "sizegrip") == 0) {
		if (Z_BVAL_P(value) == TRUE) {
			style |= SBARS_SIZEGRIP;
		} else {
			style &= ~SBARS_SIZEGRIP;
		}
	}
			
	SetWindowLongPtr(statusbar->window_handle, GWL_STYLE, style);
	SetWindowPos(statusbar->window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return SUCCESS;
}
/* }}} */

/* {{{ Read tooltips or sizegrip status */
int wingui_statusbar_read_style(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	wingui_window_object *statusbar = wingui_window_object_juggle(object);
	int style = GetWindowLongPtr(statusbar->window_handle, GWL_STYLE);

	if(strcmp(Z_STRVAL_P(member), "sizegrip") == 0) {
		if (style & SBARS_SIZEGRIP) {
			ZVAL_BOOL(*retval, 1);
		} else {
			ZVAL_BOOL(*retval, 0);
		}
	} else if(strcmp(Z_STRVAL_P(member), "tooltips") == 0) {
		if (style & SBT_TOOLTIPS || style & SBARS_TOOLTIPS) {
			ZVAL_BOOL(*retval, 1);
		} else {
			ZVAL_BOOL(*retval, 0);
		}
	}
	return SUCCESS;
}
/* }}} */

/* {{{ writes the window control id */
int wingui_statusbar_write_id(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	wingui_window_object *statusbar = wingui_window_object_juggle(object);
	wingui_juggle_type(value, IS_LONG TSRMLS_CC);
	SetWindowLongPtr(statusbar->window_handle, GWLP_ID, Z_LVAL_P(value));
	SetWindowPos(statusbar->window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return SUCCESS;

}
/* }}} */

/* {{{ Grabs current window control id */
int wingui_statusbar_read_id(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	wingui_window_object *statusbar = wingui_window_object_juggle(object);
	int id = GetWindowLongPtr(statusbar->window_handle, GWLP_ID);
	ZVAL_LONG(*retval, id);

	return SUCCESS;
}
/* }}} */

/* put lparams and wparams into zvals for progressbar messages */
zval ***wingui_control_statusbar_messages_cracker(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC)
{
	zval ***extra = NULL;
	int i = 0;
	int mask = SBT_NOBORDERS | SBT_OWNERDRAW | SBT_POPOUT | SBT_RTLREADING | SBT_NOTABPARSING;

	switch(msg) {
		case SB_GETUNICODEFORMAT:
		case SB_ISSIMPLE:
			*extra_count = 0;
			return extra;
		case SB_GETBORDERS:
			*extra_count = 3;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			//ZVAL_LONG(*extra[0], *lParam[0]);
			//ZVAL_LONG(*extra[1], *lParam[1]);
			//ZVAL_LONG(*extra[2], *lParam[2]);
			return extra;
		case SB_GETICON:
		case SB_GETTEXTLENGTH:
		case SB_SETBKCOLOR:
		case SB_SETMINHEIGHT:
			*extra_count = 1;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], *wParam);
			return extra;
		case SB_GETPARTS:
		case SB_SETPARTS:
			*extra_count = 2;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], *wParam);
			array_init(*extra[1]);
			/*for(i = 0; i < **extra[0]; i++) {
				add_next_index_long(*extra[1], *lParam[i]);
			}*/
			return extra;
		case SB_GETRECT:
			*extra_count = 2;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], *wParam);
			array_init(*extra[1]);
			//add_assoc_long(*extra[1], "left", *lParam->left);
			//add_assoc_long(*extra[1], "right", *lParam->right);
			//add_assoc_long(*extra[1], "bottom", *lParam->bottom);
			//add_assoc_long(*extra[1], "top", *lParam->top);
			return extra;
		case SB_GETTEXT:
		case SB_SETTIPTEXT:
			*extra_count = 2;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], *wParam);
			ZVAL_STRING(*extra[1], (char *) *lParam, 1);
			return extra;
		case SB_GETTIPTEXT:
			*extra_count = 3;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], LOWORD(*wParam));
			ZVAL_LONG(*extra[1], HIWORD(*wParam));
			ZVAL_STRING(*extra[2], (char *) *lParam, 1);
			return extra;
		case SB_SETICON:
			php_printf("todo, need access to statusbar object - crap");
			return extra;
		case SB_SETTEXT:
			*extra_count = 3;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_LONG(*extra[0], *wParam &~ mask);
			ZVAL_LONG(*extra[1], *wParam &~ Z_LVAL_PP(extra[0]));
			ZVAL_STRING(*extra[2], (char *) *lParam, 1);
			return extra;
		case SB_SETUNICODEFORMAT:
		case SB_SIMPLE:
			*extra_count = 1;
			extra = wingui_callback_extra_zvals_ctor(*extra_count);
			ZVAL_BOOL(*extra[0], *wParam);
			return extra;
		default:
			return wingui_window_messages_cracker(msg, wParam,lParam, extra_count TSRMLS_CC);
	}
}
/* }}} */

/* {{{ put cracked values into lparams, wparams before calling defaultproc */
void wingui_control_statusbar_messages_packer(int msg, WPARAM *wParam, LPARAM *lParam, zval ***extra TSRMLS_DC)
{
	zval *value = NULL;

	return;

	switch(msg) {
		case SB_GETBORDERS:
			//int border_array [3];
			
			if(wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC) == SUCCESS) {
				//border_array[0] = Z_LVAL_P(*extra[0]);
			}
			if(wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC) == SUCCESS) {
				//border_array[1] = Z_LVAL_P(*extra[0]);
			}
			if(wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC) == SUCCESS) {
				//border_array[2] = Z_LVAL_P(*extra[0]);
			}
			//*wParam = &border_array;
	}

		/* All the no params msgs go first
SB_GETBORDERS
SB_GETICON
SB_GETPARTS
SB_GETRECT
SB_GETTEXT
SB_GETTEXTLENGTH
SB_GETTIPTEXT
SB_GETUNICODEFORMAT
SB_ISSIMPLE
SB_SETBKCOLOR
SB_SETICON
SB_SETMINHEIGHT
SB_SETPARTS
SB_SETTEXT
SB_SETTIPTEXT
SB_SETUNICODEFORMAT
SB_SIMPLE */

/*
		case WM_CANCELMODE:
		case WM_CHILDACTIVATE:
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_ENTERSIZEMOVE:
		case WM_EXITSIZEMOVE:
			break;
		case WM_ACTIVATEAPP:
		case WM_ENABLE:
		case WM_GETICON:
			if(wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC) == SUCCESS) {
				*wParam = Z_LVAL_P(*extra[0]);
			}
			break;
		case WM_GETMINMAXINFO:
			/* maxsize 
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[0]), 0, &value) == SUCCESS) {
				if(wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
					((MINMAXINFO*)*lParam)->ptMaxSize.x = Z_LVAL_P(value);
				}
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[0]), 1, &value) == SUCCESS) {
				if(wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
					((MINMAXINFO*)*lParam)->ptMaxSize.y = Z_LVAL_P(value);
				}
			}
			
			/* maxposition 
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[1]), 0, &value) == SUCCESS) {
				if(wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
					((MINMAXINFO*)*lParam)->ptMaxPosition.x = Z_LVAL_P(value);
				}
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[1]), 1, &value) == SUCCESS) {
				value = wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMaxPosition.y = Z_LVAL_P(value);
			}

			/* mintracksize 
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[2]), 0, &value) == SUCCESS) {
				value = wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMinTrackSize.x = Z_LVAL_P(value);
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[2]), 1, &value) == SUCCESS) {
				value = wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMinTrackSize.y = Z_LVAL_P(value);
			}	

			/* maxtracksize 
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[3]), 0, &value) == SUCCESS) {
				value = wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMaxTrackSize.x = Z_LVAL_P(value);
			}
			if (zend_hash_index_find(Z_ARRVAL_P(*extra[3]), 1, &value) == SUCCESS) {
				value = wingui_juggle_type(value, IS_LONG TSRMLS_CC);
				((MINMAXINFO*)*lParam)->ptMaxTrackSize.y = Z_LVAL_P(value);
			}
			break;
		case WM_INPUTLANGCHANGE:
		case WM_INPUTLANGCHANGEREQUEST:
			*extra[0] = wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC);
			*wParam = Z_LVAL_P(*extra[0]);
			*extra[1] = wingui_juggle_type(*extra[1], IS_LONG TSRMLS_CC);
			*lParam = Z_LVAL_P(*extra[1]);
			break;
		case WM_MOVE:
			*extra[0] = wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC);
			*extra[1] = wingui_juggle_type(*extra[1], IS_LONG TSRMLS_CC);
			*lParam = MAKELPARAM(*extra[0], *extra[1]);
			break;
		case WM_SIZE:
			*extra[0] = wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC);
			*wParam = Z_LVAL_P(*extra[0]);
			*extra[1] = wingui_juggle_type(*extra[1], IS_LONG TSRMLS_CC);
			*extra[2] = wingui_juggle_type(*extra[2], IS_LONG TSRMLS_CC);
			*lParam = MAKELPARAM(*extra[1], *extra[2]);
			break;
		case WM_COMMAND:
			/* lparam is 0 we have Menu 
			if (*lParam == 0 && HIWORD(*wParam) == 0) {
				if(wingui_juggle_type(*extra[0], IS_LONG TSRMLS_CC) == SUCCESS) {
					*wParam = MAKEWPARAM(*extra[0], 0);
				}
			}
			break;
	/* basic window message crackers 
	/* Send these on to their destination classes 
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
			/* LPNMHDR STRUCT 
			HWND hwndFrom;
			UINT_PTR idFrom;
			UINT code; 
			/* Statusbar rethrowing 
			case NM_CLICK:
			case NM_DBLCLK:
			case NM_RCLICK:
			case NM_RDBLCLK:
			{
				SendMessage((HWND) ((LPNMHDR)*lParam)->hwndFrom, WINGUI_INTERNAL_MESSAGE, ((LPNMHDR)*lParam)->code, *lParam);
			}
		}
	case WM_COMMAND:
			/* Button messages are sent by default to the parent, this redirects to the button itself 
			switch(HIWORD(*wParam)) {
				case BCN_HOTITEMCHANGE:
				case BN_CLICKED:
				case BN_DBLCLK:
				case BN_DISABLE:
				case BN_HILITE: // BN_PUSHED
				case BN_KILLFOCUS:
				case BN_PAINT:
				case BN_SETFOCUS:
				case BN_UNHILITE: // BN_UNPUSHED
				{
					SendMessage((HWND) *lParam, HIWORD(*wParam), 0, 0);
					break;
				}
			}
			
		}
		break;*/
	//}
}
/* }}} */

/* {{{ put zval return values from from messages into lresult vals */
LRESULT wingui_control_statusbar_messages_results(int msg, zval *return_value TSRMLS_DC)
{
	zval *value = NULL;
	long length = 0, type = 0;

	switch(msg) {
		/* Messages with no LRESULT to manipulate */
		case SBN_SIMPLEMODECHANGE:
		case SB_GETTIPTEXT:
		case SB_SETMINHEIGHT:
		case SB_SETTIPTEXT:
		case SB_SIMPLE:
			return (LRESULT) NULL;
		/* Messages with BOOL result */
		case NM_CLICK:
		case NM_DBLCLK:
		case NM_RCLICK:
		case NM_RDBLCLK:
		case SB_GETBORDERS:
		case SB_GETRECT:
		case SB_GETUNICODEFORMAT:
		case SB_ISSIMPLE:
		case SB_SETICON:
		case SB_SETPARTS:
		case SB_SETTEXT:
		case SB_SETUNICODEFORMAT:
			if (wingui_juggle_type(return_value, IS_BOOL TSRMLS_CC) == SUCCESS) {
				return (LRESULT) Z_BVAL_P(return_value);
			} else {
				return (LRESULT) FALSE;
			}
		case SB_GETICON:
			// TODO: returns handle to icon, so get handle out of return_value
		/* Messages with INT result */
		case SB_GETPARTS:
		case SB_SETBKCOLOR:
			if (wingui_juggle_type(return_value, IS_LONG TSRMLS_CC) == SUCCESS) {
				return (LRESULT) Z_LVAL_P(return_value);
			} else {
				return (LRESULT) 0;
			}
		/* Return value is LOWORD(text_length) HIWORD(type)
		   If we have a non-array, we assume that's the text length
		   If we have an array, [0] is the text length and [1] is the type */
		case SB_GETTEXT:
		case SB_GETTEXTLENGTH:
			if (Z_TYPE_P(return_value) == IS_ARRAY) {
				/* Attempt to get values for [0] and [1] out of the array */
				if (zend_hash_index_find(Z_ARRVAL_P(return_value), 0, &value) == SUCCESS) {
					if(wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
						length = Z_LVAL_P(value);
					}
				}
				if (zend_hash_index_find(Z_ARRVAL_P(return_value), 1, &value) == SUCCESS) {
					if(wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
						type = Z_LVAL_P(value);
					}
				}
				return (LRESULT) MAKELONG(length, type);
			} else {
				if (wingui_juggle_type(return_value, IS_LONG TSRMLS_CC) == SUCCESS) {
					return (LRESULT) MAKELONG(Z_LVAL_P(return_value), 0);
				} else {
					return (LRESULT) 0;
				}
			}
		default:
			//return wingui_window_messages_results(msg, return_value TSRMLS_CC);
			return (LRESULT) NULL;
	}
}
/* }}} */

/* {{{ put lresult vals into zval return values */
void wingui_control_statusbar_messages_zvals(int msg, LRESULT lresult, zval *return_value TSRMLS_DC)
{
	switch(msg) {
		/* Messages with no LRESULT to manipulate */
		case SBN_SIMPLEMODECHANGE:
		case SB_GETTIPTEXT:
		case SB_SETMINHEIGHT:
		case SB_SETTIPTEXT:
		case SB_SIMPLE:
			return;
		/* Messages with BOOL result */
		case NM_CLICK:
		case NM_DBLCLK:
		case NM_RCLICK:
		case NM_RDBLCLK:
		case SB_GETBORDERS:
		case SB_GETRECT:
		case SB_GETUNICODEFORMAT:
		case SB_ISSIMPLE:
		case SB_SETICON:
		case SB_SETPARTS:
		case SB_SETTEXT:
		case SB_SETUNICODEFORMAT:
			ZVAL_BOOL(return_value, lresult);
			return;
		case SB_GETICON:
			// TODO: returns handle to icon, so get handle out of return_value - really icky
		/* Messages with INT result */
		case SB_GETPARTS:
		case SB_SETBKCOLOR:
			ZVAL_LONG(return_value, lresult);
			return;
		/* Return value is LOWORD(text_length) HIWORD(type)
		   so we have to create an array and fill it */
		case SB_GETTEXT:
		case SB_GETTEXTLENGTH:
			array_init(return_value);
			add_next_index_long(return_value, LOWORD(lresult));
			add_next_index_long(return_value, HIWORD(lresult));
			return;
		default:
			//return wingui_window_messages_zvals(msg, return_value TSRMLS_CC);
			return;
	}
}
/* }}} */

/* {{{ parse args from send/post/etc and shove into lparam and wparams */
int wingui_control_statusbar_messages_dispatch(INTERNAL_FUNCTION_PARAMETERS, int item, WPARAM *wParam, LPARAM *lParam)
{
	zend_error_handling error_handling;
	int retval = SUCCESS;
	long msg;
	/* get borders */
	int border_array [3];
	/* settext */
	char * string;
	int str_len;
	long part = 0, type = 0;
	/* window handle */
	wingui_window_object *statusbar_obj = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	switch(item) {
		/* series of WM_NOTIFY messages instead */
		//case NM_CLICK:
//NM_CLICK (status bar)
//NM_DBLCLK (status bar)
//NM_RCLICK (status bar)
//NM_RDBLCLK (status bar)
//SBN_SIMPLEMODECHANGE
/*
		SB_GETBORDERS
SB_GETICON
SB_GETPARTS
SB_GETRECT
SB_GETTEXT
SB_GETTEXTLENGTH
SB_GETTIPTEXT
SB_GETUNICODEFORMAT
SB_ISSIMPLE
SB_SETBKCOLOR
SB_SETICON
SB_SETMINHEIGHT
SB_SETPARTS
SB_SETTEXT
SB_SETTIPTEXT
SB_SETUNICODEFORMAT
SB_SIMPLE */
		case SB_GETBORDERS:
			if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &msg) == FAILURE) {
				retval  = FAILURE;
				break;
			}
			*wParam = (WPARAM) 0;
			*lParam = (LPARAM) &border_array;
			break;
		case SB_SETTEXT:
			if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|ll", &msg, &string, &str_len, &part, &type) == FAILURE) {
				retval  = FAILURE;
				break;
			}
			part = wingui_control_statusbar_checkparts(part, statusbar_obj->window_handle, SB_SIMPLEID);
			*wParam = (WPARAM) part | type;
			*lParam = (LPARAM) estrdup(string);
			break;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	return retval;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Control\Statusbar LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_control_statusbar)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, ZEND_NS_NAME("Control", "Statusbar"), wingui_control_statusbar_functions);
	ce_wingui_control_statusbar = zend_register_internal_class_ex(&ce, ce_wingui_control, ZEND_NS_NAME(PHP_WINGUI_NS, "Control") TSRMLS_CC);
	ce_wingui_control_statusbar->create_object  = wingui_control_statusbar_object_new;

	/* real mapped constants for setText */
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_control_statusbar, NOBORDERS,		SBT_NOBORDERS);
	/* REGISTER_WINGUI_NAMED_CONST(ce_wingui_control_statusbar, OWNERDRAW,		SBT_OWNERDRAW); need to handle ownerdraw implementation */
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_control_statusbar, POPOUT,		SBT_POPOUT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_control_statusbar, RTLREADING,	SBT_RTLREADING);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_control_statusbar, NOTABPARSING,	SBT_NOTABPARSING);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_control_statusbar, SIMPLEID,	    SB_SIMPLEID);

	/* property handlers for "fake" properties - control styles */
	zend_hash_init(&wingui_control_statusbar_prop_handlers, 0, NULL, NULL, 1);
	wingui_register_prop_handler(&wingui_control_statusbar_prop_handlers, ce_wingui_control_statusbar, "tooltips", wingui_statusbar_read_style, wingui_statusbar_write_style TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_statusbar_prop_handlers, ce_wingui_control_statusbar, "sizegrip", wingui_statusbar_read_style, wingui_statusbar_write_style TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_statusbar_prop_handlers, ce_wingui_control_statusbar, "id", wingui_statusbar_read_id, wingui_statusbar_write_id TSRMLS_CC);
	zend_hash_merge(&wingui_control_statusbar_prop_handlers, &wingui_control_prop_handlers, NULL, NULL, sizeof(wingui_prop_handler), 0);

	/* Statusbar notifications hook up to */
	zend_hash_init(&wingui_control_statusbar_callback_map, 0, NULL, NULL, 1);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, NM_CLICK,				&wingui_control_statusbar_callback_map, onnmclick);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, NM_DBLCLK,			&wingui_control_statusbar_callback_map, onnmdblclck);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, NM_RCLICK,			&wingui_control_statusbar_callback_map, onnmrclick);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, NM_RDBLCLK,			&wingui_control_statusbar_callback_map, onnmrdblclk);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SBN_SIMPLEMODECHANGE,	&wingui_control_statusbar_callback_map, onsbnsimplemodechange);
	/* Statusbar messages to hook up to */
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETBORDERS,		&wingui_control_statusbar_callback_map, onsbgetborders);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETICON,			&wingui_control_statusbar_callback_map, onsbgeticon);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETPARTS,			&wingui_control_statusbar_callback_map, onsbgetparts);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETRECT,			&wingui_control_statusbar_callback_map, onsbgetrect);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETTEXT,			&wingui_control_statusbar_callback_map, onsbgettext);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETTEXTLENGTH,		&wingui_control_statusbar_callback_map, onsbgettextlength);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETTIPTEXT,		&wingui_control_statusbar_callback_map, onsbgettiptext);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_GETUNICODEFORMAT,	&wingui_control_statusbar_callback_map, onsbgetunicodeformat);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_ISSIMPLE,			&wingui_control_statusbar_callback_map, onsbissimple);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_SETBKCOLOR,		&wingui_control_statusbar_callback_map, onsbsetbkcolor);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_SETICON,			&wingui_control_statusbar_callback_map, onsbseticon);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_SETMINHEIGHT,		&wingui_control_statusbar_callback_map, onsbsetminheight);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_SETPARTS,			&wingui_control_statusbar_callback_map, onsbsetparts);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_SETTEXT,			&wingui_control_statusbar_callback_map, onsbsettext);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_SETTIPTEXT,		&wingui_control_statusbar_callback_map, onsbsettiptext);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_SETUNICODEFORMAT,	&wingui_control_statusbar_callback_map, onsbsetunicodeformat);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control_statusbar, SB_SIMPLE,			&wingui_control_statusbar_callback_map, onsbsimple);
	/* copy in all the control ones 
	zend_hash_merge(&wingui_statusbar_callback_map, &wingui_control_callback_map, NULL, NULL, sizeof(char *), 0);*/
	return SUCCESS;
}

/* {{{ PHP_MSHUTDOWN_FUNCTION(wingui_control_statusbar) */
PHP_MSHUTDOWN_FUNCTION(wingui_control_statusbar)
{
	zend_hash_destroy(&wingui_control_statusbar_prop_handlers);
	zend_hash_destroy(&wingui_control_statusbar_callback_map);
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
/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Elizabeth M Smith <auroraeosrose@gmail.com>                  |
  +----------------------------------------------------------------------+
*/

#include "php_wingui.h"
#include "messaging.h"
#include "windowing.h"
#include "inputing.h"

/* Static controls - text, image and frame */
zend_class_entry *ce_wingui_control_statictext;
zend_class_entry *ce_wingui_control_staticimage;
zend_class_entry *ce_wingui_control_staticframe;

/* ----------------------------------------------------------------
    Win\Gui\Window C API
------------------------------------------------------------------*/
int wingui_control_static_autosize(HWND handle, int style, int extrastyle, int x, int y TSRMLS_DC)
{
	HDC dc;
	RECT rect;
	BOOL worked;
	UINT flags = DT_CALCRECT;

	dc = GetDC(handle);
	if (dc == 0) {
		goto error;
	}

	worked = GetClientRect(handle, &rect);

	if (worked == 0) {
		goto error;
	}

	/* get our flags right for drawtext */
	if ((SS_CENTER & style) == SS_CENTER) {
		flags |= DT_CENTER;
	} else if ((SS_RIGHT & style) == SS_RIGHT) {
		flags |= DT_RIGHT;
	} else if ((SS_LEFT & style) == SS_LEFT) {
		flags |= DT_LEFT;
	}
	
	if ( ((WS_EX_RTLREADING & extrastyle) == WS_EX_RTLREADING)
		|| ((WS_EX_LAYOUTRTL & extrastyle) == WS_EX_LAYOUTRTL) ) {
			flags |= DT_RTLREADING;
	}

	if ((SS_WORDELLIPSIS & style) == SS_WORDELLIPSIS) {
		flags |= DT_WORD_ELLIPSIS | DT_SINGLELINE;
	} else if ((SS_PATHELLIPSIS & style) == SS_PATHELLIPSIS) {
		flags |= DT_PATH_ELLIPSIS | DT_SINGLELINE;
	} else if ((SS_ENDELLIPSIS & style) == SS_ENDELLIPSIS) {
		flags |= DT_END_ELLIPSIS | DT_SINGLELINE;
	} 

	if ((SS_NOPREFIX & style) == SS_NOPREFIX) {
		flags |= DT_NOPREFIX;
	} 

	if ((SS_LEFTNOWORDWRAP & style) == SS_LEFTNOWORDWRAP) {
		flags |= DT_SINGLELINE;
	} else if ((SS_SIMPLE & style) == SS_SIMPLE) {
		flags |= DT_SINGLELINE;
	} else {
		flags |= DT_WORDBREAK;
	}

	if ((SS_EDITCONTROL & style) == SS_EDITCONTROL) {
		flags |= DT_EDITCONTROL;
	}

	if (IsWindowUnicode(handle)) {
		int length, worked_len;
		wchar_t * text;

		length = GetWindowTextLengthW(handle);
		text = safe_emalloc(length, sizeof(wchar_t), 1);

		worked_len = GetWindowTextW(handle, text, length + 1);
		if (worked_len == 0) {
			goto error;
		}

		worked = DrawTextW(dc, text, -1, &rect, flags); 

		if (worked == 0) {
			goto error;
		}
	} else {
		int length, worked_len;
		char * text;

		length = GetWindowTextLengthA(handle);
		text = safe_emalloc(length, sizeof(char), 1);

		worked_len = GetWindowTextA(handle, text, length + 1);
		if (worked_len == 0) {
			goto error;
		}

		worked = DrawTextA(dc, text, -1, &rect, flags); 

		if (worked == 0) {
			goto error;
		}
	}

    MoveWindow(handle, x, y, (rect.right - rect.left), (rect.bottom - rect.top), FALSE);
	ReleaseDC(handle, dc);
	return SUCCESS;

error:
	ReleaseDC(handle, dc);
	winsystem_create_error(GetLastError(), ce_winsystem_runtimeexception TSRMLS_CC);
	return FAILURE;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Control\Text Userland API
------------------------------------------------------------------*/

ZEND_BEGIN_ARG_INFO_EX(WinGuiControlText___construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
	ZEND_ARG_INFO(0, parent)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiControlText_fitToText_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\Text::__construct(Win\Gui\Window parent, string|object text[, array options])
     Create a new text area static control */
PHP_METHOD(WinGuiControlText, __construct)
{
	HashTable *options = NULL;

	/* Defaults for the window */
	zend_bool use_unicode = FALSE;
	char * text;
	int text_length;
	long x = 0, y = 0, width = 400, height = 400, style = WS_CHILD | SS_LEFT | WS_VISIBLE, extrastyle = 0;
	zval *parent = NULL, *unicode;
	HWND window_handle = NULL;
	wingui_window_object *window_object;
	zval **value;

	PHP_WINGUI_EXCEPTIONS
	/* version one, use unicode */
	if (FAILURE != zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "OO|H", &parent, ce_wingui_window,
		&unicode, ce_winsystem_unicode, &options)) {
		use_unicode = 1;
	} else if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Os|H", &parent, ce_wingui_window, &text, &text_length, &options)) {
		PHP_WINGUI_RESTORE_ERRORS
		return;
	}
	PHP_WINGUI_RESTORE_ERRORS

	window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	wingui_window_get_size(options, &x, &y, &width, &height TSRMLS_CC);

	/* Get our text alignment */
	if (zend_hash_find(options, "align", sizeof("align"), (void **) &value) == SUCCESS) {
		convert_to_long(*value);
		if (SS_CENTER == Z_LVAL_PP(value)) {
			style |= SS_CENTER;
		} else if (SS_RIGHT == Z_LVAL_PP(value)) {
			style |= SS_RIGHT;
		}
	}

	/* Get our word wrap state */
	if (zend_hash_find(options, "wordwrap", sizeof("wordwrap"), (void **) &value) == SUCCESS) {
		convert_to_boolean(*value);
		if (FALSE == Z_BVAL_PP(value)) {
			style |= SS_LEFTNOWORDWRAP;
		}
	}

	/* Get ellipsis if set */
	if (zend_hash_find(options, "ellipsis", sizeof("ellipsis"), (void **) &value) == SUCCESS) {
		convert_to_long(*value);
		if (SS_ENDELLIPSIS == Z_LVAL_PP(value)) {
			style |= SS_ENDELLIPSIS;
		} else if (SS_PATHELLIPSIS == Z_LVAL_PP(value)) {
			style |= SS_PATHELLIPSIS;
		} else if (SS_WORDELLIPSIS == Z_LVAL_PP(value)) {
			style |= SS_WORDELLIPSIS;
		}
	}

	/* prefix is OFF by default */
	if (zend_hash_find(options, "prefix", sizeof("prefix"), (void **) &value) == SUCCESS) {
		convert_to_boolean(*value);
		if (FALSE == Z_BVAL_PP(value)) {
			style |= SS_NOPREFIX;
		}
	} else {
		style |= SS_NOPREFIX;
	}

	/* simple style */
	if (zend_hash_find(options, "simple", sizeof("simple"), (void **) &value) == SUCCESS) {
		convert_to_boolean(*value);
		if (TRUE == Z_BVAL_PP(value)) {
			style |= SS_SIMPLE;
		}
	}

	/* edit style */
	if (zend_hash_find(options, "editstyle", sizeof("editstyle"), (void **) &value) == SUCCESS) {
		convert_to_boolean(*value);
		if (TRUE == Z_BVAL_PP(value)) {
			style |= SS_EDITCONTROL;
		}
	}

	/* turn on notify */
	if (zend_hash_find(options, "notify", sizeof("notify"), (void **) &value) == SUCCESS) {
		convert_to_boolean(*value);
		if (TRUE == Z_BVAL_PP(value)) {
			style |= SS_NOTIFY;
		}
	}

	/* turn on customdraw */
	if (zend_hash_find(options, "customdraw", sizeof("customdraw"), (void **) &value) == SUCCESS) {
		convert_to_boolean(*value);
		if (TRUE == Z_BVAL_PP(value)) {
			style |= SS_OWNERDRAW;
		}
	}

	// TODO: wingui_window_get_style(options, &style, &extrastyle TSRMLS_CC);
    // TODO: window id (goes into hmenu)
	// TODO: rest of style and extrastyle parsing

	if (TRUE == use_unicode) {
		window_handle = CreateWindowExW(
			extrastyle,
			WC_STATICW, 
			php_winsystem_unicode_get_wchar(&unicode TSRMLS_CC),
			style,
			x,
			y,
			width,
			height,
			wingui_window_get_handle(&parent TSRMLS_CC),
			NULL,
			GetModuleHandle(NULL),
			(LPVOID) window_object);
	} else {
		window_handle = CreateWindowExA(
			extrastyle,
			WC_STATICA, 
			text,
			style,
			x,
			y,
			width,
			height,
			wingui_window_get_handle(&parent TSRMLS_CC),
			NULL,
			GetModuleHandle(NULL),
			(LPVOID) window_object);
	}

	if (!window_handle) {
		winsystem_create_error(GetLastError(), ce_winsystem_runtimeexception TSRMLS_CC);
		return;
	} else {
		wingui_window_create_new(window_object, window_handle, use_unicode);
		if (TRUE == use_unicode) {
			SetWindowLongPtrW(window_handle, GWLP_USERDATA, (LONG_PTR) window_object);
		} else {
			SetWindowLongPtrA(window_handle, GWLP_USERDATA, (LONG_PTR) window_object);		
		}

		/* size window to contents */
		if (zend_hash_find(options, "autosize", sizeof("autosize"), (void **) &value) == SUCCESS) {
			convert_to_boolean(*value);
			if (TRUE == Z_BVAL_PP(value)) {
				wingui_control_static_autosize(window_handle, style, extrastyle, x, y TSRMLS_CC);
			}
		}

	}
}
/* }}} */

/* register text methods */
static zend_function_entry wingui_control_statictext_functions[] = {
	PHP_ME(WinGuiControlText, __construct, WinGuiControlText___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)

	/* 	Messaging Implementation */
	PHP_ME(WinGuiMessaging, connect, WinGuiMessaging_connect_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, connectFull, WinGuiMessaging_connectFull_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, connectSimple, WinGuiMessaging_connectSimple_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, disconnect, WinGuiMessaging_disconnect_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, get, WinGuiMessaging_get_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, peek, WinGuiMessaging_peek_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, post, WinGuiMessaging_post_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, send, WinGuiMessaging_send_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, sendNotify, WinGuiMessaging_sendNotify_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, sendCallback, WinGuiMessaging_sendCallback_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiMessaging, sendTimeout, WinGuiMessaging_sendTimeout_args, ZEND_ACC_PUBLIC)
	
	/* Windowing implementation */
	PHP_ME(WinGuiWindowing, isVisible, WinGuiWindowing_isVisible_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, show, WinGuiWindowing_show_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, hide, WinGuiWindowing_hide_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, getSize, WinGuiWindowing_getSize_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, getPos, WinGuiWindowing_getPos_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindowing, getText, WinGuiWindowing_getText_args, ZEND_ACC_PUBLIC)

	/* Inputing implementation */
	PHP_ME(WinGuiInputing, enable, WinGuiInputing_enable_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, disable, WinGuiInputing_disable_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, isEnabled, WinGuiInputing_isEnabled_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, activate, WinGuiInputing_activate_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, deactivate, WinGuiInputing_deactivate_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, isActive, WinGuiInputing_isActive_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, focus, WinGuiInputing_focus_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, unfocus, WinGuiInputing_unfocus_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, hasFocus, WinGuiInputing_hasFocus_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, registerHotKey, WinGuiInputing_registerHotKey_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, unregisterHotKey, WinGuiInputing_unregisterHotKey_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, capture, WinGuiInputing_capture_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, release, WinGuiInputing_release_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, hasCapture, WinGuiInputing_hasCapture_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, trackMouse, WinGuiInputing_trackMouse_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiInputing, dragDetect, WinGuiInputing_dragDetect_args, ZEND_ACC_PUBLIC)
	ZEND_FE_END
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
}
/* }}} */

/* {{{ proto  Win\Gui\Resource\Image Win\Gui\Control\Image->setImage(Win\Gui\Resource\Image image);
     set a new bitmap/cursor/icon image */
PHP_METHOD(WinGuiControlImage, setImage)
{
}
/* }}} */

/* {{{ proto Win\Gui\Resource\Image Win\Gui\Control\Image->getImage();
     gets the bitmap/cursor/icon image */
PHP_METHOD(WinGuiControlImage, getImage)
{
}
/* }}} */

/* register Image methods */
static zend_function_entry wingui_control_staticimage_functions[] = {
	PHP_ME(WinGuiControlImage, __construct, WinGuiControlImage___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiControlImage, setImage, WinGuiControlImage_setImage_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiControlImage, getImage, NULL, ZEND_ACC_PUBLIC)
	ZEND_FE_END
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
}
/* }}} */

/* register Frame methods */
static zend_function_entry wingui_control_staticframe_functions[] = {
	PHP_ME(WinGuiControlFrame, __construct, WinGuiControlFrame___construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	ZEND_FE_END
};

/* {{{ wingui_static_object_create */
static zend_object_value wingui_static_object_create(zend_class_entry *ce TSRMLS_DC)
{
	return wingui_window_object_create_ex(ce, &std_object_handlers TSRMLS_CC);
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Control\Text/Frame/Image LifeCycle Functions
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_control_static)
{
	zend_class_entry text_ce, image_ce, frame_ce;

	INIT_NS_CLASS_ENTRY(text_ce, PHP_WINGUI_NS, ZEND_NS_NAME("Control", "Text"), wingui_control_statictext_functions);
	ce_wingui_control_statictext = zend_register_internal_class(&text_ce TSRMLS_CC);
	ce_wingui_control_statictext->create_object = wingui_static_object_create;

	REGISTER_ENUM_CONST("LEFT", SS_LEFT, ce_wingui_control_statictext);
	REGISTER_ENUM_CONST("RIGHT", SS_RIGHT, ce_wingui_control_statictext);
	REGISTER_ENUM_CONST("CENTER", SS_CENTER, ce_wingui_control_statictext);

	REGISTER_ENUM_CONST("END", SS_ENDELLIPSIS, ce_wingui_control_statictext);
	REGISTER_ENUM_CONST("PATH", SS_PATHELLIPSIS, ce_wingui_control_statictext);
	REGISTER_ENUM_CONST("WORD", SS_WORDELLIPSIS, ce_wingui_control_statictext);

	INIT_NS_CLASS_ENTRY(image_ce, PHP_WINGUI_NS, ZEND_NS_NAME("Control", "Image"), wingui_control_staticimage_functions);
	ce_wingui_control_staticimage = zend_register_internal_class(&image_ce TSRMLS_CC);

	INIT_NS_CLASS_ENTRY(frame_ce, PHP_WINGUI_NS, ZEND_NS_NAME("Control", "Frame"), wingui_control_staticframe_functions);
	ce_wingui_control_staticframe = zend_register_internal_class(&frame_ce TSRMLS_CC);

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
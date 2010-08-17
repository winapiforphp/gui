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
zend_class_entry *ce_wingui_inputing;

/* Messaging table for input */
HashTable wingui_input_callback_map;

/* ----------------------------------------------------------------
  Win\Gui\Inputing Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO(WinGuiInputing_enable_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_disable_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_isEnabled_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_activate_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_deactivate_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_isActive_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_focus_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_unfocus_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_hasFocus_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_registerHotKey_args, ZEND_SEND_BY_VAL)
    ZEND_ARG_INFO(0, id)
    ZEND_ARG_INFO(0, modifier)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_unregisterHotKey_args, ZEND_SEND_BY_VAL)
    ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_capture_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_release_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_hasCapture_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiInputing_trackMouse_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 3)
    ZEND_ARG_INFO(0, hover)
    ZEND_ARG_INFO(0, leave)
    ZEND_ARG_INFO(0, nonclient)
    ZEND_ARG_INFO(0, cancel)
    ZEND_ARG_INFO(0, time)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInputing_dragDetect_args, ZEND_SEND_BY_VAL)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\Inputing->enable()
       Allows keyboard and mouse input */
PHP_METHOD(WinGuiInputing, enable)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(EnableWindow(window_object->window_handle, TRUE));
}
/* }}} */

/* {{{ proto void Win\Gui\Inputing->disable()
       Disallows keyboard and mouse input */
PHP_METHOD(WinGuiInputing, disable)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(EnableWindow(window_object->window_handle, FALSE));
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->isEnabled()
       True if the item is enabled, or false */
PHP_METHOD(WinGuiInputing, isEnabled)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(IsWindowEnabled(window_object->window_handle));
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->activate()
       Activates an item */
PHP_METHOD(WinGuiInputing, activate)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SetActiveWindow(window_object->window_handle));
}
/* }}} */

/* {{{ proto void Win\Gui\Inputing->deactivate()
       Makes an item non-active */
PHP_METHOD(WinGuiInputing, deactivate)
{
	HWND handle = NULL;
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SetActiveWindow(handle));
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->isActive()
       True if the item is active, or false */
PHP_METHOD(WinGuiInputing, isActive)
{
	HWND worked;
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	worked = GetActiveWindow();
	if (worked == NULL) {
		worked = GetForegroundWindow();
	}

	if (worked == window_object->window_handle) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->focus()
       sets the keyboard focus to the object */
PHP_METHOD(WinGuiInputing, focus)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SetFocus(window_object->window_handle));
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->unfocus()
       ignores keystrokes */
PHP_METHOD(WinGuiInputing, unfocus)
{
	HWND handle = NULL;
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SetFocus(handle));
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->hasFocus()
        True if the item has keyboard focus, or false */
PHP_METHOD(WinGuiInputing, hasFocus)
{
	HWND worked;
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	worked = GetFocus();
	if (worked == NULL) {
		worked = GetForegroundWindow();
	}

	if (worked == window_object->window_handle) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->registerHotKey(int id, int modifier, int key)
       defines a system-wide hot key for a specific window */
PHP_METHOD(WinGuiInputing, registerHotKey)
{
	zend_error_handling error_handling;
	long id, modifier, key;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &id, &modifier, &key) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(RegisterHotKey(window_object->window_handle, id, modifier, key));
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->unregisterHotKey()
       remove a hot key */
PHP_METHOD(WinGuiInputing, unregisterHotKey)
{
	zend_error_handling error_handling;
	long id;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(UnregisterHotKey(window_object->window_handle, id));
}
/* }}} */


/* {{{ proto bool Win\Gui\Inputing->capture()
       sets the mouse capture to the object */
PHP_METHOD(WinGuiInputing, capture)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SetCapture(window_object->window_handle));
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->release()
       releases the mouse capture from the object */
PHP_METHOD(WinGuiInputing, release)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(ReleaseCapture());
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->hasCapture()
        True if the item has mouse capture, or false */
PHP_METHOD(WinGuiInputing, hasCapture)
{
	HWND worked;
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	worked =  GetCapture();

	if (worked == window_object->window_handle) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto bool Win\Gui\Inputing->trackMouse(hover, leave, nonclient[, cancel, time])
        track when the mouse pointer leaves a window or
		hovers over a window for a specified amount of time */
PHP_METHOD(WinGuiInputing, trackMouse)
{
	zend_error_handling error_handling;
	TRACKMOUSEEVENT tme;
	zend_bool hover, leave, nonclient, cancel = FALSE;
	long time = HOVER_DEFAULT;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);
	
	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "bbb|bl", &hover, &leave, &nonclient, &cancel, &time) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = 0;
	if (hover) {
		tme.dwFlags |= TME_HOVER;
	}
	if (leave) {
		tme.dwFlags |= TME_LEAVE;
	}
	if (nonclient) {
		tme.dwFlags |= TME_NONCLIENT;
	}
	if (cancel) {
		tme.dwFlags |= TME_CANCEL;
	}
	tme.dwHoverTime = time;
	tme.hwndTrack = window_object->window_handle;
	RETURN_BOOL(TrackMouseEvent(&tme));

}
/* }}} */

/* {{{ proto array Win\Gui\Inputing->dragDetect(x, y)
        captures the mouse and tracks its movement until the user
		releases the left button, presses the ESC key, or moves
		the mouse outside the drag rectangle */
PHP_METHOD(WinGuiInputing, dragDetect)
{
	zend_error_handling error_handling;
	long x, y;
	POINT point;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	point.x = x;
	point.y = y;

	RETURN_BOOL(DragDetect(window_object->window_handle, point));
}
/* }}} */

/* register Message methods */
static zend_function_entry wingui_inputing_functions[] = {
	PHP_ME(WinGuiInputing, enable, WinGuiInputing_enable_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, disable, WinGuiInputing_disable_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, isEnabled, WinGuiInputing_isEnabled_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, activate, WinGuiInputing_activate_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, deactivate, WinGuiInputing_deactivate_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, isActive, WinGuiInputing_isActive_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, focus, WinGuiInputing_focus_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, unfocus, WinGuiInputing_unfocus_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, hasFocus, WinGuiInputing_hasFocus_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, registerHotKey, WinGuiInputing_registerHotKey_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, unregisterHotKey, WinGuiInputing_unregisterHotKey_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, capture, WinGuiInputing_capture_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, release, WinGuiInputing_release_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, hasCapture, WinGuiInputing_hasCapture_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, trackMouse, WinGuiInputing_trackMouse_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiInputing, dragDetect, WinGuiInputing_dragDetect_args, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Inputing LifeCycle Functions                                                    
------------------------------------------------------------------*/

PHP_MINIT_FUNCTION(wingui_inputing)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Inputing", wingui_inputing_functions);
	ce_wingui_inputing = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_inputing->ce_flags |= ZEND_ACC_INTERFACE;

	/* input messages hook up to */
	zend_hash_init(&wingui_input_callback_map, 0, NULL, NULL, 1);

	/* input messaging */
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_GETHOTKEY,			&wingui_input_callback_map, onwmgethotkey);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_SETHOTKEY,			&wingui_input_callback_map, onwmsethotkey);
	
	/* Notifications - note this cannot be sent (see send/post callback) */
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_ACTIVATE,			&wingui_input_callback_map, onwmactivate);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_APPCOMMAND,			&wingui_input_callback_map, onwmappcommand);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_CHAR,				&wingui_input_callback_map, onwmchar);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_DEADCHAR,			&wingui_input_callback_map, onwmdeadchar);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_HOTKEY,				&wingui_input_callback_map, onwmhotkey);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_KEYDOWN,			&wingui_input_callback_map, onwmkeydown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_KEYUP,				&wingui_input_callback_map, onwmkeyup);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_KILLFOCUS,			&wingui_input_callback_map, onwmkillfocus);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_SETFOCUS,			&wingui_input_callback_map, onwmsetfocus);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_SYSDEADCHAR,		&wingui_input_callback_map, onwmsysdeadchar);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_SYSKEYDOWN,			&wingui_input_callback_map, onwmsyskeydown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_SYSKEYUP,			&wingui_input_callback_map, onwmsyskeyup);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_UNICHAR,			&wingui_input_callback_map, onwmunichar);

	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_CAPTURECHANGED,		&wingui_input_callback_map, onwmcapturechanged);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_LBUTTONDBLCLK,		&wingui_input_callback_map, onwmlbuttondblclk);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_LBUTTONDOWN,		&wingui_input_callback_map, onwmlbuttondown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_LBUTTONUP,			&wingui_input_callback_map, onwmlbuttonup);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_MBUTTONDBLCLK,		&wingui_input_callback_map, onwmmbuttondblclk);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_MBUTTONDOWN,		&wingui_input_callback_map, onwmmbuttondown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_MBUTTONUP,			&wingui_input_callback_map, onwmmbuttonup);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_MOUSEACTIVATE,		&wingui_input_callback_map, onwmmouseactivate);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_MOUSEHOVER,			&wingui_input_callback_map, onwmmousehover);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_MOUSEHWHEEL,		&wingui_input_callback_map, onwmmousewheel);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_MOUSELEAVE,			&wingui_input_callback_map, onwmmouseleave);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_MOUSEMOVE,			&wingui_input_callback_map, onwmmousemove);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_MOUSEWHEEL,			&wingui_input_callback_map, onwmmousewheel);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCHITTEST,			&wingui_input_callback_map, onwmnchittest);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCLBUTTONDBLCLK,	&wingui_input_callback_map, onwmnclbuttondblclk);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCLBUTTONDOWN,		&wingui_input_callback_map, onwmnclbuttondown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCLBUTTONUP,		&wingui_input_callback_map, onwmmclbuttonup);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCMBUTTONDBLCLK,	&wingui_input_callback_map, onwmncmbuttondblclk);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCMBUTTONDOWN,		&wingui_input_callback_map, onwmncmbuttondown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCMBUTTONUP,		&wingui_input_callback_map, onwmncmbuttonup);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCMOUSEHOVER,		&wingui_input_callback_map, onwmncmousehover);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCMOUSELEAVE,		&wingui_input_callback_map, onwmncmouseleave);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCMOUSEMOVE,		&wingui_input_callback_map, onwmncmousemove);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCRBUTTONDBLCLK,	&wingui_input_callback_map, onwmncrbuttondblclk);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCRBUTTONDOWN,		&wingui_input_callback_map, onwmncrbuttondown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCRBUTTONUP,		&wingui_input_callback_map, onwmncrbuttonup);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCXBUTTONDBLCLK,	&wingui_input_callback_map, onwmncxbuttondblclk);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCXBUTTONDOWN,		&wingui_input_callback_map, onwmncxbuttondown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_NCXBUTTONUP,		&wingui_input_callback_map, onwmncxbuttonup);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_RBUTTONDBLCLK,		&wingui_input_callback_map, onwmrbuttondblclk);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_RBUTTONDOWN,		&wingui_input_callback_map, onwmrbuttondown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_RBUTTONUP,			&wingui_input_callback_map, onwmrbuttonup);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_XBUTTONDBLCLK,		&wingui_input_callback_map, onwmxbuttondblclk);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_XBUTTONDOWN,		&wingui_input_callback_map, onwmxbuttondown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_inputing, WM_XBUTTONUP,			&wingui_input_callback_map, onwmxbuttonup);

	return SUCCESS;
}

/* {{{ PHP_MSHUTDOWN_FUNCTION(wingui_inputing) */
PHP_MSHUTDOWN_FUNCTION(wingui_inputing)
{
	zend_hash_destroy(&wingui_input_callback_map);
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
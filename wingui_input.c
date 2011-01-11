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
zend_class_entry *ce_wingui_input;
zend_class_entry *ce_wingui_keys;

/* ----------------------------------------------------------------
  Win\Gui\Input Userland API                                                      
------------------------------------------------------------------*/
ZEND_BEGIN_ARG_INFO(WinGuiInput_block_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_unblock_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getActive_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getAsyncKeyState_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getFocus_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getKeyboardLayout_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getKeyboardLayoutList_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getKeyboardLayoutName_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getKeyboardState_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getKeyboardName_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getKeyState_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getLastInput_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_loadKeyboardLayout_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_mapVirtualKey_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_send_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_setKeyboardLayout_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_unloadKeyboardLayout_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_vkKeyScan_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getCapture_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getDoubleClickTime_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_getMouseMovePoints_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_setDoubleClickTime_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, milliseconds)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiInput_swapMouseButton_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiInput_queryMouse_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 3)
    ZEND_ARG_INFO(0, hover)
    ZEND_ARG_INFO(0, leave)
    ZEND_ARG_INFO(0, nonclient)
    ZEND_ARG_INFO(0, cancel)
    ZEND_ARG_INFO(0, time)
ZEND_END_ARG_INFO()

/* {{{ proto bool Win\Gui\Input->__construct()
      Cannot extend and use Input */
PHP_METHOD(WinGuiInput, __construct)
{
	zend_throw_exception(ce_wingui_exception, "Win\\Gui\\Input cannot be constructed", 0 TSRMLS_CC);
	return;
}
/* }}} */

/* {{{ proto bool Win\Gui\Input::block()
      Turn on input blocking */
PHP_METHOD(WinGuiInput, block)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(BlockInput(TRUE));
}
/* }}} */

/* {{{ proto bool Win\Gui\Input::unblock()
      Turn off input blocking */
PHP_METHOD(WinGuiInput, unblock)
{
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(BlockInput(FALSE));
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getActive()
       Returns the currently active item or NULL */
PHP_METHOD(WinGuiInput, getActive)
{
	zend_error_handling error_handling;
	HWND handle;
	wingui_window_object *window_object;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	handle = GetActiveWindow();

	if(handle == NULL) {
		return;
	}

	window_object = (wingui_window_object *)GetWindowLongPtr(handle, GWLP_USERDATA);
	if (window_object == NULL) {
		/* TODO some day: get the window class
	    get the appropriate CE besed on the class returned */
		return;
	} else if (window_object->object_zval == NULL) {
		return;
	} else {
		zval_dtor(return_value);
		*return_value = *window_object->object_zval;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
	}
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getAsyncKeyState(int key)
       determines whether a key is up or down at the time the function is called,
	   and whether the key was pressed after a previous call to asyncKeyState */
PHP_METHOD(WinGuiInput, getAsyncKeyState)
{
	zend_error_handling error_handling;
	long key;
	SHORT value;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	value = GetAsyncKeyState(key);

	array_init(return_value);
	add_next_index_bool(return_value, value & 0x8000);
	add_next_index_bool(return_value, value & 1);
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getFocus()
       Returns the currently focused item or NULL */
PHP_METHOD(WinGuiInput, getFocus)
{
	zend_error_handling error_handling;
	HWND handle;
	wingui_window_object *window_object;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	handle = GetFocus();

	if(handle == NULL) {
		return;
	}

	window_object = (wingui_window_object *)GetWindowLongPtr(handle, GWLP_USERDATA);
	if (window_object == NULL) {
		/* TODO some day: get the window class
	    get the appropriate CE besed on the class returned */
		return;
	} else if (window_object->object_zval == NULL) {
		return;
	} else {
		zval_dtor(return_value);
		*return_value = *window_object->object_zval;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
	}
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getKeyboardLayout()
        */
PHP_METHOD(WinGuiInput, getKeyboardLayout)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getKeyboardLayoutList()
        */
PHP_METHOD(WinGuiInput, getKeyboardLayoutList)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getKeyboardLayoutName()
        */
PHP_METHOD(WinGuiInput, getKeyboardLayoutName)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getKeyboardState()
        */
PHP_METHOD(WinGuiInput, getKeyboardState)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getKeyboardName()
        */
PHP_METHOD(WinGuiInput, getKeyboardName)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getKeyState(int key)
       determines whether a key is up or down at the time the function is called,
	   and whether the key was pressed after a previous call to asyncKeyState */
PHP_METHOD(WinGuiInput, getKeyState)
{
	zend_error_handling error_handling;
	long key;
	SHORT value;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	value = GetKeyState(key);

	array_init(return_value);
	add_next_index_bool(return_value, value & 0x8000);
	add_next_index_bool(return_value, value & 1);
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getLastInput()
        */
PHP_METHOD(WinGuiInput, getLastInput)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::loadKeyboardLayout()
        */
PHP_METHOD(WinGuiInput, loadKeyboardLayout)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::mapVirtualKey()
        */
PHP_METHOD(WinGuiInput, mapVirtualKey)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::send()
        */
PHP_METHOD(WinGuiInput, send)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::setKeyboardLayout()
        */
PHP_METHOD(WinGuiInput, setKeyboardLayout)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::unloadKeyboardLayout()
        */
PHP_METHOD(WinGuiInput, unloadKeyboardLayout)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::vkKeyScan()
        */
PHP_METHOD(WinGuiInput, vkKeyScan)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getCapture()
        */
PHP_METHOD(WinGuiInput, getCapture)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::getDoubleClickTime()
        */
PHP_METHOD(WinGuiInput, getDoubleClickTime)
{
}
/* }}} */


/* {{{ proto void Win\Gui\Input::getMouseMovePoints(int key)
       retrieves a history of up to 64 previous coordinates of the mouse or pen */
PHP_METHOD(WinGuiInput, getMouseMovePoints)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Input::setDoubleClickTime(int seconds)
       sets the interval for double click values in milliseconds
	   send 0 to use the system default (500 milliseconds)
	   this is a system wide call */
PHP_METHOD(WinGuiInput, setDoubleClickTime)
{
	zend_error_handling error_handling;
	long seconds;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &seconds) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SetDoubleClickTime(seconds));
}
/* }}} */

/* {{{ proto void Win\Gui\Input::swapMouseButton(bool value)
       reverses or restores the meaning of the left and right mouse buttons
	   this is a system wide call */
PHP_METHOD(WinGuiInput, swapMouseButton)
{
	zend_error_handling error_handling;
	zend_bool value;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &value) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SwapMouseButton(value));
}
/* }}} */

/* {{{ proto array Win\Gui\Inputing->queryMouse(hover, leave, nonclient[, cancel, time])
        get information about the last mouse event */
PHP_METHOD(WinGuiInput, queryMouse)
{
}
/* }}} */

/* {{{ all static methods */
static zend_function_entry wingui_input_functions[] = {
	PHP_ME(WinGuiInput, block, WinGuiInput_block_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, unblock, WinGuiInput_unblock_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getActive, WinGuiInput_getActive_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getAsyncKeyState, WinGuiInput_getAsyncKeyState_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getFocus, WinGuiInput_getFocus_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getKeyboardLayout, WinGuiInput_getKeyboardLayout_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getKeyboardLayoutList, WinGuiInput_getKeyboardLayoutList_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getKeyboardLayoutName, WinGuiInput_getKeyboardLayoutName_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getKeyboardState, WinGuiInput_getKeyboardState_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getKeyboardName, WinGuiInput_getKeyboardName_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getKeyState, WinGuiInput_getKeyState_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getLastInput, WinGuiInput_getLastInput_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, loadKeyboardLayout, WinGuiInput_loadKeyboardLayout_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, mapVirtualKey, WinGuiInput_mapVirtualKey_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, send, WinGuiInput_send_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, setKeyboardLayout, WinGuiInput_setKeyboardLayout_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, unloadKeyboardLayout, WinGuiInput_unloadKeyboardLayout_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, vkKeyScan, WinGuiInput_vkKeyScan_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getCapture, WinGuiInput_getCapture_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getDoubleClickTime, WinGuiInput_getDoubleClickTime_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, getMouseMovePoints, WinGuiInput_getMouseMovePoints_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, setDoubleClickTime, WinGuiInput_setDoubleClickTime_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, swapMouseButton, WinGuiInput_swapMouseButton_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiInput, queryMouse, WinGuiInput_queryMouse_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	// TODO: add register and unregister hot key that isn't window specific
	{NULL, NULL, NULL}
};
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Input LifeCycle Functions                                                    
------------------------------------------------------------------*/

/* {{{ register classes and all the keys */
PHP_MINIT_FUNCTION(wingui_input)
{
	zend_class_entry ce, key_ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Input", wingui_input_functions);
	ce_wingui_input = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_input->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

	INIT_NS_CLASS_ENTRY(key_ce, PHP_WINGUI_NS, "Key", NULL);
	ce_wingui_keys = zend_register_internal_class(&key_ce TSRMLS_CC);
	ce_wingui_keys->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LBUTTON", VK_LBUTTON);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "RBUTTON", VK_RBUTTON);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "CANCEL", VK_CANCEL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MBUTTON", VK_MBUTTON);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "XBUTTON1", VK_XBUTTON1);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "XBUTTON2", VK_XBUTTON2);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "BACK", VK_LBUTTON);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "TAB", VK_RBUTTON);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "CLEAR", VK_CLEAR);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "RETURN", VK_RETURN);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "SHIFT", VK_SHIFT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "CONTROL", VK_CONTROL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MENU", VK_MENU);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "PAUSE", VK_PAUSE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "CAPITAL", VK_CAPITAL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "KANA", VK_KANA);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "HANGUL", VK_HANGUL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "JUNJA", VK_JUNJA);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "FINAL", VK_FINAL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "HANJA", VK_HANJA);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "KANJI", VK_KANJI);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "ESCAPE", VK_ESCAPE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "CONVERT", VK_CONVERT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NONCONVERT", VK_NONCONVERT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "ACCEPT", VK_ACCEPT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MODECHANGE", VK_MODECHANGE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "SPACE", VK_SPACE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "PRIOR", VK_PRIOR);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NEXT", VK_NEXT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "END", VK_END);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "HOME", VK_HOME);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LEFT", VK_LEFT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "UP", VK_UP);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "RIGHT", VK_RIGHT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "DOWN", VK_DOWN);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "SELECT", VK_SELECT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "PRINT", VK_PRINT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "EXECUTE", VK_EXECUTE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "SNAPSHOT", VK_SNAPSHOT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "INSERT", VK_INSERT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "DELETE", VK_DELETE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "HELP", VK_HELP);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_0", 0x30);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_1", 0x31);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_2", 0x32);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_3", 0x33);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_4", 0x34);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_5", 0x35);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_6", 0x36);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_7", 0x37);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_8", 0x38);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "_9", 0x39);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "A", 0x41);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "B", 0x42);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "C", 0x43);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "D", 0x44);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "E", 0x45);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F", 0x46);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "G", 0x47);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "H", 0x48);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "I", 0x49);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "J", 0x4A);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "K", 0x4B);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "L", 0x4C);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "M", 0x4D);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "N", 0x4E);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "O", 0x4F);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "P", 0x50);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "Q", 0x51);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "R", 0x52);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "S", 0x53);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "T", 0x54);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "U", 0x55);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "V", 0x56);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "W", 0x57);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "X", 0x58);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "Y", 0x59);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "Z", 0x5A);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LWIN", VK_LWIN);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "RWIN", VK_RWIN);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "APPS", VK_APPS);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "SLEEP", VK_SLEEP);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD0", VK_NUMPAD0);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD1", VK_NUMPAD1);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD2", VK_NUMPAD2);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD3", VK_NUMPAD3);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD4", VK_NUMPAD4);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD5", VK_NUMPAD5);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD6", VK_NUMPAD6);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD7", VK_NUMPAD7);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD8", VK_NUMPAD8);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMPAD9", VK_NUMPAD9);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MULTIPLY", VK_MULTIPLY);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "ADD", VK_ADD);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "SEPARATOR", VK_SEPARATOR);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "DECIMAL", VK_DECIMAL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "DIVIDE", VK_DIVIDE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F1", VK_F1);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F2", VK_F2);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F3", VK_F3);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F4", VK_F4);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F5", VK_F5);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F6", VK_F6);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F7", VK_F7);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F8", VK_F8);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F9", VK_F9);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F10", VK_F10);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F12", VK_F12);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F13", VK_F13);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F14", VK_F14);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F15", VK_F15);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F16", VK_F16);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F17", VK_F17);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F18", VK_F18);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F19", VK_F19);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F20", VK_F20);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F21", VK_F21);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F22", VK_F22);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F23", VK_F23);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "F24", VK_F24);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NUMLOCK", VK_NUMLOCK);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "SCROLL", VK_SCROLL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LSHIFT", VK_LSHIFT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "RSHIFT", VK_RSHIFT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LCONTROL", VK_LCONTROL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "RCONTROL", VK_RCONTROL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LMENU", VK_LMENU);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "RMENU", VK_RMENU);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "BROWSER_BACK", VK_BROWSER_BACK);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "BROWSER_FORWARD", VK_BROWSER_FORWARD);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "BROWSER_REFRESH", VK_BROWSER_REFRESH);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "BROWSER_STOP", VK_BROWSER_STOP);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "BROWSER_SEARCH", VK_BROWSER_SEARCH);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "BROWSER_FAVORITES", VK_BROWSER_FAVORITES);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "BROWSER_HOME", VK_BROWSER_HOME);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "VOLUME_MUTE", VK_VOLUME_MUTE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "VOLUME_DOWN", VK_VOLUME_DOWN);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "VOLUME_UP", VK_VOLUME_UP);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MEDIA_NEXT_TRACK", VK_MEDIA_NEXT_TRACK);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MEDIA_PREV_TRACK", VK_MEDIA_PREV_TRACK);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MEDIA_STOP", VK_MEDIA_STOP);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MEDIA_PLAY_PAUSE", VK_MEDIA_PLAY_PAUSE);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LAUNCH_MAIL", VK_LAUNCH_MAIL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LAUNCH_MEDIA_SELECT", VK_LAUNCH_MEDIA_SELECT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LAUNCH_APP1", VK_LAUNCH_APP1);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "LAUNCH_APP2", VK_LAUNCH_APP2);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_1", VK_OEM_1);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_PLUS", VK_OEM_PLUS);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_COMMA", VK_OEM_COMMA);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_MINUS", VK_OEM_MINUS);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_PERIOD", VK_OEM_PERIOD);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_2", VK_OEM_2);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_3", VK_OEM_3);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_4", VK_OEM_4);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_5", VK_OEM_5);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_6", VK_OEM_6);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_7", VK_OEM_7);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_8", VK_OEM_8);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_102", VK_OEM_102);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "PROCESSKEY", VK_PROCESSKEY);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "PACKET", VK_PACKET);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "ATTN", VK_ATTN);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "CRSEL", VK_CRSEL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "EXSEL", VK_EXSEL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "EREOF", VK_EREOF);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "PLAY", VK_PLAY);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "ZOOM", VK_ZOOM);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "NONAME", VK_NONAME);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "PA1", VK_PA1);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "OEM_CLEAR", VK_OEM_CLEAR);

	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MOD_ALT", MOD_ALT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MOD_CONTROL", MOD_CONTROL);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MOD_SHIFT", MOD_SHIFT);
	REGISTER_WINGUI_NAMED_CONST(ce_wingui_keys, "MOD_WIN", MOD_WIN);

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
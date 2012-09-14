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

#ifndef IMPLEMENT_INPUTING_H
#define IMPLEMENT_INPUTING_H

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

extern PHP_METHOD(WinGuiInputing, enable);
extern PHP_METHOD(WinGuiInputing, disable);
extern PHP_METHOD(WinGuiInputing, isEnabled);
extern PHP_METHOD(WinGuiInputing, activate);
extern PHP_METHOD(WinGuiInputing, deactivate);
extern PHP_METHOD(WinGuiInputing, isActive);
extern PHP_METHOD(WinGuiInputing, focus);
extern PHP_METHOD(WinGuiInputing, unfocus);
extern PHP_METHOD(WinGuiInputing, hasFocus);
extern PHP_METHOD(WinGuiInputing, registerHotKey);
extern PHP_METHOD(WinGuiInputing, unregisterHotKey);
extern PHP_METHOD(WinGuiInputing, capture);
extern PHP_METHOD(WinGuiInputing, release);
extern PHP_METHOD(WinGuiInputing, hasCapture);
extern PHP_METHOD(WinGuiInputing, trackMouse);
extern PHP_METHOD(WinGuiInputing, dragDetect);

/* All the classes in this file */
extern zend_class_entry *ce_wingui_inputing;
extern HashTable wingui_input_callback_map;

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
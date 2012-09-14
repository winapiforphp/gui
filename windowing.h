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

#ifndef IMPLEMENT_WINDOWING_H
#define IMPLEMENT_WINDOWING_H

ZEND_BEGIN_ARG_INFO(WinGuiWindowing_adjustSize_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, height)
	ZEND_ARG_INFO(0, width)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindowing_animateShow_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, time)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, activate)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindowing_animateHide_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, time)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindowing_getNext_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindowing_getPrevious_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindowing_isVisible_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindowing_hide_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, async)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindowing_show_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, noactivate)
	ZEND_ARG_INFO(0, async)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindowing_getSize_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindowing_getPos_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindowing_getText_args, ZEND_SEND_BY_VAL)
ZEND_END_ARG_INFO()

extern PHP_METHOD(WinGuiWindowing, adjustSize);
extern PHP_METHOD(WinGuiWindowing, animateShow);
extern PHP_METHOD(WinGuiWindowing, animateHide);
extern PHP_METHOD(WinGuiWindowing, getNext);
extern PHP_METHOD(WinGuiWindowing, getPrevious);


extern PHP_METHOD(WinGuiWindowing, hide);
extern PHP_METHOD(WinGuiWindowing, show);
extern PHP_METHOD(WinGuiWindowing, isVisible);
extern PHP_METHOD(WinGuiWindowing, getSize);
extern PHP_METHOD(WinGuiWindowing, getPos);
extern PHP_METHOD(WinGuiWindowing, getText);

/* All the classes in this file */
extern zend_class_entry *ce_wingui_windowing;

/* Notice this is not static, other things gonna use it */
extern HashTable wingui_windowing_prop_handlers;

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
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

#ifndef IMPLEMENT_MESSAGING_H
#define IMPLEMENT_MESSAGING_H

ZEND_BEGIN_ARG_INFO(WinGuiMessaging_connect_args, ZEND_SEND_BY_VAL)
		ZEND_ARG_INFO(0, msg)
		ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessaging_connectFull_args, ZEND_SEND_BY_VAL)
		ZEND_ARG_INFO(0, msg)
		ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessaging_connectSimple_args, ZEND_SEND_BY_VAL)
		ZEND_ARG_INFO(0, msg)
		ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessaging_disconnect_args, ZEND_SEND_BY_VAL)
		ZEND_ARG_INFO(0, msg)
		ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiMessaging_get_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, filtermin)
	ZEND_ARG_INFO(0, filtermax)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiMessaging_peek_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, filtermin)
	ZEND_ARG_INFO(0, filtermax)
	ZEND_ARG_INFO(0, remove)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessaging_post_args, 0)
	ZEND_ARG_INFO(0, msg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessaging_send_args, 0)
	ZEND_ARG_INFO(0, msg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessaging_sendNotify_args, 0)
	ZEND_ARG_INFO(0, msg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessaging_sendCallback_args, 0)
	ZEND_ARG_INFO(0, msg)
	ZEND_ARG_INFO(0, callback)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiMessaging_sendTimeout_args, 0)
	ZEND_ARG_INFO(0, msg)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO()


extern PHP_METHOD(WinGuiMessaging, connect);
extern PHP_METHOD(WinGuiMessaging, connectFull);
extern PHP_METHOD(WinGuiMessaging, connectSimple);
extern PHP_METHOD(WinGuiMessaging, disconnect);
extern PHP_METHOD(WinGuiMessaging, get);
extern PHP_METHOD(WinGuiMessaging, post);
extern PHP_METHOD(WinGuiMessaging, peek);
extern PHP_METHOD(WinGuiMessaging, send);
extern PHP_METHOD(WinGuiMessaging, sendNotify);
extern PHP_METHOD(WinGuiMessaging, sendTimeout);
extern PHP_METHOD(WinGuiMessaging, sendCallback);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
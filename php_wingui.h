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

#ifndef PHP_WINGUI_H
#define PHP_WINGUI_H

/* ----------------------------------------------------------------
  Setup and includes                                              
------------------------------------------------------------------*/
#undef _WIN32_WINNT
#undef NTDDI_VERSION
#define _WIN32_WINNT 0x0600 /* Vista */
#define NTDDI_VERSION  NTDDI_VISTA /* Vista */

#define _WINSOCKAPI_ /* keep windows.h from spewing errors */
#define ISOLATION_AWARE_ENABLED          1
#define OEMRESOURCE                      1
#include <Windows.h> /* Include this FIRST we want vista stuff! */
#include <commctrl.h>

/* version info file */
#include "version.h"

/* Needed PHP includes */
#include <php.h>
#include <zend_exceptions.h>
#include <ext/spl/spl_exceptions.h>

/* winsystem stuff */
#include <php_winsystem_public.h>

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef WIN32
#define PHP_WINGUI_API __declspec(dllexport)
#else
#define PHP_WINGUI_API
#endif

/* ----------------------------------------------------------------
  Typedefs
------------------------------------------------------------------*/
#define PHP_WINGUI_NS ZEND_NS_NAME("Win", "Gui")

#define PHP_WINGUI_EXCEPTIONS \
zend_error_handling error_handling; \
zend_replace_error_handling(EH_THROW, ce_winsystem_invalidargumentexception, &error_handling TSRMLS_CC);

#define PHP_WINGUI_RESTORE_ERRORS \
zend_restore_error_handling(&error_handling TSRMLS_CC);

#define REGISTER_ENUM_CONST(const_name, value, ce) \
zend_declare_class_constant_long(ce, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);

/* struct typedefs */
typedef struct _wingui_window_object wingui_window_object;
typedef struct _wingui_callback_t wingui_callback_t;
typedef struct _wingui_message_object wingui_message_object;

/* ----------------------------------------------------------------
  Class Entries                                              
------------------------------------------------------------------*/
extern zend_class_entry *ce_wingui_window;

/* ----------------------------------------------------------------
  Macros                                               
------------------------------------------------------------------*/
LRESULT CALLBACK wingui_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define PHP_WINGUI_STOP_LOOP WM_APP + 1

#define REGISTER_WINGUI_MESSAGE_CONSTANT(ce, val, map, cb) \
	zend_declare_class_constant_long((ce), #val, sizeof(#val) - 1, (val) TSRMLS_CC); \
	zend_hash_index_update((map), (val), (void*)#cb, sizeof(#cb) + 2, NULL);

/* ----------------------------------------------------------------
  C API                                             
------------------------------------------------------------------*/
HWND wingui_window_get_handle(zval** window TSRMLS_DC);
HashTable *wingui_window_get_callbacks(zval** window TSRMLS_DC);
#ifdef ZTS
void*** wingui_window_get_tsrm(wingui_window_object *window_object);
#endif
HashTable *wingui_window_get_callbacks_struct(wingui_window_object *window_object);
WNDPROC wingui_window_get_proc(wingui_window_object *window_object);

zend_object_value wingui_window_object_create_ex(zend_class_entry *ce, zend_object_handlers *handlers TSRMLS_DC);
void wingui_window_object_free(void *object TSRMLS_DC);
void wingui_window_create_new(wingui_window_object *window_object, HWND window_handle, zend_bool use_unicode);

int wingui_window_get_size(HashTable *options, long *x, long *y, long *width, long *height TSRMLS_DC);
/* ----------------------------------------------------------------
  Object Globals, lifecycle and static linking                 
------------------------------------------------------------------*/

/* Lifecycle Function Declarations */
PHP_MINIT_FUNCTION(wingui);
PHP_MSHUTDOWN_FUNCTION(wingui);
PHP_RINIT_FUNCTION(wingui);
PHP_MINFO_FUNCTION(wingui);

/* Enums */
PHP_MINIT_FUNCTION(wingui_windowmessage);

/* Interfaces */
PHP_MINIT_FUNCTION(wingui_messaging);
PHP_MINIT_FUNCTION(wingui_inputing);
PHP_MINIT_FUNCTION(wingui_windowing);

/* Tools */
PHP_MINIT_FUNCTION(wingui_message_queue);
PHP_MINIT_FUNCTION(wingui_message);
PHP_MINIT_FUNCTION(wingui_window);

/* Controls */
PHP_MINIT_FUNCTION(wingui_control);
PHP_MINIT_FUNCTION(wingui_control_static);

/* Shutdowns */
PHP_MSHUTDOWN_FUNCTION(wingui_window);

/* Required for static linking */
extern zend_module_entry wingui_module_entry;
#define phpext_wingui_ptr &wingui_module_entry

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
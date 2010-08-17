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

#ifndef PHP_WINGUI_H
#define PHP_WINGUI_H

/* ----------------------------------------------------------------
  Setup and includes                                                  
------------------------------------------------------------------*/

/* We want the fancy common controls api stuff for newer versions
The isolation aware crap messes up our code coverage, but allows for 
pre-xp versions of common controls to work, as well as new versions */
#define _WIN32_WINNT                     0x0600
#define _WIN32_IE                        0x0600
#define ISOLATION_AWARE_ENABLED          1
#define OEMRESOURCE						 1

#include "wingui_version.h"
#include "php.h"
#include <commctrl.h>

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

/* generic object - for all objects */
typedef struct _wingui_generic_object {
	zend_object  std;
	zend_object_handle handle;
	HashTable *prop_handler;
} wingui_generic_object;

/* object for menu resources */
typedef struct _wingui_menu_object {
	zend_object  std;
	zend_object_handle handle;
	HashTable *prop_handler;
	HMENU menu_handle;
	HWND window_handle;
} wingui_menu_object;

/* object for menuitem structs */
typedef struct _wingui_menuitem_object {
	zend_object  std;
	zend_object_handle handle;
	HashTable *prop_handler;
	MENUITEMINFO *menuiteminfo;
	HMENU parent_menu;
	zval *bitmap_object;
	zval *checkbitmap_object;
	zval *uncheckbitmap_object;
} wingui_menuitem_object;

/* object for imagelists */
typedef struct _wingui_imagelist_object {
	zend_object  std;
	zend_object_handle handle;
	HIMAGELIST imagelist;
} wingui_imagelist_object;

/* struct for statusbar_control specific data, used in window_object union */
typedef struct _wingui_statusbar_data {
	long tip_text_len[256];
	zval *icon_zval[256];
} wingui_statusbar_data;

/* struct for window specific data, used in window_object union */
typedef struct _wingui_window_data {
	zval *image;
	zval *menu;
} wingui_window_data;

/* struct for listview specific data, used in window_object union */
typedef struct _wingui_listview_data {
	zval *imagelist;
} wingui_listview_data;

/* Some typedefs for the messages callbacks */
typedef void (*wingui_messages_pack_t)(int msg, WPARAM *wParam, LPARAM *lParam, zval ***extra TSRMLS_DC);
typedef zval ***(*wingui_messages_crack_t)(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC);
typedef LRESULT (*wingui_messages_results_t)(int msg, zval *return_value TSRMLS_DC);
typedef void (*wingui_messages_zvals_t)(int msg, LRESULT lresult, zval *return_value TSRMLS_DC);
typedef int (*wingui_messages_dispatch_t)(INTERNAL_FUNCTION_PARAMETERS, int msg, WPARAM *wParam, LPARAM *lParam);

/* generic HWND handle object, used for most objects including controls */
typedef struct _wingui_window_object {
	zend_object  std;
	zend_object_handle handle;
	HashTable *prop_handler;
#ifdef ZTS
	TSRMLS_D;
#endif
	WNDPROC window_proc;
	wingui_messages_pack_t messages_packer;
	wingui_messages_crack_t messages_cracker;
	wingui_messages_results_t messages_results;
	wingui_messages_zvals_t messages_zvals;
	wingui_messages_dispatch_t messages_dispatch;
	HashTable *callback_map;
	HashTable *registered_callbacks;
	zval *object_zval;
	int parent_is_temp;
	HWND window_handle;
	union data {
		wingui_statusbar_data statusbar;
		wingui_window_data window;
		wingui_listview_data listview;
	} data;
} wingui_window_object;

/* Some typedefs for stuff in the property callbacks */
typedef int (*wingui_prop_read_int_t)(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC);
typedef int (*wingui_prop_write_t)(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC);

/* Storage container for property callbacks */
typedef struct _wingui_prop_handler {
	wingui_prop_read_int_t read_func;
	wingui_prop_write_t write_func;
} wingui_prop_handler;

/* Storage container for message callbacks */
typedef struct _wingui_callback_t {
	zend_fcall_info callback_info;
	zend_fcall_info_cache callback_cache;
	char *src_filename;
	uint src_lineno;
	int send_return;
	int send_args;
} wingui_callback_t;

/* Image/Resource objects */
typedef struct _wingui_resource_object {
	zend_object  std;
	zend_object_handle handle;
	HGDIOBJ resource_handle;
	zend_bool shared;
} wingui_resource_object;

/* ----------------------------------------------------------------
  Class Entries                                              
------------------------------------------------------------------*/
extern zend_class_entry *ce_wingui_exception;
extern zend_class_entry *ce_wingui_argexception;
extern zend_class_entry *ce_wingui_versionexception;

extern zend_class_entry *ce_wingui_messaging;
extern zend_class_entry *ce_wingui_inputing;
extern zend_class_entry *ce_wingui_windowing;

extern zend_class_entry *ce_wingui_menu;
extern zend_class_entry *ce_wingui_menuitem;

extern zend_class_entry *ce_wingui_control;

extern zend_class_entry *ce_wingui_resource_image;
extern zend_class_entry *ce_wingui_resource_icon;
extern zend_class_entry *ce_wingui_resource_bitmap;
extern zend_class_entry *ce_wingui_imagelist;

/* ----------------------------------------------------------------
  Valid object checkers                                            
------------------------------------------------------------------*/
extern zend_object_handlers wingui_object_handlers;
extern HashTable wingui_control_prop_handlers;

static inline wingui_window_object* wingui_window_object_get(zval *zobj TSRMLS_DC)
{
    wingui_window_object *pobj = zend_object_store_get_object(zobj TSRMLS_CC);
    if (pobj->window_handle == NULL) {
		php_error(E_ERROR, "Internal window handle missing in %s class, you must call parent::__construct in extended classes", Z_OBJCE_P(zobj)->name);
		return NULL;
    }
	if (IsWindow(pobj->window_handle) == 0) {
		php_error(E_RECOVERABLE_ERROR, "Internal window handle is no longer valid");
		return NULL;
	}
    return pobj;
}

static inline wingui_window_object* wingui_window_object_juggle(wingui_generic_object *object)
{
    wingui_window_object *pobj = (wingui_window_object *) object;
	if (pobj->window_handle == NULL) {
		php_error(E_ERROR, "Internal window handle missing, you must call parent::__construct in extended classes");
		return NULL;
    }
	if (IsWindow(pobj->window_handle) == 0) {
		php_error(E_RECOVERABLE_ERROR, "Internal window handle is no longer valid");
		return NULL;
	}
    return pobj;
}

static inline wingui_menu_object* wingui_menu_object_get(zval *zobj TSRMLS_DC)
{
    wingui_menu_object *pobj = zend_object_store_get_object(zobj TSRMLS_CC);
    if (pobj->menu_handle == NULL) {
		php_error(E_ERROR, "Internal menu handle missing in %s class, you must call parent::__construct in extended classes", Z_OBJCE_P(zobj)->name);
		return NULL;
    }
	if (IsMenu(pobj->menu_handle) == 0) {
		php_error(E_ERROR, "Internal menu handle is no longer valid");
		return NULL;
	}
    return pobj;
}

static inline wingui_menuitem_object* wingui_menuitem_object_get(zval *zobj TSRMLS_DC)
{
    wingui_menuitem_object *pobj = zend_object_store_get_object(zobj TSRMLS_CC);
    if (pobj->menuiteminfo == NULL) {
		php_error(E_ERROR, "Internal menuitem data missing in %s class, you must call parent::__construct in extended classes", Z_OBJCE_P(zobj)->name);
		return NULL;
    }
    return pobj;
}

static inline wingui_menuitem_object* wingui_menuitem_object_juggle(wingui_generic_object *object)
{
    wingui_menuitem_object *pobj = (wingui_menuitem_object *) object;
    if (pobj->menuiteminfo == NULL) {
		php_error(E_ERROR, "Internal menuitem data missing, you must call parent::__construct in extended classes");
		return NULL;
    }
    return pobj;
}

static inline wingui_imagelist_object* wingui_imagelist_object_get(zval *zobj TSRMLS_DC)
{
    wingui_imagelist_object *pobj = zend_object_store_get_object(zobj TSRMLS_CC);
    if (pobj->imagelist == NULL) {
		php_error(E_ERROR, "Internal imagelist data missing in %s class, you must call parent::__construct in extended classes", Z_OBJCE_P(zobj)->name);
		return NULL;
    }
    return pobj;
}

static inline wingui_resource_object* wingui_resource_object_get(zval *zobj TSRMLS_DC)
{
    wingui_resource_object *pobj = zend_object_store_get_object(zobj TSRMLS_CC);
    if (pobj->resource_handle == NULL) {
		php_error(E_ERROR, "Internal resource handle missing in %s class, you must call parent::__construct in extended classes", Z_OBJCE_P(zobj)->name);
		return NULL;
    }
    return pobj;
}

static inline void wingui_register_prop_handler(HashTable *prop_handlers, zend_class_entry *ce, char *name, wingui_prop_read_int_t read_func, wingui_prop_write_t write_func TSRMLS_DC)
{
	wingui_prop_handler hnd;

	hnd.read_func = read_func;
	hnd.write_func = write_func;
	zend_hash_add(prop_handlers, name, strlen(name)+1, &hnd, sizeof(wingui_prop_handler), NULL);
	zend_declare_property_null(ce, name, strlen(name), ZEND_ACC_PUBLIC TSRMLS_CC);
}

static inline zval ***wingui_callback_extra_zvals_ctor(int argc)
{
	zval ***ret = safe_emalloc(argc, sizeof(zval**), 0);
	int i;

	for(i = 0; i < argc; i++) {
		ret[i] = emalloc(sizeof(zval*));
		ALLOC_INIT_ZVAL(*ret[i]);
	}

	return ret;
}

static inline void wingui_callback_extra_zvals_dtor(int argc, zval ***argv)
{
	int i;

	for(i = 0; i < argc; i++) {
		zval_ptr_dtor(argv[i]);
		efree(argv[i]);
	}

	efree(argv);
}

/* ----------------------------------------------------------------
  Macros                                               
------------------------------------------------------------------*/
#define PHP_WINGUI_NS ZEND_NS_NAME("Win", "Gui")

#define REGISTER_WINGUI_NAMED_CONST(ce, name, value) \
	zend_declare_class_constant_long(ce, #name, sizeof(#name)-1, (long)value TSRMLS_CC);
#define REGISTER_WINGUI_CONSTANT(ce, val) \
	zend_declare_class_constant_long((ce), #val, sizeof(#val) - 1, (long)(val) TSRMLS_CC);
#define REGISTER_WINGUI_MESSAGE_CONSTANT(ce, val, map, cb) \
	zend_declare_class_constant_long((ce), #val, sizeof(#val) - 1, (val) TSRMLS_CC); \
	zend_hash_index_update((map), (val), (void*)#cb, sizeof(#cb) + 2, NULL);

/* ----------------------------------------------------------------
  C API                                             
------------------------------------------------------------------*/
BOOL wingui_is_win7 (TSRMLS_D);
BOOL wingui_is_vista (TSRMLS_D);
BOOL wingui_is_xp(TSRMLS_D);

void wingui_create_error(int error, zend_class_entry *ce TSRMLS_DC);
int wingui_juggle_type(zval *value, int type TSRMLS_DC);
LRESULT CALLBACK wingui_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
zval* wingui_winproc_callback_dispatch(wingui_window_object *window_object, int msg, zval ***extra, int extra_count, zend_bool *stop_default TSRMLS_DC);

void wingui_messaging_destructor_helper(HashTable *registered_callbacks TSRMLS_DC);
int wingui_messaging_connect_helper(HashTable* callback_table, int message_code, zval*** args, int argc, zend_fcall_info finfo, zend_fcall_info_cache fcache, int send_args, int send_return TSRMLS_DC);
int unset_abstract_flag(zend_function *func TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key);

void wingui_window_object_destructor(void *object TSRMLS_DC);

zval ***wingui_window_messages_cracker(int msg, WPARAM *wParam, LPARAM *lParam, int *extra_count TSRMLS_DC);
/* ----------------------------------------------------------------
  Object Globals, lifecycle and static linking                                                
------------------------------------------------------------------*/

/* Global vars */
ZEND_BEGIN_MODULE_GLOBALS(wingui)
	HINSTANCE hinstance;
	BOOL end_main;
	char* common_controls;
	char* common_dialog;
ZEND_END_MODULE_GLOBALS(wingui)

#ifdef ZTS
# define WINGUI_G(v)   TSRMG(wingui_globals_id, zend_wingui_globals*, v)
#else
# define WINGUI_G(v)   (wingui_globals.v)
#endif

/* Lifecycle Function Declarations */
PHP_MINIT_FUNCTION(wingui);
PHP_MSHUTDOWN_FUNCTION(wingui);
PHP_RINIT_FUNCTION(wingui);
PHP_MINFO_FUNCTION(wingui);
PHP_MINIT_FUNCTION(wingui_util);

/* Interfaces */
PHP_MINIT_FUNCTION(wingui_messaging);
PHP_MINIT_FUNCTION(wingui_inputing);
PHP_MINIT_FUNCTION(wingui_windowing);

/* Tools */
PHP_MINIT_FUNCTION(wingui_message);
PHP_MINIT_FUNCTION(wingui_input);
PHP_MINIT_FUNCTION(wingui_window);

/* Resources*/
PHP_MINIT_FUNCTION(wingui_resource_bitmap);
PHP_MINIT_FUNCTION(wingui_resource_icon);
PHP_MINIT_FUNCTION(wingui_resource_cursor);
PHP_MINIT_FUNCTION(wingui_imagelist);
PHP_MINIT_FUNCTION(wingui_menu);

/* Controls */
PHP_MINIT_FUNCTION(wingui_control);
PHP_MINIT_FUNCTION(wingui_control_button);
PHP_MINIT_FUNCTION(wingui_control_listview);
PHP_MINIT_FUNCTION(wingui_control_static);
PHP_MINIT_FUNCTION(wingui_control_statusbar);

/* Shutdowns */
PHP_MSHUTDOWN_FUNCTION(wingui_inputing);
PHP_MSHUTDOWN_FUNCTION(wingui_window);
PHP_MSHUTDOWN_FUNCTION(wingui_control);
PHP_MSHUTDOWN_FUNCTION(wingui_control_statusbar);
PHP_MSHUTDOWN_FUNCTION(wingui_control_listview);

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
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
#include "ext/spl/spl_exceptions.h"

ZEND_DECLARE_MODULE_GLOBALS(wingui);

zend_class_entry *ce_wingui_exception;
zend_class_entry *ce_wingui_argexception;
zend_class_entry *ce_wingui_versionexception;
zend_object_handlers wingui_object_handlers;

/* {{{ winapi_create_error - grabs a message from GetLastError int and throws an exception with it */
void wingui_create_error(int error, zend_class_entry *ce TSRMLS_DC)
{
	DWORD ret;
	char * buffer = NULL;

	/* Get a system message from the getlasterror value, tell windows to allocate the buffer, and don't
	   sprintf in any args */
	ret = FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER |           
		FORMAT_MESSAGE_IGNORE_INSERTS |            
		FORMAT_MESSAGE_FROM_SYSTEM,                
		NULL,                       
		error,                                     
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&buffer,                           
		0,
		NULL);

	if (ret == 0) {
		zend_throw_exception(ce, "Cannot retrieve error message properly", GetLastError() TSRMLS_CC);
		return;
	}

	zend_throw_exception(ce, buffer, error TSRMLS_CC);
	LocalFree(buffer);
}
/* }}} */

/* {{{ wingui_juggle_type - takes a zval and juggles the type */
int wingui_juggle_type(zval *value, int type TSRMLS_DC)
{
	zval tmp_value;

	switch(type)
	{
		case IS_BOOL:
			if (Z_TYPE_P(value) != IS_BOOL) {
				tmp_value = *value;
				zval_copy_ctor(&tmp_value);
				convert_to_boolean(&tmp_value);
				value = &tmp_value;
			}
			return SUCCESS;
		case IS_LONG:
			if (Z_TYPE_P(value) != IS_LONG) {
				tmp_value = *value;
				zval_copy_ctor(&tmp_value);
				convert_to_long(&tmp_value);
				value = &tmp_value;
			}
			return SUCCESS;
		case IS_STRING:
			if (Z_TYPE_P(value) != IS_STRING) {
				tmp_value = *value;
				zval_copy_ctor(&tmp_value);
				convert_to_string(&tmp_value);
				value = &tmp_value;
			}
			return SUCCESS;
	}
	return FAILURE;
}
/* }}} */

/* {{{ unset_abstract_flag - sharing interface definitions amoung objects that implement them */
int unset_abstract_flag(zend_function *func TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
{
    zend_class_entry *iface_ce = va_arg(args, zend_class_entry *);

    if (func->type == ZEND_INTERNAL_FUNCTION) {
        if (zend_hash_quick_exists(&iface_ce->function_table, hash_key->arKey, hash_key->nKeyLength, hash_key->h)) {
            ((zend_internal_function*)func)->fn_flags &= ~ZEND_ACC_ABSTRACT;
        }
    }

    return ZEND_HASH_APPLY_KEEP;
}
/* }}} */

/* Does the callback dispatching 
     The dispatch happens in two steps - first it calls any attached callbacks
	 in order (remember attached messages are pushed onto the top so FILO order)
	 and then any "default" registered handlers are called
	 
	 This can be short circuited any time by a "true" return from a callback, which
	 will halt ALL the bubbling.  Return values are available if a callback is connected
	 with the full variant or default callback ONLY */
zval* wingui_winproc_callback_dispatch(wingui_window_object *window_object, int msg, zval ***extra,
	int extra_count, zend_bool *stop_default TSRMLS_DC)
{
	zval *return_value = NULL, ***param_list = NULL;
	HashTable **callbacks = NULL;
	int param_list_count = extra_count + 1;
	const char *callback_method;

	/* Attached callbacks are used first */
	if((zend_hash_num_elements(window_object->registered_callbacks) > 0) &&
		(zend_hash_index_find(window_object->registered_callbacks, msg, (HashTable **) &callbacks) == SUCCESS)) {
		HashPosition pos;
		wingui_callback_t **callback;

		/* iterate BACKWARD and call and optionally bail if bubble ends up as true */ 
		for(zend_hash_internal_pointer_end_ex(*callbacks, &pos);
			zend_hash_get_current_data_ex(*callbacks, (wingui_callback_t **) &callback, &pos) == SUCCESS;
			zend_hash_move_backwards_ex(*callbacks, &pos)) {

			/* in loop declarations */
			zend_fcall_info *finfo = &((*callback)->callback_info);
			zval ***orig_params = finfo->params, ***params = orig_params;
			int orig_params_count = finfo->param_count, param_count = orig_params_count;
			zval *bubble_value = NULL;

			finfo->retval_ptr_ptr = &bubble_value;

			/* The parameters list is only created/allocated on demand but is used for all callbacks */
			if (!param_list) {
				param_list = safe_emalloc(param_list_count, sizeof(zval**), 0);
				param_list[0] = emalloc(sizeof(zval*));
				ALLOC_INIT_ZVAL(*param_list[0]);
				Z_ISREF_P(*param_list[0]);
				memcpy(param_list + 1, extra, extra_count * sizeof(zval**));
			}

			/* start the crazy args manipulation
			send_args is true, send_return is true, orig_params_count is > 0 */
			if ((*callback)->send_args && (*callback)->send_return && orig_params_count) {
				/* Two sets of parameters to mix with param_list version */
				param_count = param_list_count + orig_params_count;
				params = safe_emalloc(param_count, sizeof(zval**), 0);

				memcpy(params, param_list, param_list_count * sizeof(zval**));
				memcpy(params + param_list_count, orig_params, orig_params_count * sizeof(zval**));

			/* send_args is true, send_return is true, orig_params_count is 0 */
			} else if((*callback)->send_args && (*callback)->send_return) {
				param_count = param_list_count;
				params = param_list;
			/* send_args is true, send_return is false, orig_params_count > 0 */
			} else if((*callback)->send_args && orig_params_count && extra_count) {
				/* Two sets of parameters to mix with extra version */
				param_count = extra_count + orig_params_count;
				params = safe_emalloc(param_count, sizeof(zval**), 0);

				memcpy(params, extra, extra_count * sizeof(zval**));
				memcpy(params + extra_count, orig_params, orig_params_count * sizeof(zval**));
			/* send_args is true, send return is false, orig_params_count is 0 */
			} else if((*callback)->send_args) {
				param_count = extra_count;
				params = extra;
			/* send_args is false, send_return is false, use the original params is default */
			}

			/* Place params where they go */
			finfo->params = params;
			finfo->param_count = param_count;

			if (SUCCESS == zend_call_function(finfo, &((**callback).callback_cache) TSRMLS_CC)) {
				/* returning true will break the callback chain */
				if(bubble_value && Z_TYPE_P(bubble_value) == IS_BOOL && Z_BVAL_P(bubble_value))
				{
					*stop_default = TRUE;
					finfo->params = orig_params;
					finfo->param_count = orig_params_count;
					if (param_list_count && orig_params_count) {
						efree(params);
					}
					if (bubble_value) {
						zval_ptr_dtor(&bubble_value);
					}
					break;
				}
			} else {
				php_error(E_RECOVERABLE_ERROR, "Error calling %s for msg:%d", finfo->function_name, msg);
			}

			finfo->params = orig_params;
			finfo->param_count = orig_params_count;

			if (param_list_count && orig_params_count) {
				efree(params);
			}
			if (bubble_value) {
				zval_ptr_dtor(&bubble_value);
			}

		}
	}

	/* first, is the stop_default off and do we have an item in the callback map that matches a method in the class - if so final callback time */
	if(*stop_default == FALSE && (zend_hash_index_find(window_object->callback_map, msg, (void **) &callback_method) == SUCCESS) && 
		zend_hash_num_elements(&Z_OBJCE_P(window_object->object_zval)->function_table) > 0 &&
		zend_hash_exists(&Z_OBJCE_P(window_object->object_zval)->function_table, callback_method, strlen(callback_method) + 1)) {
		
		zval *function;
		zval *bubble_value = NULL;

		MAKE_STD_ZVAL(function);
		ZVAL_STRING(function, callback_method, 1);
		
		/* The parameters list is only created/allocated on demand but is used for all callbacks */
		if (!param_list) {
			param_list = safe_emalloc(param_list_count, sizeof(zval**), 0);
			param_list[0] = emalloc(sizeof(zval*));
			ALLOC_INIT_ZVAL(*param_list[0]);
			Z_ISREF_P(*param_list[0]);
			memcpy(param_list + 1, extra, extra_count * sizeof(zval**));
		}

		if (call_user_function_ex(
					&Z_OBJCE_P(window_object->object_zval)->function_table,	// Hash value for the function table
					&window_object->object_zval,							// Pointer to an object (may be NULL)
					function,										// Function name
					&bubble_value,									// Return value																						// Parameter count
					param_list_count,								// Parameter array
					param_list,
					0,                                             // No separation flag (always 0)
					NULL TSRMLS_CC
					)  == FAILURE) {
			php_error(E_RECOVERABLE_ERROR, "Error calling %s for msg:%d", callback_method, msg);
		}

		zval_ptr_dtor(&function);
		/* returning true will break the callback chain */
		if(bubble_value && Z_TYPE_P(bubble_value) == IS_BOOL && Z_BVAL_P(bubble_value)) {
			*stop_default = TRUE;
		}
		if (bubble_value) {
			zval_ptr_dtor(&bubble_value);
		}
	}

	if (param_list) {
		return_value = *param_list[0];
		efree(param_list);
	}

	return return_value;
}
/* }}} */

/* Helper function that asks "is this a statusbar window? if so SIZE dammit */
BOOL CALLBACK wingui_check_for_statusbar(HWND hwndChild, LPARAM lParam) 
{ 
	char buf[20] = {0};

	if (!GetClassName(hwndChild, buf, 20)) {
		return FALSE;
	}

	if (strcmp(buf, STATUSCLASSNAME) == 0) {
		PostMessage(hwndChild, WM_SIZE, (WPARAM) 0, (LPARAM) 0);
	}

	return TRUE;
}

/* generic proc handler for all windows and controls - dispatches messages and uses crackers, packers, and results procs stored */
LRESULT CALLBACK wingui_proc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	zval ***extra = NULL;
	int extra_count = 0;
	zend_bool stop_default = FALSE;
	zval *return_value = NULL;
#ifdef ZTS
	TSRMLS_D;
#endif;

	// TODO: do not do this if this is WM_CREATE, instead get window_object out of the CREATESTRUCT
	wingui_window_object *window_object = (wingui_window_object *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
#ifdef ZTS
	TSRMLS_C = window_object->TSRMLS_C;
#endif

	/* This is ugly as sin.  This says take all the child windows, if they are a statusbar send them a WM_SIZE
	   Why? because the default proc is too stupid to do this */
	if(msg == WM_SIZE) {
		EnumChildWindows(hwnd, wingui_check_for_statusbar, (LPARAM) NULL);
	}

	extra = window_object->messages_cracker(msg, &wParam, &lParam, &extra_count TSRMLS_CC);

	if(window_object) {
		return_value = wingui_winproc_callback_dispatch(window_object, msg, extra, extra_count, &stop_default TSRMLS_CC);
	}
	
	if (stop_default == FALSE) {
		window_object->messages_packer(msg, &wParam, &lParam, extra TSRMLS_CC);
	}

	if (extra) {
		wingui_callback_extra_zvals_dtor(extra_count, extra);
	}

	/* return is based on if we need to call the default proc handling, and if we have an overwritten
	   return value to use instead of the default value */
	if (stop_default && return_value) {
		return window_object->messages_results(msg, return_value TSRMLS_CC);
	} else if (stop_default) {
		return (LRESULT) NULL;
	} else if (return_value) {
		CallWindowProc(window_object->window_proc, hwnd, msg, wParam, lParam);
		return window_object->messages_results(msg, return_value TSRMLS_CC);
	} else {
		return CallWindowProc(window_object->window_proc, hwnd, msg, wParam, lParam);
	}
}
/* }}} */

/* wingui_object_read_property - looks at registered property handlers for reader */
zval *wingui_object_read_property(zval *object, zval *member, int type TSRMLS_DC)
{
	zval *retval;
	wingui_prop_handler *hnd = NULL;
	int worked = FAILURE;
	wingui_generic_object *generic_object = (wingui_generic_object*)zend_object_store_get_object(object TSRMLS_CC);

	MAKE_STD_ZVAL(retval);

	wingui_juggle_type(member, IS_STRING TSRMLS_CC);
	if (generic_object->prop_handler != NULL) {
		worked = zend_hash_find(generic_object->prop_handler, Z_STRVAL_P(member), Z_STRLEN_P(member)+1, (void **) &hnd);
	}

	if (worked == SUCCESS) {
		worked = hnd->read_func(generic_object, member, &retval TSRMLS_CC);
		if (worked == SUCCESS) {
			Z_SET_REFCOUNT_P(retval, 0);
			Z_UNSET_ISREF_P(retval);
		} else {
			retval = EG(uninitialized_zval_ptr);
		}
	} else {
		retval = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
	}

	return retval;
}
/* }}} */

/* wingui_object_get_property_ptr_ptr - Returns null and forces the read to be called each time for special handlers */
zval **wingui_object_get_property_ptr_ptr(zval *object, zval *member TSRMLS_DC)
{
	zval**retval = NULL;
	wingui_prop_handler *hnd;
	int worked = FAILURE;
	wingui_generic_object *generic_object = (wingui_generic_object*)zend_object_store_get_object(object TSRMLS_CC);

	if (FAILURE == wingui_juggle_type(member, IS_STRING TSRMLS_CC)) {
		return retval;
	}

	if (generic_object->prop_handler != NULL) {
		worked = zend_hash_find(generic_object->prop_handler, Z_STRVAL_P(member), Z_STRLEN_P(member)+1, (void **) &hnd);
	}
	if (worked == FAILURE) {
		retval = std_object_handlers.get_property_ptr_ptr(object, member TSRMLS_CC);
	}
	return retval;
}
/* }}} */

/* wingui_object_write_property - reads the property handle table to find the proper writer */
void wingui_object_write_property(zval *object, zval *member, zval *value TSRMLS_DC)
{
	wingui_prop_handler *hnd = NULL;
	int worked = FAILURE;
	wingui_generic_object *generic_object = (wingui_generic_object*)zend_object_store_get_object(object TSRMLS_CC);

	if (FAILURE == wingui_juggle_type(member, IS_STRING TSRMLS_CC)) {
		return;
	}

	if (generic_object->prop_handler != NULL) {
		worked = zend_hash_find(generic_object->prop_handler, Z_STRVAL_P(member), Z_STRLEN_P(member)+1, (void **) &hnd);
	}

	if (worked == SUCCESS) {
		/* readonly properties will have NULL writers */
		if (hnd->write_func == NULL) {
			zend_throw_exception(ce_wingui_exception, "Internal properties cannot be altered", 0 TSRMLS_CC);
		} else {
			hnd->write_func(generic_object, member, value TSRMLS_CC);
		}
	} else {
		std_object_handlers.write_property(object, member, value TSRMLS_CC);
	}
}
/* }}} */

/* wingui_object_delete_property - throws exceptions on buildin properties, they cannot be unset */
void wingui_object_delete_property(zval *object, zval *member TSRMLS_DC)
{
	wingui_prop_handler *hnd;
	int worked = FAILURE;
	wingui_generic_object *generic_object = (wingui_generic_object*)zend_object_store_get_object(object TSRMLS_CC);

	if (FAILURE == wingui_juggle_type(member, IS_STRING TSRMLS_CC)) {
		return;
	}

	if (generic_object->prop_handler != NULL) {
		worked = zend_hash_find(generic_object->prop_handler, Z_STRVAL_P(member), Z_STRLEN_P(member)+1, (void **) &hnd);
	}

	if (worked == SUCCESS) {
		zend_throw_exception(ce_wingui_exception, "Internal properties cannot be unset", 0 TSRMLS_CC);
	} else {
		std_object_handlers.unset_property(object, member TSRMLS_CC);
	}
}
/* }}} */

/* wingui_object_property_exists - checks to see if a property is in the handler table */
int wingui_object_property_exists(zval *object, zval *member, int check_empty TSRMLS_DC)
{
	wingui_prop_handler *hnd = NULL;
	int worked = FAILURE, retval = 0;
	wingui_generic_object *generic_object = (wingui_generic_object*)zend_object_store_get_object(object TSRMLS_CC);

 	if (FAILURE == wingui_juggle_type(member, IS_STRING TSRMLS_CC)) {
		return retval;
	}

	if (generic_object->prop_handler != NULL) {
		worked = zend_hash_find(generic_object->prop_handler, Z_STRVAL_P(member), Z_STRLEN_P(member)+1, (void **) &hnd);
	}

	if (worked == SUCCESS) {
		zval *temp;

		MAKE_STD_ZVAL(temp);
		if (check_empty == 2) {
			retval = 1;
		} else if(hnd->read_func(generic_object, member, &temp TSRMLS_CC) == SUCCESS) {
			if (check_empty == 1) {
				retval = zend_is_true(temp);
			} else {
				retval = (Z_TYPE_P(temp) != IS_NULL);
			}
		}
		zval_ptr_dtor(&temp);
	} else {
		retval = std_object_handlers.has_property(object, member, check_empty TSRMLS_CC);
	}
	return retval;
}
/* }}} */

/* wingui_object_debug_info - Populate a temporary hash table so var_dump works pretty */
HashTable* wingui_object_debug_info(zval *object, int *is_temp TSRMLS_DC)
{
	wingui_prop_handler *hnd;
	HashTable *ht;
	zval *temp;
	ulong index_len, index_num;
	char *index;
	HashPosition pos;
	wingui_generic_object *generic_object = (wingui_generic_object*)zend_object_store_get_object(object TSRMLS_CC);

	*is_temp = 1;

	ALLOC_HASHTABLE(ht);
	zend_hash_init(ht, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(ht, generic_object->std.properties, (copy_ctor_func_t) zval_add_ref, (void *) &temp, sizeof(zval *));

	zend_hash_internal_pointer_reset_ex(generic_object->prop_handler, &pos);
	while (SUCCESS == zend_hash_get_current_data_ex(generic_object->prop_handler, (void**)&hnd, &pos)) {
		zval *value = NULL, *member = NULL; 
		MAKE_STD_ZVAL(member);
		MAKE_STD_ZVAL(value);
		zend_hash_get_current_key_ex(generic_object->prop_handler, &index, &index_len, &index_num, 0, &pos);
		zend_hash_move_forward_ex(generic_object->prop_handler, &pos);
		ZVAL_STRINGL(member, index, index_len, 0);
		hnd->read_func(generic_object, member, &value TSRMLS_CC);
		zend_hash_update(ht, index, index_len, &value, sizeof(zval*), NULL);
	}

	return ht;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(wingui_util)
{
	zend_class_entry ce, version_ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Exception", NULL);
	ce_wingui_exception = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), "Exception" TSRMLS_CC);

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "ArgumentException", NULL);
	ce_wingui_argexception = zend_register_internal_class_ex(&ce, spl_ce_InvalidArgumentException, "InvalidArgumentException" TSRMLS_CC);

	INIT_NS_CLASS_ENTRY(version_ce, PHP_WINGUI_NS, "VersionException", NULL);
	ce_wingui_versionexception = zend_register_internal_class_ex(&version_ce, spl_ce_RuntimeException, "RuntimeException" TSRMLS_CC);

	/* Setup for generic wingui object stuff, makes custom properties easy */
	memcpy(&wingui_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	wingui_object_handlers.unset_property = wingui_object_delete_property;
	wingui_object_handlers.write_property = wingui_object_write_property;
	wingui_object_handlers.read_property = wingui_object_read_property;
	wingui_object_handlers.get_property_ptr_ptr = wingui_object_get_property_ptr_ptr;
	wingui_object_handlers.has_property = wingui_object_property_exists;
	wingui_object_handlers.get_debug_info = wingui_object_debug_info;
	wingui_object_handlers.clone_obj = NULL;

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

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
#include "php_wingui.h"
#include "php_winsystem_api.h"
#include "zend_exceptions.h"

zend_object_handlers wingui_object_handlers;

/* ----------------------------------------------------------------
  wingui_object property handling
------------------------------------------------------------------*/

/* wingui_object_read_property - looks at registered property handlers for reader */
zval *wingui_object_read_property(zval *object, zval *member, int type TSRMLS_DC)
{
	zval *retval;
	wingui_prop_handler *hnd = NULL;
	int worked = FAILURE;
	wingui_generic_object *generic_object = (wingui_generic_object*)zend_object_store_get_object(object TSRMLS_CC);

	MAKE_STD_ZVAL(retval);

	winsystem_juggle_type(member, IS_STRING TSRMLS_CC);
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

	if (FAILURE == winsystem_juggle_type(member, IS_STRING TSRMLS_CC)) {
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

	if (FAILURE == winsystem_juggle_type(member, IS_STRING TSRMLS_CC)) {
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

	if (FAILURE == winsystem_juggle_type(member, IS_STRING TSRMLS_CC)) {
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

 	if (FAILURE == winsystem_juggle_type(member, IS_STRING TSRMLS_CC)) {
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
PHP_MINIT_FUNCTION(wingui_object)
{
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
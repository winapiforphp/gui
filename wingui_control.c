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
zend_class_entry *ce_wingui_control;

/* Custom Object Handler Items */
HashTable wingui_control_prop_handlers;
HashTable wingui_control_callback_map;

/* ----------------------------------------------------------------
  Win\Gui\Control Userland API                                                      
------------------------------------------------------------------*/

/* {{{ proto void Win\Gui\Control->__construct()
     this will gripe if you attempt to extend Control and use it, you must extend
	 one of the control sub-classes instead */
PHP_METHOD(WinGuiControl, __construct)
{
	zend_throw_exception(ce_wingui_exception, ZEND_NS_NAME(PHP_WINGUI_NS, "Control") " cannot be extended, extend a child control instead", 0 TSRMLS_CC);
	return;
}
/* }}} */

/* register Control methods */
static zend_function_entry wingui_control_functions[] = {
	PHP_ME(WinGuiControl, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Control Property Handlers                                                    
------------------------------------------------------------------*/

/* {{{ adds or subtracts different style flags */
int wingui_control_write_style(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	wingui_window_object *control = wingui_window_object_juggle(object);
	int style = GetWindowLongPtr(control->window_handle, GWL_STYLE);

	if(strcmp(Z_STRVAL_P(member), "divider") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				style &= ~CCS_NODIVIDER;
			} else {
				style |= CCS_NODIVIDER;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "resize") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				style &= ~CCS_NORESIZE;
			} else {
				style |= CCS_NORESIZE;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "parentalign") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				style &= ~CCS_NOPARENTALIGN;
			} else {
				style |= CCS_NOPARENTALIGN;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "movex") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				style &= ~CCS_NOMOVEX;
			} else {
				style |= CCS_NOMOVEX;
				style &= ~CCS_NORESIZE;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "movey") == 0) {
		if (wingui_juggle_type(value, IS_BOOL TSRMLS_CC) == SUCCESS) {
			if (Z_BVAL_P(value) == TRUE) {
				style &= ~CCS_NOMOVEY;
			} else {
				style |= CCS_NOMOVEY;
				style &= ~CCS_NORESIZE;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "orientation") == 0) {
		if (wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
			if (Z_LVAL_P(value) == 1) {
				style |= CCS_VERT;
			} else {
				style &= ~CCS_VERT;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "valign") == 0) {
		if (wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
			if (Z_LVAL_P(value) == 1) {
				style &= ~CCS_BOTTOM;
				style |= CCS_TOP;
			} else if (Z_LVAL_P(value) == 2) {
				style &= ~CCS_TOP;
				style |= CCS_BOTTOM;
			} else {
				style &= ~CCS_TOP;
				style &= ~CCS_BOTTOM;
			}
		}
	} else if(strcmp(Z_STRVAL_P(member), "halign") == 0) {
		if (wingui_juggle_type(value, IS_LONG TSRMLS_CC) == SUCCESS) {
			if (Z_LVAL_P(value) == 2) {
				style &= ~CCS_LEFT;
				style |= CCS_RIGHT;
			} else if (Z_LVAL_P(value) == 1) {
				style &= ~CCS_RIGHT;
				style |= CCS_LEFT;
			} else {
				style &= ~CCS_RIGHT;
				style &= ~CCS_LEFT;
			}
		}
	}
			
	SetWindowLongPtr(control->window_handle, GWL_STYLE, style);
	SetWindowPos(control->window_handle, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED);
	return SUCCESS;
}
/* }}} */

/* {{{ Grabs current window style and reads out CCS (control) values */
int wingui_control_read_style(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	wingui_window_object *control = wingui_window_object_juggle(object);
	int style = GetWindowLongPtr(control->window_handle, GWL_STYLE);

	/* more complicated versions */
	if (strcmp(Z_STRVAL_P(member), "halign") == 0) {
		if ((style & CCS_BOTTOM) == CCS_BOTTOM) {
			ZVAL_LONG(*retval, 2);
		} else if ((style & CCS_TOP) == CCS_TOP) {
			ZVAL_LONG(*retval, 1);
		} else {
			ZVAL_LONG(*retval, 0);
		}
	} else if (strcmp(Z_STRVAL_P(member), "valign") == 0) {
		if ((style & CCS_RIGHT) == CCS_RIGHT) {
			ZVAL_LONG(*retval, 2);
		} else if ((style & CCS_LEFT) == CCS_LEFT) {
			ZVAL_LONG(*retval, 1);
		} else {
			ZVAL_LONG(*retval, 0);
		}
	} else if (strcmp(Z_STRVAL_P(member), "orientation") == 0) {
		if ((style & CCS_VERT) == CCS_VERT) {
			ZVAL_LONG(*retval, 1);
		} else {
			ZVAL_LONG(*retval, 0);
		}
	} else if ((strcmp(Z_STRVAL_P(member), "divider") == 0 && (style & CCS_NODIVIDER) == CCS_NODIVIDER) || 
	    (strcmp(Z_STRVAL_P(member), "noresize") == 0 && (style & CCS_NORESIZE) == CCS_NORESIZE) ||
	    (strcmp(Z_STRVAL_P(member), "parentalign") == 0 && (style & CCS_NOPARENTALIGN) == CCS_NOPARENTALIGN) ||
		(strcmp(Z_STRVAL_P(member), "movey") == 0 && (style & CCS_NOMOVEY) == CCS_NOMOVEY) ||
		(strcmp(Z_STRVAL_P(member), "movex") == 0 && (style & CCS_NOMOVEX) == CCS_NOMOVEX)) {
		ZVAL_BOOL(*retval, 0);
	} else {
		ZVAL_BOOL(*retval, 1);
	}
	return SUCCESS;
}
/* }}} */

/* {{{ Grab current properties in class and use it to populate constructor data */
int wingui_control_property_values(zval *object, int *style TSRMLS_DC)
{
	zval *member, *temp;

	MAKE_STD_ZVAL(member);
	
	ZVAL_STRING(member, "valign", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_LONG TSRMLS_CC);
		if (Z_LVAL_P(temp) == 1) {
			*style &= ~CCS_BOTTOM;
			*style |= CCS_TOP;
		} else if (Z_LVAL_P(temp) == 2) {
			*style &= ~CCS_BOTTOM;
			*style |= CCS_TOP;
		} else {
			*style &= ~CCS_TOP;
			*style &= ~CCS_BOTTOM;
		}
	}
	ZVAL_STRING(member, "halign", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_LONG TSRMLS_CC);
		if (Z_LVAL_P(temp) == 2) {
			*style &= ~CCS_LEFT;
			*style |= CCS_RIGHT;
		} else if (Z_LVAL_P(temp) == 1) {
			*style &= ~CCS_RIGHT;
			*style |= CCS_LEFT;
		} else {
			*style &= ~CCS_RIGHT;
			*style &= ~CCS_LEFT;
		}
	}
	ZVAL_STRING(member, "orientation", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_LONG TSRMLS_CC);
		if (Z_LVAL_P(temp) == 1) {
			*style |= CCS_VERT;
		} else {
			*style &= ~CCS_VERT;
		}
	}
	ZVAL_STRING(member, "divider", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp) == TRUE) { 
			*style &= ~CCS_NODIVIDER;
		} else {
			*style |= CCS_NODIVIDER;
		}
	}
	ZVAL_STRING(member, "noresize", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp) == TRUE) {
			*style &= ~CCS_NORESIZE;
		} else {
			*style |= CCS_NORESIZE;
		}
	}
	ZVAL_STRING(member, "parentalign", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp) == TRUE) {
			*style &= ~CCS_NOPARENTALIGN;
		} else {
			*style |= CCS_NOPARENTALIGN;
		}
	}
	ZVAL_STRING(member, "movex", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp) == TRUE) {
			*style &= ~CCS_NOMOVEX;
		} else {
			*style |= CCS_NOMOVEX;
			*style &= ~CCS_NORESIZE;
		}
	}
	ZVAL_STRING(member, "movey", 0);
	if (std_object_handlers.has_property(object, member, 0 TSRMLS_CC)) {
		temp = std_object_handlers.read_property(object, member, 0 TSRMLS_CC);
		wingui_juggle_type(temp, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_P(temp) == TRUE) {
			*style &= ~CCS_NOMOVEY;
		} else {
			*style |= CCS_NOMOVEY;
			*style &= ~CCS_NORESIZE;
		}
	}
	return SUCCESS;
}
/* }}} */

/* {{{ Generic option parser for control options */
int wingui_control_parse_options(zval *options, int *style TSRMLS_DC)
{
	zval **value;

	if (zend_hash_find(Z_ARRVAL_P(options), "valign", sizeof("valign"), (void **) &value) == SUCCESS) {
		wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
		if (Z_LVAL_PP(value) == 1) {
			*style &= ~CCS_BOTTOM;
			*style |= CCS_TOP;
		} else if (Z_LVAL_PP(value) == 2) {
			*style &= ~CCS_TOP;
			*style |= CCS_BOTTOM;
		} else {
			*style &= ~CCS_TOP;
			*style &= ~CCS_BOTTOM;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "halign", sizeof("halign"), (void **) &value) == SUCCESS) {
		wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
		if (Z_LVAL_PP(value) == 2) {
			*style &= ~CCS_LEFT;
			*style |= CCS_RIGHT;
		} else if (Z_LVAL_PP(value) == 1) {
			*style &= ~CCS_RIGHT;
			*style |= CCS_LEFT;
		} else {
			*style &= ~CCS_RIGHT;
			*style &= ~CCS_LEFT;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "orientation", sizeof("orientation"), (void **) &value) == SUCCESS) {
		wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
		if (Z_LVAL_PP(value) == 1) {
			*style |= CCS_VERT;
		} else {
			*style &= ~CCS_VERT;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "divider", sizeof("divider"), (void **) &value) == SUCCESS) {
		wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_PP(value) == TRUE) { 
			*style &= ~CCS_NODIVIDER;
		} else {
			*style |= CCS_NODIVIDER;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "noresize", sizeof("noresize"), (void **) &value) == SUCCESS) {
		wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_PP(value) == TRUE) { 
			*style &= ~CCS_NORESIZE;
		} else {
			*style |= CCS_NORESIZE;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "parentalign", sizeof("parentalign"), (void **) &value) == SUCCESS) {
		wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_PP(value) == TRUE) {
			*style &= ~CCS_NOPARENTALIGN;
		} else {
			*style |= CCS_NOPARENTALIGN;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "movex", sizeof("movex"), (void **) &value) == SUCCESS) {
		wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_PP(value) == TRUE) {
			*style &= ~CCS_NOMOVEX;
		} else {
			*style |= CCS_NOMOVEX;
			*style &= ~CCS_NORESIZE;
		}
	}
	if (zend_hash_find(Z_ARRVAL_P(options), "movey", sizeof("movey"), (void **) &value) == SUCCESS) {
		wingui_juggle_type(*value, IS_BOOL TSRMLS_CC);
		if (Z_BVAL_PP(value) == TRUE) {
			*style &= ~CCS_NOMOVEY;
		} else {
			*style |= CCS_NOMOVEY;
			*style &= ~CCS_NORESIZE;
		}
	}
	return SUCCESS;
}
/* }}} */

/* ----------------------------------------------------------------
  Win\Gui\Control LifeCycle Functions                                                    
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_control)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Control", wingui_control_functions);
	ce_wingui_control = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_control->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

	/* Constants to help with style based property stuff */
	zend_declare_class_constant_long(ce_wingui_control, "TOP", sizeof("TOP") - 1, 1 TSRMLS_CC); 
	zend_declare_class_constant_long(ce_wingui_control, "BOTTOM", sizeof("BOTTOM") - 1, 2 TSRMLS_CC); 
	zend_declare_class_constant_long(ce_wingui_control, "LEFT", sizeof("LEFT") - 1, 1 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_control, "RIGHT", sizeof("RIGHT") - 1, 2 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_control, "HOR", sizeof("HOR") - 1, 0 TSRMLS_CC);
	zend_declare_class_constant_long(ce_wingui_control, "VERT", sizeof("VERT") - 1, 1 TSRMLS_CC);

	zend_class_implements(ce_wingui_control TSRMLS_CC, 3, ce_wingui_messaging, ce_wingui_inputing, ce_wingui_windowing);
	zend_hash_apply_with_arguments(&(ce_wingui_control->function_table) TSRMLS_CC, (apply_func_args_t) unset_abstract_flag, 1, ce_wingui_windowing);
	zend_hash_apply_with_arguments(&(ce_wingui_control->function_table) TSRMLS_CC, (apply_func_args_t) unset_abstract_flag, 1, ce_wingui_messaging);
	zend_hash_apply_with_arguments(&(ce_wingui_control->function_table) TSRMLS_CC, (apply_func_args_t) unset_abstract_flag, 1, ce_wingui_inputing);

	/* property handlers control styles */
	zend_hash_init(&wingui_control_prop_handlers, 0, NULL, NULL, 1);
	/* Note: these MIGHT not make sense in all controls */
	wingui_register_prop_handler(&wingui_control_prop_handlers, ce_wingui_control, "halign", wingui_control_read_style, wingui_control_write_style TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_prop_handlers, ce_wingui_control, "valign", wingui_control_read_style, wingui_control_write_style TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_prop_handlers, ce_wingui_control, "orientation", wingui_control_read_style, wingui_control_write_style TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_prop_handlers, ce_wingui_control, "divider", wingui_control_read_style, wingui_control_write_style TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_prop_handlers, ce_wingui_control, "resize", wingui_control_read_style, wingui_control_write_style TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_prop_handlers, ce_wingui_control, "parentalign", wingui_control_read_style, wingui_control_write_style TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_prop_handlers, ce_wingui_control, "movex", wingui_control_read_style, wingui_control_write_style TSRMLS_CC);
	wingui_register_prop_handler(&wingui_control_prop_handlers, ce_wingui_control, "movey", wingui_control_read_style, wingui_control_write_style TSRMLS_CC);

	/* generic control messages hook up to */
	zend_hash_init(&wingui_control_callback_map, 0, NULL, NULL, 1);
	//zend_hash_merge(&wingui_control_callback_map, &wingui_window_callback_map, NULL, NULL, sizeof(char *), 0);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, CCM_DPISCALE,			&wingui_control_callback_map, onccmdpiscale);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, CCM_GETUNICODEFORMAT,	&wingui_control_callback_map, onccmgetunicodeformat);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, CCM_GETVERSION,			&wingui_control_callback_map, onccmgetversion);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, CCM_SETUNICODEFORMAT,	&wingui_control_callback_map, onccmsetunicodeformat);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, CCM_SETVERSION,			&wingui_control_callback_map, onccmsetversion);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, CCM_SETWINDOWTHEME,		&wingui_control_callback_map, onccmsetwindowtheme);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, WM_NOTIFY,				&wingui_control_callback_map, onwmnotify);
	
	/* Notifications - note this cannot be sent (see send/post callback) */
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_CHAR,				&wingui_control_callback_map, onnmchar);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_CUSTOMDRAW,			&wingui_control_callback_map, onnmcustomdraw);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_CUSTOMTEXT,			&wingui_control_callback_map, onnmcustomtext);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_FONTCHANGED,			&wingui_control_callback_map, onnmfontchanged);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_GETCUSTOMSPLITRECT,	&wingui_control_callback_map, onnmgetcustomsplitrect);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_HOVER,				&wingui_control_callback_map, onnmhover);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_KEYDOWN,				&wingui_control_callback_map, onnmkeydown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_KILLFOCUS,			&wingui_control_callback_map, onnmkillfocus);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_LDOWN,				&wingui_control_callback_map, onnmldown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_NCHITTEST,			&wingui_control_callback_map, onnmnchittest);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_OUTOFMEMORY,			&wingui_control_callback_map, onnmoutofmemory);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_RDOWN,				&wingui_control_callback_map, onnmrddown);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_RELEASEDCAPTURE,		&wingui_control_callback_map, onnmreleasedcapture);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_RETURN,				&wingui_control_callback_map, onnmreturn);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_SETCURSOR,			&wingui_control_callback_map, onnmsetcursor);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_SETFOCUS,			&wingui_control_callback_map, onnmsetfocus);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_THEMECHANGED,		&wingui_control_callback_map, onnmthemechanged);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_TOOLTIPSCREATED,		&wingui_control_callback_map, onnmtooltipscreated);
	REGISTER_WINGUI_MESSAGE_CONSTANT(ce_wingui_control, NM_TVSTATEIMAGECHANGING,&wingui_control_callback_map, onnmtvstateimagechanging);

	return SUCCESS;
}

/* {{{ PHP_MSHUTDOWN_FUNCTION(wingui_control) */
PHP_MSHUTDOWN_FUNCTION(wingui_control)
{
	zend_hash_destroy(&wingui_control_prop_handlers);
	zend_hash_destroy(&wingui_control_callback_map);
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
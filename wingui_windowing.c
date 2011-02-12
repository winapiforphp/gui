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

#include "php_wingui.h"
#include "zend_exceptions.h"
#include "php_winsystem_api.h"
#include "implement_windowing.h"

/* All the classes in this file */
zend_class_entry *ce_wingui_windowing;

/* Notice this is not static, other things gonna use it */
HashTable wingui_windowing_prop_handlers;

/* ----------------------------------------------------------------
  Win\Gui\Windowing Userland API
------------------------------------------------------------------*/

/* {{{ proto array Win\Gui\Windowing->adjustSize(width, height)
       calculates the required size of the window rectangle, based on the desired client-rectangle size
       returns array(width, height) for window size */
PHP_METHOD(WinGuiWindowing, adjustSize)
{
	zend_error_handling error_handling;
	long height = 0, width = 0, style = 0, ex_style = 0;
	RECT rect;
	BOOL menu = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &height, &width) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* Populate our rect */
	rect.top = 0;
	rect.left = 0;
	rect.bottom = height;
	rect.right = width;

	/* grab additional values from current window for styles, extrastyles and menus, needed for proper calculation */ 
	style = GetWindowLongPtr(window_object->window_handle, GWL_STYLE);
	ex_style = GetWindowLongPtr(window_object->window_handle, GWL_EXSTYLE);
	if (GetMenu(window_object->window_handle) != NULL) {
		menu = TRUE;
	}

	/* If call works properly, return array, otherwise throw exception */
	if(AdjustWindowRectEx(&rect, style, menu, ex_style)) {
		array_init(return_value);

		/* add scrollbars afterward because AdjustWindowRectEx doesn't include them */
		if(style & WS_VSCROLL) {
			rect.right += GetSystemMetrics(SM_CXVSCROLL);
		}
		if(style & WS_HSCROLL) {
			rect.bottom += GetSystemMetrics(SM_CYHSCROLL);
		}

		add_next_index_long(return_value, rect.right);
		add_next_index_long(return_value, rect.bottom);
		return;
	}

	winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
}
/* }}} */

/* {{{ proto bool Win\Gui\Windowing->animateShow([int time, int flags, bool activate])
       Identical to Win\Gui\Windowing->show except it animates the display of the window */
PHP_METHOD(WinGuiWindowing, animateShow)
{
	zend_error_handling error_handling;
	long time = 200, flags = 0;
	zend_bool activate = TRUE;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|llb", &time, &flags, &activate) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if(activate == TRUE) {
		flags |= AW_ACTIVATE;
	}

	RETURN_BOOL(AnimateWindow(window_object->window_handle, time, flags));
}
/* }}} */

/* {{{ proto bool Win\Gui\Windowing->animateHide([int time, int flags])
       Identical to  Win\Gui\Windowing->hide except it animates the hiding of the window */
PHP_METHOD(WinGuiWindowing, animateHide)
{
	zend_error_handling error_handling;
	long time = 200, flags = 0;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ll", &time, &flags) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(AnimateWindow(window_object->window_handle, time, flags | AW_HIDE));
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getChildFromPoint()
        */
PHP_METHOD(WinGuiWindowing, getChildFromPoint)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->foreachChild()
        */
PHP_METHOD(WinGuiWindowing, foreachChild)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->findChild()
        */
PHP_METHOD(WinGuiWindowing, findChild)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getAncestor()
        */
PHP_METHOD(WinGuiWindowing, getAncestor)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getClientSize()
        */
PHP_METHOD(WinGuiWindowing, getClientSize)
{
}
/* }}} */

/* {{{ proto object Win\Gui\Windowing->getNext()
       Gets next window in Z order */
PHP_METHOD(WinGuiWindowing, getNext)
{
	HWND handle;
	zend_error_handling error_handling;
	wingui_window_object *next_object;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	handle = GetNextWindow(window_object->window_handle, GW_HWNDNEXT);

	if(handle == NULL) {
		return;
	}

	next_object = (wingui_window_object *)GetWindowLongPtr(handle, GWLP_USERDATA);
	if (next_object == NULL || next_object->object_zval == NULL) {
		/* TODO some day: get the window class get the appropriate CE besed on the class returned */
		zend_throw_exception(ce_wingui_exception, "Win\\Gui\\Windowing::getNext() could not retrieve the requested window - it may have not been created with wingui", 0 TSRMLS_CC);
		return;
	} else {
		zval_dtor(return_value);
		*return_value = *next_object->object_zval;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
	}

}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getPrevious()
        Gets prev window in Z order */
PHP_METHOD(WinGuiWindowing, getPrevious)
{
	HWND handle;
	zend_error_handling error_handling;
	wingui_window_object *prev_object;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	handle = GetNextWindow(window_object->window_handle, GW_HWNDPREV);

	if(handle == NULL) {
		return;
	}

	prev_object = (wingui_window_object *)GetWindowLongPtr(handle, GWLP_USERDATA);
	if (prev_object == NULL || prev_object->object_zval == NULL) {
		/* TODO some day: get the window class get the appropriate CE besed on the class returned */
		zend_throw_exception(ce_wingui_exception, "Win\\Gui\\Windowing::getNext() could not retrieve the requested window - it may have not been created with wingui", 0 TSRMLS_CC);
		return;
	} else {
		zval_dtor(return_value);
		*return_value = *prev_object->object_zval;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
	}
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getParent()
        */
PHP_METHOD(WinGuiWindowing, getParent)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getTopChild()
        */
PHP_METHOD(WinGuiWindowing, getTopChild)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getOwner()
        */
PHP_METHOD(WinGuiWindowing, getOwner)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getFirst()
        */
PHP_METHOD(WinGuiWindowing, getFirst)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getLast()
        */
PHP_METHOD(WinGuiWindowing, getLast)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getChild()
        */
PHP_METHOD(WinGuiWindowing, getChild)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getSize()
        */
PHP_METHOD(WinGuiWindowing, getSize)
{
}
/* }}} */

/* {{{ proto bool Win\Gui\Windowing->getText()
       gets the text set for the window */
PHP_METHOD(WinGuiWindowing, getText)
{
	zend_error_handling error_handling;
	char * window_text;
	int str_len, worked_len;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* First we need the length of the string for our buffer */
	str_len = GetWindowTextLength(window_object->window_handle);
	
	/* Then we allocate a buffer and copy our text into it */
	window_text = safe_emalloc(str_len, sizeof(char), 1);
	worked_len = GetWindowText(window_object->window_handle, window_text, str_len + 1);
	if (worked_len == 0) {
		winsystem_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}

	RETURN_STRINGL(window_text, worked_len, 1);
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getProcessId()
        */
PHP_METHOD(WinGuiWindowing, getProcessId)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getPlacement()
        */
PHP_METHOD(WinGuiWindowing, getPlacement)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getTextDirect()
        */
PHP_METHOD(WinGuiWindowing, getTextDirect)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->isChild()
        */
PHP_METHOD(WinGuiWindowing, isChild)
{
}
/* }}} */

/* {{{ proto bool Win\Gui\Windowing->isVisible()
       True if the object is visible, or false.  This only shows the visible flag, not if item is clipped  */
PHP_METHOD(WinGuiWindowing, isVisible)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(IsWindowVisible(window_object->window_handle));
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->logicalToPhysical()
        */
PHP_METHOD(WinGuiWindowing, logicalToPhysical)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->move()
        */
PHP_METHOD(WinGuiWindowing, move)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->physicalToLogical()
        */
PHP_METHOD(WinGuiWindowing, physicalToLogical)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->getVisibleChild()
        */
PHP_METHOD(WinGuiWindowing, getVisibleChild)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->setParent()
        */
PHP_METHOD(WinGuiWindowing, setParent)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->setPlacement()
        */
PHP_METHOD(WinGuiWindowing, setPlacement)
{
}
/* }}} */

/* {{{ proto void Win\Gui\Windowing->setPos()
        */
PHP_METHOD(WinGuiWindowing, setPos)
{
}
/* }}} */

/* {{{ proto bool Win\Gui\Windowing->setText()
       sets the caption on the window */
PHP_METHOD(WinGuiWindowing, setText)
{
	zend_error_handling error_handling;
	char * window_text;
	int str_len;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &window_text, &str_len) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SetWindowText(window_object->window_handle, window_text));
}
/* }}} */

/* {{{ proto bool Win\Gui\Windowing->hide()
       Makes an object invisible
       returns true if the object was previously visible
       returns false if the object was previously hidden */
PHP_METHOD(WinGuiWindowing, hide)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(ShowWindow(window_object->window_handle, SW_HIDE));
}
/* }}} */

/* {{{ proto bool Win\Gui\Windowing->show([bool noactivate, bool async])
       displays an object in its current size and position.
       if noactivate is true, then the object is not activated
       returns true if the object was previously visible
       returns false if the object was previously hidden */
PHP_METHOD(WinGuiWindowing, show)
{
	zend_error_handling error_handling;
	zend_bool noactivate = FALSE, async = FALSE;
	int flag = SW_SHOW;
	wingui_window_object *window_object = (wingui_window_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_argexception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|bb", &noactivate, &async) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (noactivate == TRUE) {
		flag = SW_SHOWNA;
	}
	if (async == TRUE) {
		RETURN_BOOL(ShowWindowAsync(window_object->window_handle, flag));
	} else {
		RETURN_BOOL(ShowWindow(window_object->window_handle, flag));
	}
}
/* }}} */





/* register Windowing methods */
static zend_function_entry wingui_windowing_functions[] = {
	PHP_ABSTRACT_ME(WinGuiWindowing, adjustSize, WinGuiWindowing_adjustSize_args)
	PHP_ABSTRACT_ME(WinGuiWindowing, animateShow, WinGuiWindowing_animateShow_args)
	PHP_ABSTRACT_ME(WinGuiWindowing, animateHide, WinGuiWindowing_animateHide_args)
	PHP_ME(WinGuiWindowing, getChildFromPoint, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, foreachChild, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, findChild, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getAncestor, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getClientSize, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ABSTRACT_ME(WinGuiWindowing, getNext, WinGuiWindowing_getNext_args)
	PHP_ABSTRACT_ME(WinGuiWindowing, getPrevious, WinGuiWindowing_getPrevious_args)
	PHP_ME(WinGuiWindowing, getParent, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getTopChild, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getOwner, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getFirst, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getLast, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getChild, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getSize, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getText, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getProcessId, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getPlacement, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getTextDirect, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, isChild, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, isVisible, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, logicalToPhysical, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, move, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, physicalToLogical, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, getVisibleChild, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, setParent, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, setPlacement, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, setPos, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ME(WinGuiWindowing, setText, NULL, ZEND_ACC_PUBLIC| ZEND_ACC_ABSTRACT)
	PHP_ABSTRACT_ME(WinGuiWindowing, show, WinGuiWindowing_show_args)
	PHP_ABSTRACT_ME(WinGuiWindowing, hide, WinGuiWindowing_hide_args)
	{NULL, NULL, NULL}
};

/* ----------------------------------------------------------------
  Win\Gui\Window Property Handlers
------------------------------------------------------------------*/

/* ----------------------------------------------------------------
  Win\Gui\Windowing LifeCycle Functions
------------------------------------------------------------------*/
PHP_MINIT_FUNCTION(wingui_windowing)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_WINGUI_NS, "Windowing", wingui_windowing_functions);
	ce_wingui_windowing = zend_register_internal_class(&ce TSRMLS_CC);
	ce_wingui_windowing->ce_flags |= ZEND_ACC_INTERFACE;

	/* Property map for all items that implement windowing */
	zend_hash_init(&wingui_windowing_prop_handlers, 0, NULL, NULL, 1);
	//wingui_register_prop_handler(&wingui_windowing_prop_handlers, ce_wingui_window, "x", wingui_window_read_pos, wingui_window_write_size TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_windowing_prop_handlers, ce_wingui_window, "y", wingui_window_read_pos, wingui_window_write_size TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_windowing_prop_handlers, ce_wingui_window, "width", wingui_window_read_size, wingui_window_write_size TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_windowing_prop_handlers, ce_wingui_window, "height", wingui_window_read_size, wingui_window_write_size TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_windowing_prop_handlers, ce_wingui_window, "text", wingui_window_read_text, wingui_window_write_text TSRMLS_CC);
	//wingui_register_prop_handler(&wingui_windowing_prop_handlers, ce_wingui_window, "parent", wingui_window_read_parent, wingui_window_write_parent TSRMLS_CC);

	return SUCCESS;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
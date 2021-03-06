ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_arrangeMinimized_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, parent)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_cascade_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, parent)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, rect)
	ZEND_ARG_INFO(0, kids)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindow_addFilterMessage_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, message)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(WinGuiWindow_removeFilterMessage_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_INFO(0, message)
ZEND_END_ARG_INFO()













ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_setText_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_maximize_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, noactivate)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_restore_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, noactivate)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_show_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, noactivate)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(WinGuiWindow_showMinimized_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
	ZEND_ARG_INFO(0, noactivate)
ZEND_END_ARG_INFO()





ZEND_BEGIN_ARG_INFO(WinGuiWindow_attachMenu_args, ZEND_SEND_BY_VAL)
	ZEND_ARG_OBJ_INFO(0, menu, Win\\Gui\\Menu, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Win\Gui\Window::__construct([type, text, parent, ][array options])
     Create a new Window - types are TOPLEVEL (default), POPUP or CHILD
     x and y are in client coordinates not screen coordinates,
     height and width are client area, not window area */
PHP_METHOD(WinGuiWindow, __construct)
{
	zend_error_handling error_handling;
	/* These are the "usual suspects" for window options */
	char *text = "";
	long type = 1;
	zval *parent = NULL, *options = NULL;
	HMENU menu = NULL;
	BOOL has_menu = FALSE;
	wingui_window_object *window_object;
	wingui_menu_object *menu_object;
	zval **zval_menu = NULL;
	zval *member, *temp;

	/* Defaults for window values */
	long x = CW_USEDEFAULT, y = CW_USEDEFAULT, width = CW_USEDEFAULT, height = CW_USEDEFAULT, style = WS_OVERLAPPEDWINDOW, extrastyle = 0;

	/* Populate data from properties - no parent allowed */
	if(wingui_window_object_property_values(getThis(), &x, &y, &height, &width, &text, &style, &extrastyle TSRMLS_CC) == FAILURE) {
		return;
	}
	/* Window specific properties */
	MAKE_STD_ZVAL(member);
	ZVAL_STRING(member, "type", 0);
	if (std_object_handlers.has_property(getThis(), member, 0 TSRMLS_CC)) {
		temp = wingui_juggle_type(std_object_handlers.read_property(getThis(), member, 0 TSRMLS_CC), IS_LONG TSRMLS_CC);
		if (Z_LVAL_P(temp) == 3) {
			style |= WS_CHILD;
		}
		else if (Z_LVAL_P(temp) == 2) {
			style |= WS_POPUP;
		}
		else if (Z_LVAL_P(temp) != 1) {
			zend_throw_exception(ce_wingui_exception, "Valid window types are Window::CHILD, Window::POPUP, and Window::TOPLEVEL", 0 TSRMLS_CC);
			return;
		}
	}

	/* we want a nice exception here not a warning */
	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a", &options) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if(options && Z_TYPE_P(options) == IS_ARRAY) {

		/* Basic window options */
		if(wingui_window_object_parse_options(options, &x, &y, &height, &width, &text, &style, &extrastyle TSRMLS_CC) == FAILURE) {
			return;
		}

		/* Only windows need to worry about menus */
		
	}

	/* handle setting type */
	if (type == 3) {
		style &= ~WS_POPUP;
		style |= WS_CHILD;
	} else if (type == 2) {
		style &= ~WS_CHILD;
		style |= WS_POPUP;
	} else if (type == 1) {
		style &= ~WS_POPUP;
		style &= ~WS_CHILD;
	} else {
		zend_throw_exception(ce_wingui_exception, "Valid window types are Window::CHILD, Window::POPUP, and Window::TOPLEVEL", 0 TSRMLS_CC);
		return;
	}

	/* a window with a parent must have either popup or child set */ 
	if (parent && !(style & WS_POPUP)) {
		zend_throw_exception(ce_wingui_exception, "Valid window types if you are sending a parent are Window::CHILD or Window::POPUP", 0 TSRMLS_CC);
		return;
	}

	/* WS_MAXIMIZEBOX or WS_MINIMIZEBOX cannot be seen with WS_EX_CONTEXTHELP
	if ((style & WS_MAXIMIZEBOX || style & WS_MINIMIZEBOX) && (extrastyle & WS_EX_CONTEXTHELP)) {
		zend_throw_exception(ce_wingui_exception, "You cannot use helpbox with maximizebox or minimizebox", 0 TSRMLS_CC);
		return;
	} */




	/* WS_CAPTION is magically added to WS_TILED (not WS_CHILD OR WS_POPUP) windows without other styles 
	if (!(style & WS_POPUP) && !(style & WS_CHILD)) {
		style |= WS_CAPTION;
	}

	/* If these */
	if(style == 0 && extrastyle == 0) {
		style &= WS_OVERLAPPEDWINDOW;
		//extrastyle = WS_EX_WINDOWEDGE;
	}

	/* adjust client width/height to window width/height */
	if (width != CW_USEDEFAULT || height != CW_USEDEFAULT) {
		if (wingui_client_to_window_size(&x, &y, &width, &height, style, extrastyle, has_menu) == FAILURE) {
			wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
			return;
		}
	}

	window_object = wingui_window_object_constructor("php_wingui", text, x, y, width, height, style, extrastyle, parent, menu, getThis(), wingui_window_object_proc_handler TSRMLS_CC);
	if (menu) {
		menu_object->window_handle = window_object->window_handle;
		window_object->menu = *zval_menu;
		Z_ADDREF_P(window_object->menu);
	}
}
/* }}} */

/* {{{ proto int Win\Gui\Window::arrangeMinimized([Win\Gui\Window parent])
       Arranges the minimized child windows of the window given, or of the desktop if no window is passed */
PHP_METHOD(WinGuiWindow, arrangeMinimized)
{
	zend_error_handling error_handling;
	zval *parent_zval = NULL;
	HWND parent;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|O!", &parent_zval, ce_wingui_window) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* NULL means desktop */
	if(parent_zval) {
		wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(parent_zval TSRMLS_CC);
		parent = window_object->window_handle;
	} else {
		parent =  GetDesktopWindow();
	}

	/* make the call */
	RETURN_LONG(ArrangeIconicWindows(parent));
}
/* }}} */

/* {{{ proto int Win\Gui\Window::cascade([Win\Gui\Window parent, int flags, array rect, array kids])
       Cascades the specified children of the specified parent */
PHP_METHOD(WinGuiWindow, cascade)
{
	zend_error_handling error_handling;
	zval *parent_zval = NULL, *rect_zval = NULL, *kids_zval = NULL, **ppzval;
	zval **value;
    long tile_flags = 0, error = 0;
	RECT *rect;
	UINT num_kids = 0;
	HWND parent, *kids;
	WORD total;
	HashTable *kids_hash = NULL;

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|O!la!a", &parent_zval, ce_wingui_window, &tile_flags, &rect_zval, &kids_zval) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	/* NULL means desktop */
	if(parent_zval) {
		wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(parent_zval TSRMLS_CC);
		parent = window_object->window_handle;
	} else {
		parent = NULL;
	}

	/* RECT is NULL if nothing is passed */
	if(rect_zval) {
		rect = ecalloc(1, sizeof(RECT));
		rect->bottom = 0;
		rect->top = 0;
		rect->left = 0;
		rect->right = 0;
		if (zend_hash_find(Z_ARRVAL_P(rect_zval), "left", sizeof("left"), (void **) &value) == SUCCESS) {
			*value = wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
			rect->left = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(rect_zval), "right", sizeof("right"), (void **) &value) == SUCCESS) {
			*value = wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
			rect->right = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(rect_zval), "top", sizeof("top"), (void **) &value) == SUCCESS) {
			*value = wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
			rect->top = Z_LVAL_PP(value);
		}
		if (zend_hash_find(Z_ARRVAL_P(rect_zval), "bottom", sizeof("bottom"), (void **) &value) == SUCCESS) {
			*value = wingui_juggle_type(*value, IS_LONG TSRMLS_CC);
			rect->bottom = Z_LVAL_PP(value);
		}
	} else {
		rect = (RECT *) NULL;
	}

	/* walk the kids array and look for legal containers */
	if(kids_zval) {
		/* Grab the zend hash */
		kids_hash = Z_ARRVAL_P(kids_zval);
		/* emalloc our space as number of elements in the hash - that MIGHT be too big if we hit bad values */
		kids = emalloc(zend_hash_num_elements(kids_hash));

		/* iterate the array, each value inside MUST be an instance of Win\Gui\Container */
		for(zend_hash_internal_pointer_reset(kids_hash); zend_hash_has_more_elements(kids_hash) == SUCCESS; zend_hash_move_forward(kids_hash)) { 
			if (zend_hash_get_current_data(kids_hash, (void **)&ppzval) == FAILURE) {
				continue; 
			}

			/* Check for instanceof */
			if (Z_TYPE_PP(ppzval) == IS_OBJECT && instanceof_function(Z_OBJCE_PP(ppzval), ce_wingui_window TSRMLS_CC)) {
				/* we have success, grab the internal window object and stick it in the array */
				wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(parent_zval TSRMLS_CC);
				kids[++num_kids] = window_object->window_handle;
			}
		}

	} else {
		kids = NULL;
	}

	/* make the call */
	total = CascadeWindows(parent, tile_flags, rect, num_kids, kids);
	
	if (rect_zval) {
		efree(rect);
	}
	if (kids_zval) {
		efree(kids);
	}
	error = GetLastError();

	if (total < 1 && error > 0) {
		wingui_create_error(error, ce_wingui_exception TSRMLS_CC);
	} else {
		RETURN_LONG(total);
	}
}
/* }}} */

/* {{{ proto array Win\Gui\Window::addFilterMessage(int message)
       The filter, somewhat contrary to intuition, is a list of messages that are allowed through */
PHP_METHOD(WinGuiWindow, addFilterMessage)
{
	zend_error_handling error_handling;
	HMODULE module;
	long message;

	typedef BOOL (WINAPI *ChangeWindowMessageFilterFunc)(UINT message, DWORD dwFlag);

	ChangeWindowMessageFilterFunc ChangeWindowMessageFilterFuncHandle;

	/* Short Circuit if we're NOT on vista or higher */
	if(!wingui_is_vista(TSRMLS_C)) {
		zend_throw_exception(ce_wingui_versionexception, "Win\\Gui\\Window::addFilterMessage() does not work on Windows versions before Vista", 0 TSRMLS_CC);
		return;
	}

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &message) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	module = LoadLibrary("user32.dll");

	if (module) {
		ChangeWindowMessageFilterFuncHandle  = (ChangeWindowMessageFilterFunc)GetProcAddress(module, "ChangeWindowMessageFilter");
		if (ChangeWindowMessageFilterFuncHandle == NULL) {
			wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
			return;
		}
	} else {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}

	RETVAL_BOOL(ChangeWindowMessageFilterFuncHandle(message, MSGFLT_ADD));
	FreeLibrary(module);
}
/* }}} */

/* {{{ proto array Win\Gui\Window::removeFilterMessage(int message)
       The filter, somewhat contrary to intuition, is a list of messages that are allowed through
	   removing a message from the list blocks it */
PHP_METHOD(WinGuiWindow, removeFilterMessage)
{
	zend_error_handling error_handling;
	HMODULE module;
	long message;

	typedef BOOL (WINAPI *ChangeWindowMessageFilterFunc)(UINT message, DWORD dwFlag);

	ChangeWindowMessageFilterFunc ChangeWindowMessageFilterFuncHandle;

	/* Short Circuit if we're NOT on vista or higher */
	if(!wingui_is_vista(TSRMLS_C)) {
		zend_throw_exception(ce_wingui_versionexception, "Win\\Gui\\Window::addFilterMessage() does not work on Windows versions before Vista", 0 TSRMLS_CC);
		return;
	}

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &message) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	module = LoadLibrary("user32.dll");

	if (module) {
		ChangeWindowMessageFilterFuncHandle  = (ChangeWindowMessageFilterFunc)GetProcAddress(module, "ChangeWindowMessageFilter");
		if (ChangeWindowMessageFilterFuncHandle == NULL) {
			wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
			return;
		}
	} else {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}

	RETVAL_BOOL(ChangeWindowMessageFilterFuncHandle(message, MSGFLT_REMOVE));
	FreeLibrary(module);
}
/* }}} */

/* {{{ proto bool Win\Gui\Window->isMinimized()
       True if the window is minimized, or false. */
PHP_METHOD(WinGuiWindow, isMinimized)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(IsIconic(window_object->window_handle));
}
/* }}} */

/* {{{ proto bool Win\Gui\Container->isVisible()
       True if the container is visible, or false.  This only shows the visible flag, not if item is clipped  */
PHP_METHOD(WinGuiWindow, isVisible)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(IsWindowVisible(window_object->window_handle));
}
/* }}} */

/* {{{ proto bool Win\Gui\Container->showOwnedPopups()
       shows all pop-up windows owned by the specified container
	   shows only windows hidden by a previous call to hideOwnedPopups.
	   For example, if a pop-up window is hidden by using hide,
	   subsequently calling showOwnedPopups does not cause the window to be shown. 
*/
PHP_METHOD(WinGuiWindow, showOwnedPopups)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(ShowOwnedPopups(window_object->window_handle, TRUE));
}
/* }}} */

/* {{{ proto bool Win\Gui\Container->hideOwnedPopups()
       hides all pop-up windows owned by the specified container
*/
PHP_METHOD(WinGuiWindow, hideOwnedPopups)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(ShowOwnedPopups(window_object->window_handle, FALSE));
}
/* }}} */



/* {{{ proto bool Win\Gui\Window->setText()
       sets the caption on the window */
PHP_METHOD(WinGuiWindow, setText)
{
	zend_error_handling error_handling;
	char * window_text;
	int str_len;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &window_text, &str_len) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(SetWindowText(window_object->window_handle, window_text));
}
/* }}} */

/* {{{ proto bool Win\Gui\Window->getText()
       gets the text set for the window */
PHP_METHOD(WinGuiWindow, getText)
{
	zend_error_handling error_handling;
	char * window_text;
	int str_len, worked_len;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
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
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
		return;
	}

	RETURN_STRINGL(window_text, worked_len, 1);
}
/* }}} */









/* ----------------------------------------------------------------
  Win\Gui\Window Menu Related API                                               
------------------------------------------------------------------*/

/* {{{ proto bool Win\Gui\Window->attachMenu(Win\Gui\Menu menu)
       attaches a menu to a window */
PHP_METHOD(WinGuiWindow, attachMenu)
{
	zend_error_handling error_handling;
	zval *menu;
	wingui_menu_object *menu_object;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O",  &menu, ce_wingui_menu) == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	menu_object = (wingui_menu_object*)wingui_menu_object_get(menu TSRMLS_CC);

	if (window_object->menu) {
		Z_DELREF_P(window_object->menu);
		window_object->menu = NULL;
	}

	if(SetMenu(window_object->window_handle, menu_object->menu_handle)) {
		menu_object->window_handle = window_object->window_handle;
		DrawMenuBar(window_object->window_handle);
		window_object->menu = menu;
		Z_ADDREF_P(window_object->menu);
		RETURN_TRUE;
	} else {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
	}
}
/* }}} */

/* {{{ proto bool Win\Gui\Window->detachMenu(void)
       removes the window menu */
PHP_METHOD(WinGuiWindow, detachMenu)
{
	zend_error_handling error_handling;
	wingui_window_object *window_object = (wingui_window_object*)wingui_window_object_get(getThis() TSRMLS_CC);

	zend_replace_error_handling(EH_THROW, ce_wingui_exception, &error_handling TSRMLS_CC);
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (window_object->menu) {
		Z_DELREF_P(window_object->menu);
		window_object->menu = NULL;
	}

	if(SetMenu(window_object->window_handle, NULL)) {
		DrawMenuBar(window_object->window_handle);
		RETURN_TRUE;
	} else {
		wingui_create_error(GetLastError(), ce_wingui_exception TSRMLS_CC);
	}
}
/* }}} */

/* register Message methods */
static zend_function_entry wingui_window_functions[] = {
	PHP_ME(WinGuiWindow, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(WinGuiWindow, allowSetForeground, WinGuiWindow_allowSetForeground_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, animateShow, WinGuiWindow_animateHide_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, animateHide, WinGuiWindow_animateShow_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, arrangeMinimized, WinGuiWindow_arrangeMinimized_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, bringToTop, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, popupPosition, WinGuiWindow_popupPosition_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, cascade, WinGuiWindow_cascade_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, addFilterMessage, WinGuiWindow_addFilterMessage_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WinGuiWindow, removeFilterMessage, WinGuiWindow_removeFilterMessage_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)




	PHP_ME(WinGuiWindow, isMinimized, NULL, ZEND_ACC_PUBLIC)

	PHP_ME(WinGuiWindow, isVisible, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, showOwnedPopups, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, hideOwnedPopups, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, hide, NULL, ZEND_ACC_PUBLIC)
	
	
	
	PHP_ME(WinGuiWindow, tile, WinGuiWindow_tile_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	
	
	PHP_ME(WinGuiWindow, setText, WinGuiWindow_setText_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, getText, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, minimize, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, maximize, WinGuiWindow_maximize_args, ZEND_ACC_PUBLIC)
	
	PHP_ME(WinGuiWindow, restore, WinGuiWindow_restore_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, show, WinGuiWindow_show_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, showMinimized, WinGuiWindow_showMinimized_args, ZEND_ACC_PUBLIC)
	
	/* Menu related API */
	PHP_ME(WinGuiWindow, attachMenu, WinGuiWindow_attachMenu_args, ZEND_ACC_PUBLIC)
	PHP_ME(WinGuiWindow, detachMenu, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* Uses getclientrect to retrieve width, height of a window */
int wingui_window_read_size(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	BOOL worked;
	RECT client;
	wingui_window_object *window_object = wingui_window_object_juggle(object);

	worked = GetClientRect(window_object->window_handle, &client);
	if (worked == 0) {
		return FAILURE;
	}

	if(strcmp(Z_STRVAL_P(member), "width") == 0) {
		ZVAL_LONG(*retval, client.right);
	} else if(strcmp(Z_STRVAL_P(member), "height") == 0) {
		ZVAL_LONG(*retval, client.bottom);
	}

	return SUCCESS;
}
/* }}} */

/* Uses getwindowrect to retrieve x, y of a window */
int wingui_window_read_pos(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	BOOL worked;
	int parentstyle;
	RECT parent, client;
	wingui_window_object *window_object = wingui_window_object_juggle(object);

	/* this will give us x and y properly */
	worked = GetWindowRect(window_object->window_handle, &client);
	if (worked == 0) {
		return FAILURE;
	}
	/* we need to check if this is a child window - WS_CHILD - if so things get nasty */
	if (GetWindowLongPtr(window_object->window_handle, GWL_STYLE) & WS_CHILD)
	{
		/* figure out our parent rect */ 
		worked = GetWindowRect(GetParent(window_object->window_handle), &parent);
		if (worked == 0) {
			return FAILURE;
		}

		/* subtract parent left and parent top to remove parent position */
		client.left = client.left - parent.left;
		client.top = client.top - parent.top;

		/* grab parent style */
		parentstyle = GetWindowLongPtr(GetParent(window_object->window_handle), GWL_STYLE);
		if (parentstyle & WS_CAPTION) {
			client.top -= GetSystemMetrics(SM_CYCAPTION);
		}
		if (parentstyle & WS_THICKFRAME) {
			client.left -= GetSystemMetrics(SM_CXSIZEFRAME);
			client.top -= GetSystemMetrics(SM_CYSIZEFRAME);
		}
		else if (parentstyle & WS_CAPTION && parentstyle & WS_BORDER) {
			client.left -= GetSystemMetrics(SM_CYFIXEDFRAME);
			client.top -= GetSystemMetrics(SM_CXFIXEDFRAME);
		}
		else if (parentstyle & WS_BORDER) {
			client.left -= GetSystemMetrics(SM_CXBORDER);
			client.top -= GetSystemMetrics(SM_CYBORDER);
		}
		// TODO: probably need the 3-d edge stuff in here, how to detect?
	}
	if(strcmp(Z_STRVAL_P(member), "x") == 0) {
		ZVAL_LONG(*retval, client.left);
	}
	else if(strcmp(Z_STRVAL_P(member), "y") == 0) {
		ZVAL_LONG(*retval, client.top);
	}

	return SUCCESS;
}
/* }}} */

/* Uses movewindow to alter position and/or size after retrieving current size and position */
int wingui_window_write_size(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	BOOL worked;
	RECT client;
	long x, y, width, height, style, extrastyle;
	wingui_window_object *window_object = wingui_window_object_juggle(object);

	value = wingui_juggle_type(value, IS_LONG TSRMLS_CC);
	
	/* this will give us x and y properly */
	worked = GetWindowRect(window_object->window_handle, &client);
	if (worked == 0) {
		return FAILURE;
	}
	y = client.top;
	x = client.left;

	/* this will give us the correct height and width */
	worked = GetClientRect(window_object->window_handle, &client);
	if (worked == 0) {
		return FAILURE;
	}
	height = client.bottom;
	width = client.right;
	
	if(strcmp(Z_STRVAL_P(member), "x") == 0) {
		x = Z_LVAL_P(value);
	} else if(strcmp(Z_STRVAL_P(member), "y") == 0) {
		y = Z_LVAL_P(value);
	} else if(strcmp(Z_STRVAL_P(member), "width") == 0) {
		width = Z_LVAL_P(value);
	} else if(strcmp(Z_STRVAL_P(member), "height") == 0) {
		height = Z_LVAL_P(value);
	}

	/* retrieve styles - they effect size */
	style = GetWindowLongPtr(window_object->window_handle, GWL_STYLE);
	extrastyle = GetWindowLongPtr(window_object->window_handle, GWL_EXSTYLE);

	/* adjustclientrectex does not take scrollbars into account */
	if(style & WS_VSCROLL) {
		width = width + GetSystemMetrics(SM_CXVSCROLL);
	}
	if(style & WS_HSCROLL) {
		height = height + GetSystemMetrics(SM_CYHSCROLL);
	}

	/* now we have to calculate real size including decorations from the requested client size */
	client.top = y;
	client.left = x;
	client.bottom = y + height;
	client.right = x + width;
	// TODO: adjust for menu!
	worked = AdjustWindowRectEx(&client, style, 0, extrastyle);
	if (worked == 0) {
		return FAILURE;
	}

	/* actually move the window */
	worked =  MoveWindow(window_object->window_handle, x, y, client.right - client.left, client.bottom - client.top, TRUE);
	if (worked == 0) {
		return FAILURE;
	}

	return SUCCESS;
}
/* }}} */

/* Uses setwindowtext to change the title/text of a window */
int wingui_window_write_text(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	BOOL worked;
	wingui_window_object *window_object = wingui_window_object_juggle(object);

	/* cast the value to a string */
	value = wingui_juggle_type(value, IS_STRING TSRMLS_CC);

	worked = SetWindowText(window_object->window_handle, Z_STRVAL_P(value));
	if (worked == 0)
	{
		return FAILURE;
	}
	return SUCCESS;
}

/* read in the text/title with getwindowtext */
int wingui_window_read_text(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	BOOL worked;
	int len;
	LPTSTR text;
	wingui_window_object *window_object = wingui_window_object_juggle(object);

	/* First we need the length of the string for our buffer */
	len = GetWindowTextLength(window_object->window_handle);
	
	/* Then we allocate a buffer and copy our text into it */
	text = safe_emalloc(len, sizeof(char), 1);
	worked = GetWindowText(window_object->window_handle, text, len + 1);
	if (worked == 0) {
		return FAILURE;
	}
	ZVAL_STRING(*retval, text, 0);

	return SUCCESS;
}

/* Uses setparent to change the parent of a window - notice this will NOT change the owner */
int wingui_window_write_parent(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	HWND parent;
	wingui_window_object *window_object = wingui_window_object_juggle(object);

	/* grab previous parent, if there was one */
	parent = GetParent(window_object->window_handle);
	if (parent && IsWindow(parent))
	{
		/* Grab the stored window object zval and knock off a ref */
		wingui_window_object *parent_object = (wingui_window_object*)GetWindowLongPtr(parent, GWLP_USERDATA);
		if (parent_object->object)
		{
			Z_DELREF_P(parent_object->object);
		}
	}
	/* a null value means we're "unparenting" it */
	if (Z_TYPE_P(value) == IS_NULL)
	{
		SetParent(window_object->window_handle, NULL);
		SetWindowLongPtr(window_object->window_handle, GWL_STYLE, GetWindowLongPtr(window_object->window_handle, GWL_STYLE) & ~WS_CHILD);
	}
	/* we don't bother casting this - either its an object with the right ancestor or we blow up */
	else if (Z_TYPE_P(value) == IS_OBJECT && instanceof_function(Z_OBJCE_P(value), ce_wingui_window TSRMLS_CC))
	{
		int style;
		HWND worked;
		wingui_window_object *parent_obj = (wingui_window_object *)zend_object_store_get_object(value TSRMLS_CC);

		worked = SetParent(window_object->window_handle, parent_obj->window_handle);
		Z_ADDREF_P(value);
		/* if WS_POPUP isn't set we must add WS_CHILD */
		style = GetWindowLongPtr(window_object->window_handle, GWL_STYLE);
		if (!(style & WS_POPUP))
		{
			SetWindowLongPtr(window_object->window_handle, GWL_STYLE, style | WS_CHILD);
		}
	}
	else
	{
		zend_throw_exception(ce_wingui_exception, "Parent must be null or an instance of Win::Ui::Window", 0 TSRMLS_CC);
		return FAILURE;
	}
	return SUCCESS;
}

/* grab the zval for a parent if one exists */
int wingui_window_read_parent(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	HWND parent = NULL;
	wingui_window_object *window = wingui_window_object_juggle(object);

	ZVAL_NULL(*retval);
	/* Grab the window handle */
	parent = GetParent(window->window_handle);
	if (parent) {
		/* Grab the stored window object */
		wingui_window_object *window_object = (wingui_window_object*)GetWindowLongPtr(parent, GWLP_USERDATA);
		/* The parent had better be a valid window or all this is moot */
		//REPLACE_ZVAL_VALUE(retval, window_object->object, 1);
	}
	return SUCCESS;
}

/* grab the zval for a menu if one exists */
int wingui_window_read_menu(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	wingui_window_object *window = wingui_window_object_juggle(object);

	ZVAL_NULL(*retval);
	/* Do we have a menu? */
	if (window->menu) {
		/* replace the zval */
		REPLACE_ZVAL_VALUE(retval, window->menu, 1);
	}
	return SUCCESS;
}
/* }}} */

/* Identical to attach/detach Menu */
int wingui_window_write_menu(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	HMENU menu;
	wingui_menu_object *menu_object = NULL;
	wingui_window_object *window = wingui_window_object_juggle(object);

	/* grab previous menu, if there was one */
	if (window->menu) {
		Z_DELREF_P(window->menu);
	}
	/* a null value means we're detaching the menu */
	if (Z_TYPE_P(value) == IS_NULL) {
		menu = NULL;
	} else if (Z_TYPE_P(value) == IS_OBJECT &&
		instanceof_function(Z_OBJCE_P(value), ce_wingui_menu TSRMLS_CC)) {
		menu_object = (wingui_menu_object*)wingui_menu_object_get(value TSRMLS_CC);
		menu = menu_object->menu_handle;
	} else {
		zend_throw_exception(ce_wingui_exception, "Parent must be null or an instance of Win\\Gui\\Menu", 0 TSRMLS_CC);
		return FAILURE;
	}
	SetMenu(window->window_handle, menu);
	if(menu_object) {
		menu_object->window_handle = window->window_handle;
	}
	DrawMenuBar(window->window_handle);
	Z_ADDREF_P(value);
	return SUCCESS;
}
/* }}} */

/* juggle between top level, child, and popup windows */
int wingui_window_write_type(wingui_generic_object *object, zval *member, zval *value TSRMLS_DC)
{
	long style;
	HWND parent = NULL;
	wingui_window_object *window = wingui_window_object_juggle(object);

	value = wingui_juggle_type(value, IS_LONG TSRMLS_CC);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);

	/* Handle it becoming a child window */
	if (Z_LVAL_P(value) == 3) {
		/* Grab the stored parent handle */
		if (style & WS_POPUP) {
			parent = GetAncestor(window->window_handle, GA_PARENT);
		} else {
			parent = GetParent(window->window_handle);
		}

		if (!parent || !IsWindow(parent)) {
			zend_throw_exception(ce_wingui_exception, "A child window requires a valid parent window", 0 TSRMLS_CC);
			return FAILURE;
		}

		style &= ~WS_POPUP;
		style |= WS_CHILD;
	} else if (Z_LVAL_P(value) == 2)    {
		style &= ~WS_CHILD;
		style |= WS_POPUP;
	} else if (Z_LVAL_P(value) == 1) {
		SetParent(window->window_handle, NULL);
		style &= ~WS_CHILD;
		style &= ~WS_POPUP;
	} else {
		zend_throw_exception(ce_wingui_exception, "Valid window types are Window::CHILD, Window::POPUP, and Window::TOPLEVEL", 0 TSRMLS_CC);
		return FAILURE;
	}
	SetWindowLongPtr(window->window_handle, GWL_STYLE, style);
	SetWindowPos(window->window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return SUCCESS;
}
/* }}} */

/* Grabs current window style and tells whether it is popup, tiled, or child */
int wingui_window_read_type(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	long style;
	wingui_window_object *window = wingui_window_object_juggle(object);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);
	if (style & WS_CHILD) {
		ZVAL_LONG(*retval, 3);
	} else if (style & WS_POPUP) {
		ZVAL_LONG(*retval, 2);
	} else {
		ZVAL_LONG(*retval, 1);
	}
	return SUCCESS;
}
/* }}} */

/* adds or subtracts different style flags */
int wingui_window_write_style(wingui_window_object *object, zval *member, zval *value TSRMLS_DC)
{
	int style, extrastyle;

	style = GetWindowLongPtr(object->window_handle, GWL_STYLE);

	if(strcmp(Z_STRVAL_P(member), "border") == 0)
	{
		value = wingui_juggle_type(value, IS_LONG TSRMLS_CC);
		extrastyle = GetWindowLongPtr(object->window_handle, GWL_EXSTYLE);
		/* remember WS_CAPTION is WS_BORDER | WS_DLGFRAME (stupid, yes) */
		if (Z_LVAL_P(value) == 1) // WS_BORDER
		{
			style |= WS_BORDER;
			if (!(style & WS_CAPTION))
			{
				style &= ~WS_DLGFRAME;
			}
			style &= ~WS_THICKFRAME;
			extrastyle &= ~WS_EX_DLGMODALFRAME;
		}
		else if (Z_LVAL_P(value) == 2) // WS_THICKFRAME
		{
			style |= WS_THICKFRAME;
			if (!(style & WS_CAPTION))
			{
				style &= ~WS_BORDER;
				style &= ~WS_DLGFRAME;
			}
			extrastyle &= ~WS_EX_DLGMODALFRAME;
		}
		else if (Z_LVAL_P(value) == 3) // WS_DLGFRAME && WS_EX_MODALDLGFRAME
		{
			style |= WS_DLGFRAME;
			if (!(style & WS_CAPTION))
			{
				style &= ~WS_BORDER;
			}
			style &= ~WS_THICKFRAME;
			extrastyle |= WS_EX_DLGMODALFRAME;
		}
		else // REMOVE ALL
		{
			if (style & WS_CAPTION)
			{
				zend_throw_exception(ce_wingui_exception, "A window with a caption must have a border", 0 TSRMLS_CC);
				return FAILURE;
			}
			style &= ~WS_BORDER;
			style &= ~WS_DLGFRAME;
			style &= ~WS_THICKFRAME;
			extrastyle &= ~WS_EX_DLGMODALFRAME;
		}
	}
	else
	{
		value = wingui_juggle_type(value, IS_BOOL TSRMLS_CC);
		if(strcmp(Z_STRVAL_P(member), "minimizebox") == 0)
		{
			extrastyle = GetWindowLongPtr(object->window_handle, GWL_EXSTYLE);
			if (Z_BVAL_P(value) && (extrastyle & WS_EX_CONTEXTHELP))
			{
				zend_throw_exception(ce_wingui_exception, "You cannot use helpbox with maximizebox or minimizebox", 0 TSRMLS_CC);
				return FAILURE;
			}
			else if (Z_BVAL_P(value) && !(style & WS_CAPTION))
			{
				zend_throw_exception(ce_wingui_exception, "You cannot use minimizebox without a caption enabled", 0 TSRMLS_CC);
				return FAILURE;
			}
			else if (Z_BVAL_P(value))
			{
				style |= WS_MINIMIZEBOX;
			}
			else
			{
				style &= ~WS_MINIMIZEBOX;
			}
		}
		if(strcmp(Z_STRVAL_P(member), "maximizebox") == 0)
		{
			extrastyle = GetWindowLongPtr(object->window_handle, GWL_EXSTYLE);
			if (Z_BVAL_P(value) && (extrastyle & WS_EX_CONTEXTHELP))
			{
				zend_throw_exception(ce_wingui_exception, "You cannot use helpbox with maximizebox or minimizebox", 0 TSRMLS_CC);
				return FAILURE;
			}
			else if (Z_BVAL_P(value) && !(style & WS_CAPTION))
			{
				zend_throw_exception(ce_wingui_exception, "You cannot use maximizebox without a caption enabled", 0 TSRMLS_CC);
				return FAILURE;
			}
			else if (Z_BVAL_P(value))
			{
				style |= WS_MAXIMIZEBOX;
			}
			else
			{
				style &= ~WS_MAXIMIZEBOX;
			}
		}
		if(strcmp(Z_STRVAL_P(member), "tabstop") == 0)
		{
			if (Z_BVAL_P(value))
			{
				style |= WS_TABSTOP;
			}
			else
			{
				style &= ~WS_TABSTOP;
			}
		}
		if(strcmp(Z_STRVAL_P(member), "clipchildren") == 0)
		{
			if (Z_BVAL_P(value))
			{
				style |= WS_CLIPCHILDREN;
			}
			else
			{
				style &= ~WS_CLIPCHILDREN;
			}
		}
		if(strcmp(Z_STRVAL_P(member), "clipsiblings") == 0)
		{
			if (!Z_BVAL_P(value) && !(style & WS_CHILD))
			{
				zend_throw_exception(ce_wingui_exception, "A POPUP or TILED window must have clipsiblings enabled", 0 TSRMLS_CC);
				return FAILURE;
			}
			else if (Z_BVAL_P(value))
			{
				style |= WS_CLIPSIBLINGS;
			}
			else
			{
				style &= ~WS_CLIPSIBLINGS;
			}
		}
		if(strcmp(Z_STRVAL_P(member), "hscroll") == 0)
		{
			if (Z_BVAL_P(value))
			{
				style |= WS_HSCROLL;
			}
			else
			{
				style &= ~WS_HSCROLL;
			}
		}
		if(strcmp(Z_STRVAL_P(member), "vscroll") == 0)
		{
			if (Z_BVAL_P(value))
			{
				style |= WS_VSCROLL;
			}
			else
			{
				style &= ~WS_VSCROLL;
			}
		}
		if(strcmp(Z_STRVAL_P(member), "caption") == 0)
		{
			if (Z_BVAL_P(value))
			{
				style |= WS_CAPTION;
			}
			else
			{
				style &= ~WS_CAPTION;
			}
		}
		if(strcmp(Z_STRVAL_P(member), "sysmenu") == 0)
		{
			if (Z_BVAL_P(value))
			{
				style |= WS_SYSMENU;
			}
			else
			{
				style &= ~WS_SYSMENU;
			}
		}
	}
	SetWindowLongPtr(object->window_handle, GWL_STYLE, style);
	SetWindowPos(object->window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return SUCCESS;
}
/* }}} */

/* Grabs current window style and tells whether it is popup, tiled, or child */
int wingui_window_read_style(wingui_generic_object *object, zval *member, zval **retval TSRMLS_DC)
{
	int style, extrastyle;
	wingui_window_object *window = wingui_window_object_juggle(object);

	style = GetWindowLongPtr(window->window_handle, GWL_STYLE);
	if(strcmp(Z_STRVAL_P(member), "border") == 0)
	{
		extrastyle = GetWindowLongPtr(window->window_handle, GWL_EXSTYLE);
		if (extrastyle & WS_EX_DLGMODALFRAME && style & WS_DLGFRAME)
		{
			ZVAL_LONG(*retval, 3);
		}
		else if (style & WS_THICKFRAME)
		{
			ZVAL_LONG(*retval, 2);
		}
		else if (style & WS_BORDER)
		{
			ZVAL_LONG(*retval, 1);
		}
		else
		{
			ZVAL_LONG(*retval, 0);
		}
	}
	else if ((strcmp(Z_STRVAL_P(member), "maximizebox") == 0 && style & WS_MAXIMIZEBOX) ||
	    (strcmp(Z_STRVAL_P(member), "minimizebox") == 0 && style & WS_MINIMIZEBOX) ||
	    (strcmp(Z_STRVAL_P(member), "tabstop") == 0 && style & WS_TABSTOP) ||
		(strcmp(Z_STRVAL_P(member), "clipchildren") == 0 && style & WS_CLIPCHILDREN) ||
		(strcmp(Z_STRVAL_P(member), "clipsiblings") == 0 && style & WS_CLIPSIBLINGS) ||
		(strcmp(Z_STRVAL_P(member), "vscroll") == 0 && style & WS_VSCROLL) ||
		(strcmp(Z_STRVAL_P(member), "hscroll") == 0 && style & WS_VSCROLL) ||
	    (strcmp(Z_STRVAL_P(member), "caption") == 0 && style & WS_CAPTION) ||
		(strcmp(Z_STRVAL_P(member), "sysmenu") == 0 && style & WS_SYSMENU))
	{
		ZVAL_BOOL(*retval, 1);
	}
	else
	{
		ZVAL_BOOL(*retval, 0);
	}
	return SUCCESS;

}
/* }}} */

/* adds or subtracts different extra style flags */
int wingui_window_write_extrastyle(wingui_window_object *object, zval *member, zval *value TSRMLS_DC)
{
	int style, extrastyle;

	value = wingui_juggle_type(value, IS_BOOL TSRMLS_CC);
	extrastyle = GetWindowLongPtr(object->window_handle, GWL_EXSTYLE);

	if(strcmp(Z_STRVAL_P(member), "helpbox") == 0)
	{
		style = GetWindowLongPtr(object->window_handle, GWL_STYLE);
		if (Z_BVAL_P(value) && (style & WS_MAXIMIZEBOX || style & WS_MINIMIZEBOX))
		{
			zend_throw_exception(ce_wingui_exception, "You cannot use helpbox with maximizebox or minimizebox", 0 TSRMLS_CC);
			return FAILURE;
		}
		else if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_CONTEXTHELP;
		}
		else
		{
			extrastyle &= ~WS_EX_CONTEXTHELP;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "textdir") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_RTLREADING;
		}
		else
		{
			extrastyle &= ~WS_EX_RTLREADING;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "align") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_RIGHT;
		}
		else
		{
			extrastyle &= ~WS_EX_RIGHT;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "scrollbar") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle &= ~WS_EX_LEFTSCROLLBAR;
		}
		else
		{
			extrastyle |= WS_EX_LEFTSCROLLBAR;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "composited") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_COMPOSITED;
		}
		else
		{
			extrastyle &= ~WS_EX_COMPOSITED;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "transparent") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_TRANSPARENT;
		}
		else
		{
			extrastyle &= ~WS_EX_TRANSPARENT;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "layered") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_LAYERED;
		}
		else
		{
			extrastyle &= ~WS_EX_LAYERED;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "inheritlayout") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle &= ~WS_EX_NOINHERITLAYOUT;
		}
		else
		{
			extrastyle |= WS_EX_NOINHERITLAYOUT;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "layout") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_LAYOUTRTL;
		}
		else
		{
			extrastyle &= ~WS_EX_LAYOUTRTL;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "notifyparent") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle &= ~WS_EX_NOPARENTNOTIFY;
		}
		else
		{
			extrastyle |= WS_EX_NOPARENTNOTIFY;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "controlparent") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_CONTROLPARENT;
		}
		else
		{
			extrastyle &= ~WS_EX_CONTROLPARENT;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "forcetaskbar") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_APPWINDOW;
		}
		else
		{
			extrastyle &= ~WS_EX_APPWINDOW;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "staticborder") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_STATICEDGE;
		}
		else
		{
			extrastyle &= ~WS_EX_STATICEDGE;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "raisedborder") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_WINDOWEDGE;
		}
		else
		{
			extrastyle &= ~WS_EX_WINDOWEDGE;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "sunkenborder") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_CLIENTEDGE;
		}
		else
		{
			extrastyle &= ~WS_EX_CLIENTEDGE;
		}
	}
	else if(strcmp(Z_STRVAL_P(member), "toolwindow") == 0)
	{
		if (Z_BVAL_P(value))
		{
			extrastyle |= WS_EX_TOOLWINDOW;
		}
		else
		{
			extrastyle &= ~WS_EX_TOOLWINDOW;
		}
	}
	SetWindowLongPtr(object->window_handle, GWL_EXSTYLE, extrastyle);
	SetWindowPos(object->window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return SUCCESS;
}
/* }}} */

/* Grabs current window extra style */
int wingui_window_read_extrastyle(wingui_window_object *object, zval *member, zval **retval TSRMLS_DC)
{
	long extrastyle;

	extrastyle = GetWindowLongPtr(object->window_handle, GWL_EXSTYLE);
	if ((strcmp(Z_STRVAL_P(member), "helpbox") == 0 && extrastyle & WS_EX_CONTEXTHELP) ||
		(strcmp(Z_STRVAL_P(member), "layered") == 0 && extrastyle & WS_EX_LAYERED) ||
		(strcmp(Z_STRVAL_P(member), "composited") == 0 && extrastyle & WS_EX_COMPOSITED) ||
		(strcmp(Z_STRVAL_P(member), "transparent") == 0 && extrastyle & WS_EX_TRANSPARENT) ||
		(strcmp(Z_STRVAL_P(member), "inheritlayout") == 0 && !(extrastyle & WS_EX_NOINHERITLAYOUT)) ||
		(strcmp(Z_STRVAL_P(member), "notifyparent") == 0 && !(extrastyle & WS_EX_NOPARENTNOTIFY)) ||
		(strcmp(Z_STRVAL_P(member), "controlparent") == 0 && extrastyle & WS_EX_CONTROLPARENT) ||
		(strcmp(Z_STRVAL_P(member), "forcetaskbar") == 0 && extrastyle & WS_EX_APPWINDOW) ||
		(strcmp(Z_STRVAL_P(member), "staticborder") == 0 && extrastyle & WS_EX_STATICEDGE) ||
		(strcmp(Z_STRVAL_P(member), "raisedborder") == 0 && extrastyle & WS_EX_WINDOWEDGE) ||
		(strcmp(Z_STRVAL_P(member), "toolwindow") == 0 && extrastyle & WS_EX_TOOLWINDOW) ||
		(strcmp(Z_STRVAL_P(member), "sunkenborder") == 0 && extrastyle & WS_EX_CLIENTEDGE))
	{
		ZVAL_BOOL(*retval, 1);
	}
	else if (strcmp(Z_STRVAL_P(member), "textdir") == 0)
	{
		if (extrastyle & WS_EX_RTLREADING)
		{
			ZVAL_LONG(*retval, 1);
		}
		else
		{
			ZVAL_LONG(*retval, 0);
		}
	}
	else if (strcmp(Z_STRVAL_P(member), "align") == 0)
	{
		if (extrastyle & WS_EX_RIGHT)
		{
			ZVAL_LONG(*retval, 1);
		}
		else
		{
			ZVAL_LONG(*retval, 0);
		}
	}
	else if (strcmp(Z_STRVAL_P(member), "scrollbar") == 0)
	{
		if (extrastyle & WS_EX_LEFTSCROLLBAR)
		{
			ZVAL_LONG(*retval, 0);
		}
		else
		{
			ZVAL_LONG(*retval, 1);
		}
	}
	else if (strcmp(Z_STRVAL_P(member), "layout") == 0)
	{
		if (extrastyle & WS_EX_LAYOUTRTL)
		{
			ZVAL_LONG(*retval, 1);
		}
		else
		{
			ZVAL_LONG(*retval, 0);
		}
	}
	else
	{
		ZVAL_BOOL(*retval, 0);
	}
	return SUCCESS;

}
/* }}} */

/* Helper to translate client -> window width and height */
int wingui_client_to_window_size(long *x, long *y, long *width, long *height, long style, long extrastyle, BOOL menu) {
	RECT client;
	
	/* add scrollbars if needed to requested client width because AdjustWindowRectEx doesn't include them */
	if(style & WS_VSCROLL) {
		width += GetSystemMetrics(SM_CXVSCROLL);
	}
	if(style & WS_HSCROLL) {
		height += GetSystemMetrics(SM_CYHSCROLL);
	}

	client.top = *x;
	client.left = *y;
	client.bottom = *height;
	client.right = *width;
	if(AdjustWindowRectEx(&client, style, menu, extrastyle)) {
		*height = client.bottom;
		*width = client.right;
		return SUCCESS;
	}
	return FAILURE;
}
/* }}} */

--TEST--
Win\Gui\Window::popupPosition() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(!Win\Gui::isWin7()) die('skip - Win\Gui\Window::popupPosition not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// calculate our popup window's new position
var_dump(Window::popupPosition(1, 1, 50, 50));

/* Wrong number args - 1 */
try {
    Window::popupPosition();
    trigger_error('popupPosition requires 4 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    Window::popupPosition(1);
    trigger_error('popupPosition requires at least 4 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 3 */
try {
    Window::popupPosition(1, 1);
    trigger_error('popupPosition requires at least 4 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 4 */
try {
    Window::popupPosition(1, 1, 1);
    trigger_error('popupPosition requires at least 4 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 5 */
try {
    Window::popupPosition(1, 1, 1, 1, 1, array(), 1);
    trigger_error('popupPosition requires at most 6 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    Window::popupPosition(array(), 1 ,1 , 1);
    trigger_error('popupPosition requires arg 1 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    Window::popupPosition(1, array() ,1 , 1);
    trigger_error('popupPosition requires arg 2 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    Window::popupPosition(1, 1 ,array() , 1);
    trigger_error('popupPosition requires arg 3 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    Window::popupPosition(1, 1 ,1 , array());
    trigger_error('popupPosition requires arg 4 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    Window::popupPosition(1, 1 ,1 , 1, array());
    trigger_error('popupPosition requires arg 5 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    Window::popupPosition(1, 1, 1 , 1, 1, 1);
    trigger_error('popupPosition requires arg 6 to be array');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
array(4) {
  ["left"]=>
  int(%d)
  ["right"]=>
  int(%d)
  ["bottom"]=>
  int(%d)
  ["top"]=>
  int(%d)
}
Win\Gui\Window::popupPosition() expects at least 4 parameters, 0 given
Win\Gui\Window::popupPosition() expects at least 4 parameters, 1 given
Win\Gui\Window::popupPosition() expects at least 4 parameters, 2 given
Win\Gui\Window::popupPosition() expects at least 4 parameters, 3 given
Win\Gui\Window::popupPosition() expects at most 6 parameters, 7 given
Win\Gui\Window::popupPosition() expects parameter 1 to be long, array given
Win\Gui\Window::popupPosition() expects parameter 2 to be long, array given
Win\Gui\Window::popupPosition() expects parameter 3 to be long, array given
Win\Gui\Window::popupPosition() expects parameter 4 to be long, array given
Win\Gui\Window::popupPosition() expects parameter 5 to be long, array given
Win\Gui\Window::popupPosition() expects parameter 6 to be array, integer given
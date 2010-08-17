--TEST--
Win\Gui\Windowing->adjustSize() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

$window = new Window();

var_dump($window->adjustSize(1, 3));

/* Wrong number args - 0 */
try {
    $window->adjustSize();
    trigger_error('Windowing->adjustSize requires 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 1 */
try {
    $window->adjustSize(1);
    trigger_error('Windowing->adjustSize requires 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 3 */
try {
    $window->adjustSize(1, 1, 1);
    trigger_error('Windowing->adjustSize requires only 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 1 */
try {
    $window->adjustSize(array(), 1);
    trigger_error('Windowing->adjustSize requires arg 1 as int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 2 */
try {
    $window->adjustSize(1, array());
    trigger_error('Windowing->adjustSize requires arg 2 as int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
array(2) {
  [0]=>
  int(%d)
  [1]=>
  int(%d)
}
Win\Gui\Windowing::adjustSize() expects exactly 2 parameters, 0 given
Win\Gui\Windowing::adjustSize() expects exactly 2 parameters, 1 given
Win\Gui\Windowing::adjustSize() expects exactly 2 parameters, 3 given
Win\Gui\Windowing::adjustSize() expects parameter 1 to be long, array given
Win\Gui\Windowing::adjustSize() expects parameter 2 to be long, array given
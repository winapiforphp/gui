--TEST--
Win\Gui\Control\ListView->approximateSize() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\ListView;

$window = new Window;
$listview = new ListView($window);

var_dump($listview->approximateSize());

var_dump($listview->approximateSize(1));

var_dump($listview->approximateSize(25, 25, 1));

/* Wrong number args - 1 */
try {
    $listview->approximateSize(1, 1, 1, 1);
    trigger_error('ListView->approximateSize requires at most 3 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 1 */
try {
    $listview->approximateSize(array());
    trigger_error('ListView->approximateSize requires arg 1 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 2 */
try {
    $listview->approximateSize(1, array());
    trigger_error('ListView->approximateSize requires arg 2 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 2 */
try {
    $listview->approximateSize(1, 1, array());
    trigger_error('ListView->approximateSize requires arg 3 to be int');
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
array(2) {
  [0]=>
  int(%d)
  [1]=>
  int(%d)
}
array(2) {
  [0]=>
  int(%d)
  [1]=>
  int(%d)
}
Win\Gui\Control\ListView::approximateSize() expects at most 3 parameters, 4 given
Win\Gui\Control\ListView::approximateSize() expects parameter 1 to be long, array given
Win\Gui\Control\ListView::approximateSize() expects parameter 2 to be long, array given
Win\Gui\Control\ListView::approximateSize() expects parameter 3 to be long, array given
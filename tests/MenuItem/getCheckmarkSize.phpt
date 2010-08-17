--TEST--
Win\Gui\MenuItem::getCheckmarkSize() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\MenuItem;

// get the default checkmarksize
var_dump(MenuItem::getCheckmarkSize());

/* Wrong number args - 1 */
try {
    MenuItem::getCheckmarkSize(1);
    trigger_error('getCheckmarkSize requires no args');
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
Win\Gui\MenuItem::getCheckmarkSize() expects exactly 0 parameters, 1 given
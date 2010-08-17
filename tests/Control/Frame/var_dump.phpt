--TEST--
Win\Gui\Control\Frame var_dump output
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
$window = new Win\Gui\Window;

$frame = new Win\Gui\Control\Frame($window);
var_dump($frame);

?>
--EXPECTF--
object(Win\Gui\Control\Frame)#%d (10) {
  ["x"]=>
  int(%d)
  ["y"]=>
  int(%d)
  ["width"]=>
  int(%d)
  ["height"]=>
  int(%d)
  ["text"]=>
  NULL
  ["sunken"]=>
  bool(false)
  ["notify"]=>
  bool(false)
  ["filled"]=>
  bool(false)
  ["color"]=>
  int(1)
  ["parent"]=>
  NULL
}
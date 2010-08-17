--TEST--
Win\Gui\MenuItem->__construct() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\MenuItem;

$item = new MenuItem(1, 'Item');
var_dump($item);

?>
--EXPECTF--
object(Win\Gui\MenuItem)#%d (8) {
  ["id"]=>
  int(%d)
  ["text"]=>
  string(4) "Item"
  ["enabled"]=>
  bool(true)
  ["hilite"]=>
  bool(false)
  ["checked"]=>
  bool(false)
  ["default"]=>
  bool(false)
  ["radio"]=>
  bool(false)
  ["image"]=>
  NULL
}
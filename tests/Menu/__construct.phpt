--TEST--
Win\Gui\Menu->__construct() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Menu;

$menu = new Menu();
var_dump($menu);

?>
--EXPECTF--
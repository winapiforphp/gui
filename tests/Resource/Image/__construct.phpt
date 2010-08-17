--TEST--
Win\Gui\Resource\Image->__construct() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Resource\Image;

try {
   new Image;
} catch (Win\Gui\Exception $e) {
    echo $e->getMessage();
}
?>
--EXPECTF--
Win\Gui\Resource\Image cannot be extended, extend a child control instead
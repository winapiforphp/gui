--TEST--
Win\Gui\Resource\Bitmap->__construct() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Resource\Bitmap;

$item = new Bitmap(__DIR__ . DIRECTORY_SEPARATOR . 'snowflake.bmp');
var_dump($item);

try {
    $item = new Bitmap(__DIR__ . DIRECTORY_SEPARATOR . time() . '.bmp');
} catch (Win\Gui\Exception $e) {
    echo $e->getMessage();
}
?>
--EXPECTF--
object(Win\Gui\Resource\Bitmap)#%d (0) {
}
The system cannot find the file specified.

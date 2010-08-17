--TEST--
Win\Gui\Control\Image->getImage() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Image;
use Win\Gui\Resource\Bitmap;
use Win\Gui\Resource\Cursor;
use Win\Gui\Resource\Icon;

$window = new Window;
$bitmap = Bitmap::load(Bitmap::OBM_CLOSE);
$icon = Icon::load(Icon::OIC_SHIELD);
$cursor = Cursor::load(Cursor::OCR_WAIT);

$image = new Image($window, $bitmap);
var_dump($bitmap === $image->getImage());

$image = new Image($window, $icon);
var_dump($icon === $image->getImage());

$image = new Image($window, $cursor);
var_dump($cursor === $image->getImage());

$image = new Image($window, null);
var_dump(null === $image->getImage());

/* Wrong number args - 1 */
try {
    $image->getImage(1);
    trigger_error('Image::getImage requires no arguments');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
Win\Gui\Control\Image::getImage() expects exactly 0 parameters, 1 given
--TEST--
Win\Gui\Control\Image->setImage() method
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
$image = new Image($window, null);

$bitmap = Bitmap::load(Bitmap::OBM_CLOSE);
$icon = Icon::load(Icon::OIC_SHIELD);
$cursor = Cursor::load(Cursor::OCR_WAIT);

var_dump(null === $image->setImage($bitmap));
var_dump($bitmap === $image->setImage($cursor));
var_dump($cursor === $image->setImage($icon));
var_dump($icon === $image->setImage(null));

/* Wrong number args - 1 */
try {
    $image->setImage();
    trigger_error('Image::setImage requires 1 argument');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    $image->setImage(null, 1);
    trigger_error('Image::setImage only 1 argument');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    $image->setImage(1);
    trigger_error('Image::setImage requires arg 1 to be null or instanceof Win\Gui\Resource\Cursor or Bitmap or Icon');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
Win\Gui\Control\Image::setImage() expects exactly 1 parameter, 0 given
Win\Gui\Control\Image::setImage() expects exactly 1 parameter, 2 given
Win\Gui\Control\Image::setImage() expects parameter 1 to be Win\Gui\Resource\Image, integer given
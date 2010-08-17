--TEST--
Win\Gui\Control\Image->__construct() method
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
    new Image;
    trigger_error('Image::__construct requires at least 2 arguments');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    new Image($window);
    trigger_error('Image::__construct requires at least 2 arguments');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 3 */
try {
    new Image($window, null, array('x' => 25), 1);
    trigger_error('Image::__construct requires no more than 3 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
   new Image(1, null);
    trigger_error('Image::__construct requires arg 2 to be null or instanceof Win\Gui\Resource\Image');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
   new Image($window, null, 1);
    trigger_error('Image::__construct requires arg 3 to be array');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
Win\Gui\Control\Image::__construct() expects at least 2 parameters, 0 given
Win\Gui\Control\Image::__construct() expects at least 2 parameters, 1 given
Win\Gui\Control\Image::__construct() expects at most 3 parameters, 4 given
Win\Gui\Control\Image::__construct() expects parameter 1 to be Win\Gui\Window, integer given
Win\Gui\Control\Image::__construct() expects parameter 3 to be array, integer given
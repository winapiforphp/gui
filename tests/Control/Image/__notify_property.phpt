--TEST--
Win\Gui\Control\Image->notify property
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Image;

$window = new Window;

// default is false
$image = new Image($window, null);
var_dump($image->notify);

// option to true
$image = new Image($window, null, array('notify' => true));
var_dump($image->notify);
echo PHP_EOL;

// set to true via properties
class Notify extends Image {
    public $notify = true;
}
$image = new Notify($window, null);
var_dump($image->notify);
echo PHP_EOL;

// default is false
$image = new Image($window, null);
var_dump($image->notify);

// change to true
$image->notify = true;
var_dump($image->notify);
echo PHP_EOL;

// isset check
if (!isset($image->notify)) {
    trigger_error('Image property notify should be set');
}

// unset check
try {
    unset($image->notify);
    trigger_error('Image property notify should not be unsetable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// var_dump the properties
ob_start();
var_dump($image);
$string = ob_get_clean();
if (strstr($string, '["notify"]') === 0) {
    trigger_error('Image property notify should be in var_dump');
}
?>
--EXPECT--
bool(false)
bool(true)

bool(true)

bool(false)
bool(true)

Internal properties cannot be unset
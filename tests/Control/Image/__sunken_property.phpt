--TEST--
Win\Gui\Control\Image->sunken property
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
var_dump($image->sunken);

// option to true
$image = new Image($window, null, array('sunken' => true));
var_dump($image->sunken);
echo PHP_EOL;

// set to true via properties
class Sunken extends Image {
    public $sunken = true;
}
$image = new Sunken($window, null);
var_dump($image->sunken);
echo PHP_EOL;

// default is false
$image = new Image($window, null);
var_dump($image->sunken);

// change to true
$image->sunken = true;
var_dump($image->sunken);
echo PHP_EOL;

// isset check
if (!isset($image->sunken)) {
    trigger_error('Image property sunken should be set');
}

// unset check
try {
    unset($image->sunken);
    trigger_error('Image property notify sunken not be unsetable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// var_dump the properties
ob_start();
var_dump($frame);
$string = ob_get_clean();
if (strstr($string, '["sunken"]') === 0) {
    trigger_error('Image property sunken should be in var_dump');
}
?>
--EXPECT--
bool(false)
bool(true)

bool(true)

bool(false)
bool(true)

Internal properties cannot be unset
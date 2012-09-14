--TEST--
Win\Gui\Window->__construct() method
--SKIPIF--
<?php
include __DIR__ . '/../../skipif.inc';
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\System\Unicode;
use Win\System\Codepage;
use Win\System\InvalidArgumentException;

/* Part 1 - testing setting the window title via construct arguments
   a) default title
   b) default title forcing unicode
   c) ansi title
   d) uft8 title forcing unicode
   e) utf16 title
   */
$window = new Window([]);
echo $window->getText(), PHP_EOL;

$window = new Window(['text' => 'mywindow']);
echo $window->getText(), PHP_EOL;

$string = 'काचं शक्नोम्यत्तुम् । नोपहिनस्ति माम् ॥';
$window = new Window(['text' => $string, 'use_unicode' => true]);
echo $window->getText(), PHP_EOL;

$unicode = new Unicode(file_get_contents(__DIR__ . '/../utf16.txt'), new CodePage(CodePage::UTF16));
$window = new Window(['text' => $unicode]);
echo $window->getText(), PHP_EOL;

/* Part 2 - testing x, y, height and width */
$window = new Window(['x' => 100, 'y' => 100, 'width' => 300, 'height' => 300]);
var_dump($window->getSize());
var_dump($window->getPos());

/* Part 3 - window styles - lots of various flags */
$window = new Window([]);
var_dump($window->isEnabled());
$window = new Window(['disable' => true]);
var_dump($window->isEnabled());

/* TODO YET: 
	// TODO: style and extrastyle parsing
	// TODO: parent
	// TODO: menu
	// TODO: classname */
?>
= DONE =
--EXPECT--
Window
mywindow
काचं शक्नोम्यत्तुम् । नोपहिनस्ति माम् ॥
﻿日本語
object(stdClass)#3 (2) {
  ["height"]=>
  int(300)
  ["width"]=>
  int(300)
}
object(stdClass)#3 (2) {
  ["x"]=>
  int(100)
  ["y"]=>
  int(100)
}
= DONE =
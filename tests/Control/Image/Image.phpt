--TEST--
Win\Gui\Control\Image Reflection
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
ReflectionClass::export('Win\Gui\Control\Image');
?>
--EXPECT--
Class [ <internal:wingui> class Win\Gui\Control\Image extends Win\Gui\Control implements Win\Gui\Messaging ] {

  - Constants [34] {
    Constant [ integer TOPLEVEL ] { 1 }
    Constant [ integer POPUP ] { 2 }
    Constant [ integer CHILD ] { 3 }
    Constant [ integer WM_ACTIVATEAPP ] { 28 }
    Constant [ integer WM_CANCELMODE ] { 31 }
    Constant [ integer WM_CHILDACTIVATE ] { 34 }
    Constant [ integer WM_CLOSE ] { 16 }
    Constant [ integer WM_DESTROY ] { 2 }
    Constant [ integer WM_ENABLE ] { 10 }
    Constant [ integer WM_ENTERSIZEMOVE ] { 561 }
    Constant [ integer WM_EXITSIZEMOVE ] { 562 }
    Constant [ integer WM_GETICON ] { 127 }
    Constant [ integer WM_GETMINMAXINFO ] { 36 }
    Constant [ integer WM_INPUTLANGCHANGE ] { 81 }
    Constant [ integer WM_INPUTLANGCHANGEREQUEST ] { 80 }
    Constant [ integer WM_MOVE ] { 3 }
    Constant [ integer WM_MOVING ] { 534 }
    Constant [ integer WM_NCACTIVATE ] { 134 }
    Constant [ integer WM_NCCALCSIZE ] { 131 }
    Constant [ integer WM_NCDESTROY ] { 130 }
    Constant [ integer WM_NULL ] { 0 }
    Constant [ integer WM_PARENTNOTIFY ] { 528 }
    Constant [ integer WM_QUERYDRAGICON ] { 55 }
    Constant [ integer WM_QUERYOPEN ] { 19 }
    Constant [ integer WM_SHOWWINDOW ] { 24 }
    Constant [ integer WM_SIZE ] { 5 }
    Constant [ integer WM_SIZING ] { 532 }
    Constant [ integer WM_STYLECHANGED ] { 125 }
    Constant [ integer WM_STYLECHANGING ] { 124 }
    Constant [ integer WM_THEMECHANGED ] { 794 }
    Constant [ integer WM_USERCHANGED ] { 84 }
    Constant [ integer WM_WINDOWPOSCHANGED ] { 71 }
    Constant [ integer WM_WINDOWPOSCHANGING ] { 70 }
    Constant [ integer WM_COMMAND ] { 273 }
  }

  - Static properties [0] {
  }

  - Static methods [18] {
    Method [ <internal:wingui, inherits Win\Gui\Window> static public method allowSetForeground ] {

      - Parameters [2] {
        Parameter #0 [ <required> $process_id ]
        Parameter #1 [ <optional> $allow_any ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> static public method arrangeMinimized ] {

      - Parameters [1] {
        Parameter #0 [ <optional> $parent ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> static public method popupPosition ] {

      - Parameters [6] {
        Parameter #0 [ <required> $x ]
        Parameter #1 [ <required> $y ]
        Parameter #2 [ <required> $width ]
        Parameter #3 [ <required> $height ]
        Parameter #4 [ <optional> $flags ]
        Parameter #5 [ <optional> $exclude ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> static public method cascade ] {

      - Parameters [4] {
        Parameter #0 [ <optional> $parent ]
        Parameter #1 [ <optional> $flags ]
        Parameter #2 [ <optional> $rect ]
        Parameter #3 [ <optional> $kids ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> static public method addFilterMessage ] {

      - Parameters [1] {
        Parameter #0 [ <required> $message ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> static public method removeFilterMessage ] {

      - Parameters [1] {
        Parameter #0 [ <required> $message ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> static public method tile ] {

      - Parameters [4] {
        Parameter #0 [ <optional> $parent ]
        Parameter #1 [ <optional> $flags ]
        Parameter #2 [ <optional> $rect ]
        Parameter #3 [ <optional> $kids ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> static public method soundSentry ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> static public method getFromPhysicalPoint ] {

      - Parameters [2] {
        Parameter #0 [ <required> $x ]
        Parameter #1 [ <required> $y ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> static public method getFromPoint ] {

      - Parameters [2] {
        Parameter #0 [ <required> $x ]
        Parameter #1 [ <required> $y ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> static public method block ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> static public method unblock ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> static public method getActive ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> static public method getAsyncKeyState ] {

      - Parameters [1] {
        Parameter #0 [ <required> $key ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> static public method getFocus ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> static public method getKeyState ] {

      - Parameters [1] {
        Parameter #0 [ <required> $key ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> static public method peek ] {

      - Parameters [2] {
        Parameter #0 [ <required> $filtermin ]
        Parameter #1 [ <optional> $filtermax ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> static public method sendNotifyMessage ] {

      - Parameters [1] {
        Parameter #0 [ <required> $msg ]
      }
    }
  }

  - Properties [8] {
    Property [ <default> public $x ]
    Property [ <default> public $y ]
    Property [ <default> public $width ]
    Property [ <default> public $height ]
    Property [ <default> public $text ]
    Property [ <default> public $parent ]
    Property [ <default> public $sunken ]
    Property [ <default> public $notify ]
  }

  - Methods [30] {
    Method [ <internal:wingui, overwrites Win\Gui\Control, ctor> public method __construct ] {

      - Parameters [2] {
        Parameter #0 [ <required> $parent ]
        Parameter #1 [ <required> $image ]
      }
    }

    Method [ <internal:wingui> public method setImage ] {

      - Parameters [1] {
        Parameter #0 [ <required> $image ]
      }
    }

    Method [ <internal:wingui> public method getImage ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method animateShow ] {

      - Parameters [2] {
        Parameter #0 [ <optional> $time ]
        Parameter #1 [ <optional> $flags ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method animateHide ] {

      - Parameters [3] {
        Parameter #0 [ <optional> $time ]
        Parameter #1 [ <optional> $flags ]
        Parameter #2 [ <optional> $activate ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method bringToTop ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method isMinimized ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method isVisible ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method showOwnedPopups ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method hideOwnedPopups ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method hide ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method setText ] {

      - Parameters [1] {
        Parameter #0 [ <optional> $text ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method getText ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method minimize ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method maximize ] {

      - Parameters [1] {
        Parameter #0 [ <optional> $noactivate ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method restore ] {

      - Parameters [1] {
        Parameter #0 [ <optional> $noactivate ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method show ] {

      - Parameters [1] {
        Parameter #0 [ <optional> $noactivate ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method showMinimized ] {

      - Parameters [1] {
        Parameter #0 [ <optional> $noactivate ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method attachMenu ] {

      - Parameters [1] {
        Parameter #0 [ <required> Win\Gui\Menu $menu ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Window> public method detachMenu ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> public method enable ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> public method disable ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> public method isEnabled ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> public method activate ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> public method deactivate ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Input> public method isActive ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method connect ] {

      - Parameters [2] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $callback ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method connectFull ] {

      - Parameters [2] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $callback ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method connectSimple ] {

      - Parameters [2] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $callback ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method disconnect ] {

      - Parameters [2] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $id ]
      }
    }
  }
}
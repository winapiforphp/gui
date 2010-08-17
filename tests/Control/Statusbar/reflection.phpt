--TEST--
Win\Gui\Control\Statusbar reflection output
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
Reflection::export(new ReflectionClass('Win\Gui\Control\Statusbar'));
?>
--EXPECT--
Class [ <internal:wingui> class Win\Gui\Control\Statusbar extends Win\Gui\Control implements Win\Gui\Windowing, Win\Gui\Inputing, Win\Gui\Messaging ] {

  - Constants [10] {
    Constant [ integer NOBORDERS ] { 256 }
    Constant [ integer OWNERDRAW ] { 4096 }
    Constant [ integer POPOUT ] { 512 }
    Constant [ integer RTLREADING ] { 1024 }
    Constant [ integer NOTABPARSING ] { 2048 }
    Constant [ integer NM_CLICK ] { -2 }
    Constant [ integer NM_DBLCLK ] { -3 }
    Constant [ integer NM_RCLICK ] { -5 }
    Constant [ integer NM_RDBLCLK ] { -6 }
    Constant [ integer SBN_SIMPLEMODECHANGE ] { -880 }
  }

  - Static properties [0] {
  }

  - Static methods [0] {
  }

  - Properties [0] {
  }

  - Methods [35] {
    Method [ <internal:wingui, overwrites Win\Gui\Control, ctor> public method __construct ] {

      - Parameters [1] {
        Parameter #0 [ <optional> $options ]
      }
    }

    Method [ <internal:wingui> public method getBorders ] {

      - Parameters [0] {
      }
    }

    Method [ <internal:wingui> public method setSimple ] {

      - Parameters [1] {
        Parameter #0 [ <required> $simple ]
      }
    }

    Method [ <internal:wingui> public method isSimple ] {

      - Parameters [0] {
      }
    }

    Method [ <internal:wingui> public method getParts ] {

      - Parameters [0] {
      }
    }

    Method [ <internal:wingui> public method setParts ] {

      - Parameters [1] {
        Parameter #0 [ <required> $parts ]
      }
    }

    Method [ <internal:wingui> public method setText ] {

      - Parameters [3] {
        Parameter #0 [ <required> $string ]
        Parameter #1 [ <optional> $part ]
        Parameter #2 [ <optional> $type ]
      }
    }

    Method [ <internal:wingui> public method getTextLength ] {

      - Parameters [1] {
        Parameter #0 [ <optional> $part ]
      }
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

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method get ] {

      - Parameters [2] {
        Parameter #0 [ <optional> $filtermin ]
        Parameter #1 [ <optional> $filtermax ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method peek ] {

      - Parameters [3] {
        Parameter #0 [ <optional> $filtermin ]
        Parameter #1 [ <optional> $filtermax ]
        Parameter #2 [ <optional> $remove ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method post ] {

      - Parameters [1] {
        Parameter #0 [ <required> $msg ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method send ] {

      - Parameters [1] {
        Parameter #0 [ <required> $msg ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method sendCallback ] {

      - Parameters [3] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $callback ]
        Parameter #2 [ <required> $data ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method sendTimeout ] {

      - Parameters [3] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $flags ]
        Parameter #2 [ <required> $timeout ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method enable ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method disable ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method isEnabled ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method activate ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method deactivate ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method isActive ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method focus ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method unfocus ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method hasFocus ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method capture ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method release ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method hasCapture ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method trackMouse ] {

      - Parameters [2] {
        Parameter #0 [ <required> $type ]
        Parameter #1 [ <required> $time ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method queryMouse ] {

      - Parameters [2] {
        Parameter #0 [ <required> $type ]
        Parameter #1 [ <required> $time ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Inputing> public method dragDetect ] {

      - Parameters [2] {
        Parameter #0 [ <required> $x ]
        Parameter #1 [ <required> $y ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method adjustSize ] {

      - Parameters [2] {
        Parameter #0 [ <required> $height ]
        Parameter #1 [ <required> $width ]
      }
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method show ] {
    }
  }
}
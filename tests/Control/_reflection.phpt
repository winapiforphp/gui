--TEST--
Win\Gui\Control reflection output
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
Reflection::export(new ReflectionClass('Win\Gui\Control'));
?>
--EXPECT--
Class [ <internal:wingui> abstract class Win\Gui\Control implements Win\Gui\Messaging, Win\Gui\Inputing, Win\Gui\Windowing ] {

  - Constants [32] {
    Constant [ integer TOP ] { 1 }
    Constant [ integer BOTTOM ] { 2 }
    Constant [ integer LEFT ] { 1 }
    Constant [ integer RIGHT ] { 2 }
    Constant [ integer HOR ] { 0 }
    Constant [ integer VERT ] { 1 }
    Constant [ integer CCM_DPISCALE ] { 8204 }
    Constant [ integer CCM_GETUNICODEFORMAT ] { 8198 }
    Constant [ integer CCM_GETVERSION ] { 8200 }
    Constant [ integer CCM_SETUNICODEFORMAT ] { 8197 }
    Constant [ integer CCM_SETVERSION ] { 8199 }
    Constant [ integer CCM_SETWINDOWTHEME ] { 8203 }
    Constant [ integer WM_NOTIFY ] { 78 }
    Constant [ integer NM_CHAR ] { -18 }
    Constant [ integer NM_CUSTOMDRAW ] { -12 }
    Constant [ integer NM_CUSTOMTEXT ] { -24 }
    Constant [ integer NM_FONTCHANGED ] { -23 }
    Constant [ integer NM_GETCUSTOMSPLITRECT ] { -1247 }
    Constant [ integer NM_HOVER ] { -13 }
    Constant [ integer NM_KEYDOWN ] { -15 }
    Constant [ integer NM_KILLFOCUS ] { -8 }
    Constant [ integer NM_LDOWN ] { -20 }
    Constant [ integer NM_NCHITTEST ] { -14 }
    Constant [ integer NM_OUTOFMEMORY ] { -1 }
    Constant [ integer NM_RDOWN ] { -21 }
    Constant [ integer NM_RELEASEDCAPTURE ] { -16 }
    Constant [ integer NM_RETURN ] { -4 }
    Constant [ integer NM_SETCURSOR ] { -17 }
    Constant [ integer NM_SETFOCUS ] { -7 }
    Constant [ integer NM_THEMECHANGED ] { -22 }
    Constant [ integer NM_TOOLTIPSCREATED ] { -19 }
    Constant [ integer NM_TVSTATEIMAGECHANGING ] { -24 }
  }

  - Static properties [0] {
  }

  - Static methods [0] {
  }

  - Properties [8] {
    Property [ <default> public $halign ]
    Property [ <default> public $valign ]
    Property [ <default> public $orientation ]
    Property [ <default> public $divider ]
    Property [ <default> public $resize ]
    Property [ <default> public $parentalign ]
    Property [ <default> public $movex ]
    Property [ <default> public $movey ]
  }

  - Methods [58] {
    Method [ <internal:wingui, ctor> public method __construct ] {
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

    Method [ <internal:wingui, inherits Win\Gui\Messaging> public method sendNotify ] {

      - Parameters [1] {
        Parameter #0 [ <required> $msg ]
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

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method animateShow ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method animateHide ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method bringToTop ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getChildFromPoint ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method foreachChild ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method findChild ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getAncestor ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getClientSize ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getNext ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getPrevious ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getParent ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getTopChild ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getOwner ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getFirst ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getLast ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getChild ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getSize ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getText ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getProcessId ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getPlacement ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getTextDirect ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method isChild ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method isVisible ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method logicalToPhysical ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method move ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method physicalToLogical ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method getVisibleChild ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method setParent ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method setPlacement ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method setPos ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method setText ] {
    }

    Method [ <internal:wingui, inherits Win\Gui\Windowing> public method show ] {
    }
  }
}
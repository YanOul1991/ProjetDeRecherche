#include "Global/OptimEngineGlobal.h"

namespace op::input
{
  enum class EMouseAction : uint8
  {
    ButtonOneDown,
    ButtonOneUp,
    ButtonTwoDown,
    ButtonTwoUp,
    ButtonThreeDown,
    ButtonThreeUp,
    ButtonFourDown,
    ButtonFourUp,
    ButtonFiveDown,
    ButtonFiveUp,
    ScrollUp,
    ScrollDown
  };

  struct MouseInputData
  {
    int32 xDelta;
    int32 yDelta;
    int32 xPosition;
    int32 yPosition;
    EMouseAction action;
  };

  struct GamepadInputData
  {
    uint32  sticks;
    uint8   padAndSymbols;
    uint8   buttons;
  };
}
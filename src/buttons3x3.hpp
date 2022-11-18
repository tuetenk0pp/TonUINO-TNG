#ifndef SRC_BUTTONS3X3_HPP_
#define SRC_BUTTONS3X3_HPP_

#include <Arduino.h>
#include <AceButton.h>

#include "constants.hpp"
#include "commands.hpp"

#ifdef BUTTONS3X3
class Buttons3x3: public CommandSource, public ace_button::IEventHandler {
public:
  Buttons3x3();
  commandRaw getCommandRaw() override;

private:
  static constexpr uint8_t releasedState = (buttonPinType == levelType::activeLow) ? HIGH : LOW;
  static constexpr uint8_t numButtons    = 9;
  static constexpr uint8_t numLevels     = numButtons + 1;

  ace_button::AceButton buttons[numButtons]{
    ace_button::AceButton{nullptr, 0, releasedState, 1},
    ace_button::AceButton{nullptr, 1, releasedState, 2},
    ace_button::AceButton{nullptr, 2, releasedState, 3},
    ace_button::AceButton{nullptr, 3, releasedState, 4},
    ace_button::AceButton{nullptr, 4, releasedState, 5},
    ace_button::AceButton{nullptr, 5, releasedState, 6},
    ace_button::AceButton{nullptr, 6, releasedState, 7},
    ace_button::AceButton{nullptr, 7, releasedState, 8},
    ace_button::AceButton{nullptr, 8, releasedState, 9}
  };
  ace_button::AceButton* const BUTTONS[numButtons] = { &buttons[0]
                                                     , &buttons[1]
                                                     , &buttons[2]
                                                     , &buttons[3]
                                                     , &buttons[4]
                                                     , &buttons[5]
                                                     , &buttons[6]
                                                     , &buttons[7]
                                                     , &buttons[8] };
  const uint16_t LEVELS[numLevels] = {   0
                                     ,  77
                                     , 148
                                     , 234
                                     , 306
                                     , 390
                                     , 465
                                     , 534
                                     , 593
                                     , 784 };

  ace_button::LadderButtonConfig buttonConfig{button3x3Pin, numLevels, LEVELS, numButtons, BUTTONS};

  void handleEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t buttonState) final;

  uint8_t lastButton{0};
};
#endif /* BUTTONS3X3 */
#endif /* SRC_BUTTONS3X3_HPP_ */

#ifndef SRC_BUTTONS_HPP_
#define SRC_BUTTONS_HPP_

#include <Arduino.h>
#include <AceButton.h>
#include <ace_button/fast/ButtonConfigFast3.h>

#include "commands.hpp"
#include "constants.hpp"

class Buttons: public CommandSource, public ace_button::IEventHandler {
public:
  Buttons();

  commandRaw getCommandRaw() override;
  bool isReset();

private:

  bool isNoButton();
  void readButtons();

  static constexpr uint8_t releasedState = (buttonPinType == levelType::activeLow) ? HIGH : LOW;
  ace_button::AceButton buttonPause{ buttonPausePin, releasedState };
  ace_button::AceButton buttonUp   { buttonUpPin   , releasedState };
  ace_button::AceButton buttonDown { buttonDownPin , releasedState };
#ifdef FIVEBUTTONS
  ace_button::AceButton buttonFour { buttonFourPin , releasedState };
  ace_button::AceButton buttonFive { buttonFivePin , releasedState };
#endif

  void handleEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t buttonState) final;

  enum class buttonState: uint8_t {
    none       ,
    released   ,
    pressedLong,
  };

  buttonState stateButtonPause{buttonState::none};
  buttonState stateButtonUp   {buttonState::none};
  buttonState stateButtonDown {buttonState::none};
#ifdef FIVEBUTTONS
  buttonState stateButtonFour {buttonState::none};
  buttonState stateButtonFive {buttonState::none};
#endif

  bool ignoreAll         = false;

};

#endif /* SRC_BUTTONS_HPP_ */

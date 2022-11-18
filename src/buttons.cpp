#include "buttons.hpp"

#include "constants.hpp"
#include "logger.hpp"

namespace {
constexpr bool buttonPinIsActiveLow = (buttonPinType == levelType::activeLow);
}

Buttons::Buttons()
: CommandSource()
{
  if constexpr (buttonPinIsActiveLow) {
    pinMode(buttonPausePin, INPUT_PULLUP);
    pinMode(buttonUpPin   , INPUT_PULLUP);
    pinMode(buttonDownPin , INPUT_PULLUP);
#ifdef FIVEBUTTONS
    pinMode(buttonFourPin , INPUT_PULLUP);
    pinMode(buttonFivePin , INPUT_PULLUP);
#endif
  }

  ace_button::ButtonConfig* buttonConfig = ace_button::ButtonConfig::getSystemButtonConfig();
  buttonConfig->setIEventHandler(this);
  buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureSuppressAfterLongPress);
}

void Buttons::handleEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t /*buttonState*/) {
  const buttonState bs = (eventType == ace_button::AceButton::kEventReleased   ) ? buttonState::released    :
                         (eventType == ace_button::AceButton::kEventLongPressed) ? buttonState::pressedLong :
                                                                                   buttonState::none        ;
  switch (button->getPin()) {
  case buttonPausePin: stateButtonPause = bs; break;
  case buttonUpPin   : stateButtonUp    = bs; break;
  case buttonDownPin : stateButtonDown  = bs; break;
#ifdef FIVEBUTTONS
  case buttonFourPin : stateButtonFour  = bs; break;
  case buttonFivePin : stateButtonFive  = bs; break;
#endif
  }

}

commandRaw Buttons::getCommandRaw() {
  commandRaw ret = commandRaw::none;
  readButtons();

  if (ignoreAll) {
    if (isNoButton()) {
      ignoreAll = false;
      stateButtonPause = buttonState::none;
      stateButtonUp    = buttonState::none;
      stateButtonDown  = buttonState::none;
    }
    return commandRaw::none;
  }

  if ((  stateButtonPause == buttonState::pressedLong
      || stateButtonUp    == buttonState::pressedLong
      || stateButtonDown  == buttonState::pressedLong)
      && isReset()
     ) {
    ret = commandRaw::allLong;
    ignoreAll = true;
    stateButtonPause = buttonState::none;
    stateButtonUp    = buttonState::none;
    stateButtonDown  = buttonState::none;
  }

  else if (stateButtonPause == buttonState::released) {
    ret = commandRaw::pause;
    stateButtonPause = buttonState::none;
  }

  else if (stateButtonPause == buttonState::pressedLong) {
    ret = commandRaw::pauseLong;
    stateButtonPause = buttonState::none;
  }

  else if (stateButtonUp == buttonState::released) {
    ret = commandRaw::up;
    stateButtonUp = buttonState::none;
  }

  else if (stateButtonUp == buttonState::pressedLong) {
    ret = commandRaw::upLong;
    stateButtonUp = buttonState::none;
  }

  else if (stateButtonDown == buttonState::released) {
    ret = commandRaw::down;
    stateButtonDown = buttonState::none;
  }

  else if (stateButtonDown == buttonState::pressedLong) {
    ret = commandRaw::downLong;
    stateButtonDown = buttonState::none;
  }

#ifdef FIVEBUTTONS
  else if (stateButtonFour == buttonState::released) {
    ret = commandRaw::four;
    stateButtonFour = buttonState::none;
  }

  else if (stateButtonFive == buttonState::released) {
    ret = commandRaw::five;
    stateButtonFive = buttonState::none;
  }
#endif

  if (ret != commandRaw::none) {
    LOG(button_log, s_debug, F("Button raw: "), static_cast<uint8_t>(ret));
  }
  return ret;
}

bool Buttons::isNoButton() {
  return not buttonPause.isPressedRaw()
      && not buttonUp   .isPressedRaw()
      && not buttonDown .isPressedRaw();
}

bool Buttons::isReset() {
  return buttonPause.isPressedRaw()
      && buttonUp   .isPressedRaw()
      && buttonDown .isPressedRaw();
}

void Buttons::readButtons() {
  buttonPause.check();
  buttonUp   .check();
  buttonDown .check();
#ifdef FIVEBUTTONS
  buttonFour .check();
  buttonFive .check();
#endif
}

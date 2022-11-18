#include "buttons3x3.hpp"

#include "constants.hpp"
#include "logger.hpp"
#ifdef BUTTONS3X3

//#define CALIBRATE3X3

Buttons3x3::Buttons3x3()
: CommandSource()
{
  pinMode(button3x3Pin, INPUT);
  buttonConfig.setIEventHandler(this);
  buttonConfig.setFeature(ace_button::ButtonConfig::kFeatureLongPress);
  buttonConfig.setFeature(ace_button::ButtonConfig::kFeatureSuppressAfterLongPress);
}

commandRaw Buttons3x3::getCommandRaw() {
  commandRaw ret = commandRaw::none;

#ifdef CALIBRATE3X3
  static uint8_t t = 0;
  if (++t % 20 == 0)
    LOG(button_log, s_info, F("Button3x3 analog value: "), static_cast<int>(analogRead(button3x3Pin)));
#else
  buttonConfig.checkButtons();

  const uint8_t button = lastButton;

  if (button > 0 && button <= buttonExtSC_buttons) {
    ret = static_cast<commandRaw>(static_cast<uint8_t>(commandRaw::ext_begin) + button - 1);
  }

  if (ret != commandRaw::none) {
    LOG(button_log, s_debug, F("Button3x3 raw: "), static_cast<uint8_t>(ret));
  }
  lastButton = 0;
#endif
  return ret;
}

void Buttons3x3::handleEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t /*buttonState*/) {
  switch (eventType) {
  case ace_button::AceButton::kEventReleased   : lastButton = button->getId()           ; break;
  case ace_button::AceButton::kEventLongPressed: lastButton = button->getId()+numButtons; break;
  }
}


#endif // BUTTONS3X3

#ifndef BASSBOYCC_HARDWARE_H_
#define BASSBOYCC_HARDWARE_H_

#include "avrlib/gpio.h"
#include "avrlib/serial.h"

/*****************************************************************************
 * hardware config
 ****************************************************************************/

// Debug output
typedef avrlib::SerialPort0 DebugPort;

// MIDI
typedef avrlib::SerialPort1 MidiPort;


// Buttons
typedef avrlib::Gpio<avrlib::PortE, 4> ToggleSwitchGpio;
typedef avrlib::Gpio<avrlib::PortB, 7> DebugLedGpio;

//

#endif BASSBOYCC_HARDWARE_H_

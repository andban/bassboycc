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
typedef avrlib::SerialPort0 MidiPort;

// Buttons
typedef avrlib::Gpio<avrlib::PortD, 3> ToggleSwitchGpio;
typedef avrlib::Gpio<avrlib::PortB, 5> DebugLedGpio;



#endif BASSBOYCC_HARDWARE_H_

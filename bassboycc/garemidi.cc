#include "avrlib/time.h"
#include "avrlib/boot.h"
#include "avrlib/avrlib.h"
#include "avrlib/software_serial.h"
#include "avrlib/debug_output.h"
#include "avrlib/watchdog_timer.h"

#include <avr/io.h>
#include <string.h>

#include "app.h"
#include "hardware.h"
#include "midihandler.h"
#include "ui.h"


using namespace avrlib;
using namespace bassboycc;
using namespace midi;

MidiStreamParser<MidiHandler> midiParser;
Serial<MidiPort, 31250, POLLED, POLLED> midiChannel;

#ifndef NDEBUG
DebugOutput< Serial<DebugPort, 9600, DISABLED, POLLED> > dbg;
#endif // !NDEBUG

void readMidiIn()
{
    if (midiChannel.readable())
    {
        uint8_t byte = midiChannel.ImmediateRead();
        if (byte != 0xfe)
        {
            midiParser.PushByte(byte);
        }
    }
}

void writeMidiOut()
{
    if (MidiHandler::OutputBuffer::readable() && midiChannel.writable())
    {
        uint8_t byte = MidiHandler::OutputBuffer::ImmediateRead();
#ifdef NDEBUG
        midiChannel.Overwrite(byte);
#endif // NDEBUG
    }
}

ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
{
    // 4.9 kHz
    readMidiIn();
    writeMidiOut();
    static uint8_t subClock = 0;
    ++subClock;
    if ((subClock & 1) == 0)
    {
        // 2.45 kHz
        ui.read();
        if ((subClock & 3) == 0)
        {
            // 1.225 kHz
            TickSystemClock();
        }
    }
}

void init()
{
    sei();

#ifndef NDEBUG
    dbg.Init();
#else // NDEBUG
    midiChannel.Init();
#endif // NDEBUG

    Timer<0>::set_prescaler(1);
    Timer<0>::set_mode(TIMER_PWM_PHASE_CORRECT);

    Timer<1>::set_prescaler(1);
    Timer<1>::set_mode(TIMER_PWM_PHASE_CORRECT);

    Timer<2>::set_prescaler(2);
    Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);

    app.init();
    ui.init();

    Timer<2>::Start();
}

int main(void)
{
    ResetWatchdog();
    init();

    while (1)
    {
        ui.update();
    }

    return 0;
}

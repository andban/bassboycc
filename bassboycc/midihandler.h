#ifndef BASSBOYCC_MIDIHANDLER_H_
#define BASSBOYCC_MIDIHANDLER_H_

#include "midi/midi.h"
#include "avrlib/ring_buffer.h"


namespace bassboycc
{

struct MidiHandler : public midi::MidiDevice
{
        enum
        {
                buffer_size = 128,
                data_size = 8
        };

        typedef avrlib::RingBuffer<MidiHandler> OutputBuffer;
        typedef avrlib::DataTypeForSize<data_size>::Type Value;

        static void NoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
        {
                app.onNoteOn(channel, note, velocity);
        }

        static void NoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
        {
                app.onNoteOff(channel, note, velocity);
        }

        static void Aftertouch(uint8_t channel, uint8_t note, uint8_t velocity)
        {
                app.onAftertouch(channel, note, velocity);
        }

        static void Aftertouch(uint8_t channel, uint8_t note)
        {
                app.onAftertouch(channel, note);
        }

        static void ControlChange(uint8_t channel, uint8_t controller, uint8_t value)
        {
                app.onControlChange(channel, controller, value);
        }

        static void ProgramChange(uint8_t channel, uint8_t program)
        {
                app.onProgramChange(channel, program);
        }

        static void PitchBend(uint8_t channel, uint16_t pitchBend)
        {
                app.onPitchBend(channel, pitchBend);
        }

        static void SysExStart() { }
        static void SysExByte(uint8_t sysexByte) { }
        static void SysExEnd() { }

        static void BozoByte(uint8_t bozoByte) { }

        static void Clock() { }
        static void Start() { }
        static void Continue() { }
        static void Stop()
        {
                app.onStop();
        }

        static void ActiveSensing() { }
        static void Reset() { }

        static uint8_t CheckChannel(uint8_t channel)
        {
                return app.onCheckChannel(channel);
        }

        static void RawByte(uint8_t byte)
        {
                app.onRawByte(byte);
        }

        static void RawMidiData(uint8_t status, uint8_t* data, uint8_t size, uint8_t channel)
        {
                app.onRawMidiData(status, data, size, channel);
        }
};

}
#endif // !BASSBOYCC_MIDIHANDLER_H_

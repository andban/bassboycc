#include "app.h"

#include "avrlib/serial.h"

#include <stdio.h>
#include <stdlib.h>

#include "hardware.h"
#include "midihandler.h"
#include "ui.h"

static const uint8_t OUT_CHANNEL = 0;


namespace bassboycc
{

avrlib::Serial<MidiPort, 31250, avrlib::DISABLED, avrlib::POLLED> midiChannel;

// static
uint8_t App::m_channel = 0;
uint8_t App::m_preFilterGain = 0;
uint8_t App::m_cutoff = 0;
uint8_t App::m_cutoffStep = 30;
uint8_t App::m_resonance = 0;
uint8_t App::m_envelopeDecay = 0;
uint8_t App::m_envelopeMod = 0;
uint8_t App::m_accent = 0;
uint8_t App::m_distortion = 4;
uint8_t App::m_volume = 127;
BassBoyWave App::m_waveForm = BB_WAVE_SAW;

void App::init()
{
}

void App::onPotRotate(uint8_t id, int8_t value)
{
        switch (id)
        {
        case EVT_POT_1:
                setCutOff(value);
                break;
        case EVT_POT_2:
                setResonance(value);
                break;
        case EVT_POT_3:
                setAccent(value);
                break;
        case EVT_POT_4:
                setEnvelopeDecay(value);
                break;
        case EVT_POT_5:
                setEnvelopeMod(value);
                break;
        case EVT_POT_6:
                //setCutOffStep(value);
                setPreFilterGain(value);
                break;
        case EVT_POT_7:
                setDistortion(value);
                break;
        case EVT_POT_8:
                setVolume(value);
                break;
        default:
                break;
        }
}

void App::onSwitchToggled(uint8_t id, uint8_t value)
{
        switch (id)
        {
        case EVT_TOGGLE_1:
                setWaveForm(value ? BB_WAVE_SQUARE : BB_WAVE_SAW);
                break;
        default:
                break;
        }
}

void App::onIdle()
{

}


void App::sendCC(uint8_t parameter, uint8_t value)
{
        midiSend3(0xB0 | OUT_CHANNEL, parameter, value);
}

void App::sendPC(uint8_t program)
{
        midiSend2(0xC0 | OUT_CHANNEL, program);
}

void App::setPreFilterGain(uint8_t value)
{
        if (m_preFilterGain == value)
        {
                return;
        }

        m_preFilterGain = value;
        sendCC(53, value);
}

void App::setCutOff(uint8_t value)
{
        if (m_cutoff == value)
        {
                return;
        }

        m_cutoff = value;
        sendCC(54, value);
}

void App::setCutOffStep(uint8_t value)
{
        if (m_cutoffStep == value)
        {
                return;
        }

        m_cutoffStep = value;
        sendCC(60, value);
}

void App::setResonance(uint8_t value)
{
        if (m_resonance == value)
        {
                return;
        }

        m_resonance = value;
        sendCC(55, value);
}

void App::setEnvelopeDecay(uint8_t value)
{
        if (m_envelopeDecay == value)
        {
                return;
        }

        m_envelopeDecay = value;
        sendCC(57, value);
}

void App::setEnvelopeMod(uint8_t value)
{
        if (m_envelopeMod == value)
        {
                return;
        }

        m_envelopeMod = value;
        sendCC(56, value);
}

void App::setAccent(uint8_t value)
{
        if (m_accent == value)
        {
                return;
        }

        m_accent = value;
        sendCC(58, value);
}

void App::setDistortion(uint8_t value)
{
        if (m_distortion == value)
        {
                return;
        }

        m_distortion = value;
        sendCC(59, value);
}

void App::setVolume(uint8_t value)
{
        if (m_volume == value)
        {
                return;
        }

        m_volume = value;
        sendCC(7, value);
}

void App::setWaveForm(BassBoyWave waveForm)
{
        if (m_waveForm == waveForm)
        {
                return;
        }

        m_waveForm = waveForm;
        sendPC(waveForm);
}


void App::midiSend(uint8_t status, uint8_t* data, uint8_t size)
{
        flushOutputBuffer(size + 1);
        MidiHandler::OutputBuffer::Write(status); // inject out channel here?
        if (size)
        {
                MidiHandler::OutputBuffer::Write(*data++);
                --size;
        }
        if (size)
        {
                MidiHandler::OutputBuffer::Write(*data++);
                --size;
        }
}

void App::midiSend2(uint8_t p1, uint8_t p2)
{
        flushOutputBuffer(3);
        MidiHandler::OutputBuffer::Write(p1);
        MidiHandler::OutputBuffer::Write(p2);
}

void App::midiSend3(uint8_t p1, uint8_t p2, uint8_t p3)
{
        flushOutputBuffer(3);
        MidiHandler::OutputBuffer::Write(p1);
        MidiHandler::OutputBuffer::Write(p2);
        MidiHandler::OutputBuffer::Write(p3);

}

void App::flushOutputBuffer(uint8_t n)
{
        while (MidiHandler::OutputBuffer::writable() < n)
        {
                uint8_t byte = MidiHandler::OutputBuffer::Read();
#ifdef NDEBUG
                midiChannel.Write(byte);
#endif // NDEBUG
        }
}

void App::onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
        if (m_channel != channel)
        {
                return;
        }

        midiSend3(0x90 | OUT_CHANNEL, note, velocity);
}

void App::onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
        if (m_channel != channel)
        {
                return;
        }

        midiSend3(0x80 | OUT_CHANNEL, note, velocity);
}

void App::onAftertouch(uint8_t channel, uint8_t note, uint8_t velocity)
{
        if (m_channel != channel)
        {
                return;
        }
}

void App::onAftertouch(uint8_t channel, uint8_t velocity)
{
        if (m_channel != channel)
        {
                return;
        }
}

void App::onControlChange(uint8_t channel, uint8_t controller, uint8_t value)
{
        if (m_channel != channel)
        {
                return;
        }

        //midiSend3(0x90 | OUT_CHANNEL, note, velocity);
}

void App::onProgramChange(uint8_t channel, uint8_t program)
{
        if (m_channel != channel)
        {
                return;
        }

        //midiSend3(0x90 | OUT_CHANNEL, note, velocity);
}

void App::onPitchBend(uint8_t channel, uint16_t pitch)
{
        if (m_channel != channel)
        {
                return;
        }

        //midiSend3(0x90 | OUT_CHANNEL, note, velocity);
}

void App::onStop()
{
        sendCC(123, 0x7F);
}

uint8_t App::onCheckChannel(uint8_t channel)
{
        return 1;
}

void App::onRawByte(uint8_t byte)
{
        //flushOutputBuffer(1);
        //MidiHandler::OutputBuffer::Write(byte);
}

void App::onRawMidiData(uint8_t status, uint8_t* data, uint8_t size, uint8_t channel)
{
        if (m_channel != channel)
        {
                return;
        }
        midiSend(status, data, size);
}


}

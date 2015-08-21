#ifndef BASSBOYCC_APP_H_
#define BASSBOYCC_APP_H_

#include "avrlib/base.h"

namespace bassboycc
{

enum BassBoyWave {
        BB_WAVE_SAW = 1,
        BB_WAVE_SQUARE = 2
};

class App
{
public:
        App() { }

        static void init();

        // ui callbacks
        static void onLoad() { };
        static void onPotRotate(uint8_t id, int8_t value);
        static void onSwitchToggled(uint8_t id, uint8_t value);
        static void onIdle();

        // bassboy parameters
        static void setPreFilterGain(uint8_t value);
        static void setCutOff(uint8_t value);
        static void setCutOffStep(uint8_t value);
        static void setResonance(uint8_t value);
        static void setEnvelopeDecay(uint8_t value);
        static void setEnvelopeMod(uint8_t value);
        static void setAccent(uint8_t value);
        static void setDistortion(uint8_t value);
        static void setVolume(uint8_t value);
        static void setWaveForm(BassBoyWave waveForm);

        // midi send
        static void midiSend(uint8_t status, uint8_t* data, uint8_t size);
        static void midiSend2(uint8_t p1, uint8_t p2);
        static void midiSend3(uint8_t p1, uint8_t p2, uint8_t p3);
        static void flushOutputBuffer(uint8_t n);

        // midi callbacks
        static void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
        static void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        static void onAftertouch(uint8_t channel, uint8_t note, uint8_t velocity);
        static void onAftertouch(uint8_t channel, uint8_t velocity);
        static void onControlChange(uint8_t channel, uint8_t controller, uint8_t value);
        static void onProgramChange(uint8_t channel, uint8_t program);
        static void onPitchBend(uint8_t channel, uint16_t pitch);
        static void onStop();
        static uint8_t onCheckChannel(uint8_t channel);
        static void onRawByte(uint8_t byte);
        static void onRawMidiData(uint8_t status, uint8_t* data, uint8_t size, uint8_t channel);

private:
        static void sendCC(uint8_t parameter, uint8_t value);
        static void sendPC(uint8_t program);

        static uint8_t m_channel;
        static uint8_t m_preFilterGain;
        static uint8_t m_cutoff;
        static uint8_t m_cutoffStep;
        static uint8_t m_resonance;
        static uint8_t m_envelopeDecay;
        static uint8_t m_envelopeMod;
        static uint8_t m_accent;
        static uint8_t m_distortion;
        static uint8_t m_volume;
        static BassBoyWave m_waveForm;
};

extern App app;

} // namespace bassboycc

#endif // !BASSBOYCC_APP_H_


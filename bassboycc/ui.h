#ifndef BASSBOYCC_UI_H_
#define BASSBOYCC_UI_H_

#include <string.h>
#include <stdio.h>

#include "avrlib/devices/pot_scanner.h"
#include "avrlib/devices/switch.h"
#include "avrlib/ui/event_queue.h"

#include "bassboycc/hardware.h"


namespace bassboycc
{

enum PotIndex
{
        POT_1,
        POT_2,
        POT_3,
        POT_4,
        // POT_5,
        // POT_6,
        // POT_7,
        // POT_8,

        POT_COUNT
};

enum ToggleIndex
{
        TOGGLE_1,

        TOGGLE_COUNT
};

// move somewhere else...
enum EventId {
        EVT_POT_1,
        EVT_POT_2,
        EVT_POT_3,
        EVT_POT_4,
        EVT_POT_5,
        EVT_POT_6,
        EVT_POT_7,
        EVT_POT_8,

        EVT_TOGGLE_1,

        EVT_COUNT
};


typedef avrlib::PotScanner<POT_COUNT, 0, 8, 7> ParameterPots;
typedef avrlib::DebouncedSwitch<ToggleSwitchGpio, false> ToggleSwitch;
/**
 *
 */
class UI {
public:
        /**
         *
         */
        UI() { }

        /**
         * Initialize user interface.
         */
        static void init();

        /**
         * Read inputs data.
         */
        static void read();

        /**
         * Process input events.
         */
        static void update();

private:
        static void handleEvent(const avrlib::Event& event);
        static void handleToggleEvent(const avrlib::Event& event);
        static void handlePotEvent(const avrlib::Event& event);

        static ParameterPots          m_pots;
        static ToggleSwitch           m_toggleSwitch;
        static avrlib::EventQueue<16> m_events;

        static uint8_t                m_potWarmupCycles;
        static uint8_t                m_potValues[POT_COUNT];
        static uint8_t                m_toggleValue;
        static uint8_t                m_cycle;
};

extern UI ui;

}

#endif // !BASSBOYCC_UI_H_

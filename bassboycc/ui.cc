#include "ui.h"
#include "app.h"
#include "memdbg.h"

namespace bassboycc
{

// static members
ParameterPots UI::m_pots;
ToggleSwitch UI::m_toggleSwitch;
avrlib::EventQueue<16> UI::m_events;
uint8_t UI::m_potWarmupCycles = POT_COUNT * 8;
uint8_t UI::m_cycle = 0;

// external variables
UI ui;

void UI::init()
{
        m_pots.Init();
}

void UI::read()
{
        ++m_cycle;

        // poll wave form toggle switch
        if ((m_cycle & 3) == 0)
        {
                uint8_t value = m_toggleSwitch.Read();
                m_events.AddEvent(avrlib::CONTROL_SWITCH, EVT_TOGGLE_1, value);
        }

        // poll parameter pots
        m_pots.Read();
        uint8_t potIndex = m_pots.last_read();
        uint8_t potValue = m_pots.value(potIndex);
        if (!m_potWarmupCycles)
        {
                m_events.AddEvent(avrlib::CONTROL_POT, EVT_POT_1 + potIndex, potValue);
        }

        if (m_potWarmupCycles)
        {
                --m_potWarmupCycles;
        }
}

void UI::update()
{
        while (m_events.available())
        {
                avrlib::Event event = m_events.PullEvent();
                handleEvent(event);
                m_events.Touch();
        }
        m_events.Flush();
}

void UI::handleEvent(const avrlib::Event& event)
{
        switch (event.control_type)
        {
        case avrlib::CONTROL_POT:
                app.onPotRotate(event.control_id, event.value);
                break;
        case avrlib::CONTROL_SWITCH:
                app.onSwitchToggled(event.control_id, event.value);
                break;
        default:
                break;
        }
}

} // namespace bassboycc

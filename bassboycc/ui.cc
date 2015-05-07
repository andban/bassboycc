#include "ui.h"
#include "app.h"
#include "memdbg.h"

namespace bassboycc
{

// static members
ParameterPots UI::m_pots;
ToggleSwitch UI::m_toggleSwitch;
avrlib::EventQueue<16> UI::m_events;
uint8_t UI::m_potWarmupCycles = 16;
uint8_t UI::m_potValues[POT_COUNT];
uint8_t UI::m_toggleValue;
uint8_t UI::m_cycle;

// external variables
UI ui;

void UI::init()
{
        m_pots.Init();
        avrlib::Adc::set_reference(avrlib::ADC_DEFAULT);
        m_toggleSwitch.Init();
}

void UI::read()
{
        ++m_cycle;

        // poll wave form toggle switch
        if ((m_cycle & 3) == 0)
        {
                m_toggleSwitch.Read();
                uint8_t value = m_toggleSwitch.immediate_value();
                if (value != m_toggleValue)
                {
                        m_toggleValue = value;
                        m_events.AddEvent(avrlib::CONTROL_SWITCH, EVT_TOGGLE_1, value);
                }
        }

        // poll parameter pots
        m_pots.Read();
        uint8_t potIndex = m_pots.last_read();
        uint8_t potValue = m_pots.value(potIndex);
        if (potValue != m_potValues[potIndex])
        {
                m_potValues[potIndex] = potValue;
                if (!m_potWarmupCycles)
                {
                        m_events.AddEvent(avrlib::CONTROL_POT, EVT_POT_1 + potIndex, potValue);
                }
                else if (potIndex == POT_COUNT - 1)
                {
                        --m_potWarmupCycles;
                }
        }
}

void UI::update()
{
        while (m_events.available())
        {
                m_events.Touch();
                avrlib::Event event = m_events.PullEvent();
                handleEvent(event);
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

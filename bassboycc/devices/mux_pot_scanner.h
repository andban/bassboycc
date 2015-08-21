#ifndef BASSBOYCC_MUX_POTSCANNER_H
#define BASSBOYCC_MUX_POTSCANNER_H

#include "avrlib/adc.h"
#include "avrlib/log2.h"
#include "avrlib/devices/mux4051.h"

namespace bassboycc
{

template<typename Port,
        uint8_t adc_pin,
        uint8_t num_inputs,
        uint8_t oversampling = 8,
        uint8_t resolution = 7>
class MuxPotScanner {
public:
        MuxPotScanner() { }

        static inline void Init()
        {
                Port::Init();

                avrlib::Adc::Init();
                avrlib::Adc::set_alignment(avrlib::ADC_LEFT_ALIGNED);
                scan_cycle_ = 0;
                avrlib::Adc::StartConversion(adc_pin);
                history_ptr_ = 0;
                memset(history_, 0, sizeof(history_));
                memset(value_, 0, sizeof(value_));
        }

        static inline void Read() {
                uint8_t index = history_ptr_ + oversampling * scan_cycle_;
                value_[scan_cycle_] -= history_[index];
                avrlib::Adc::Wait();
                history_[index] = avrlib::Adc::ReadOut8();
                value_[scan_cycle_] += history_[index];

                ++scan_cycle_;
                if (scan_cycle_ == num_inputs) {
                        scan_cycle_ = 0;
                        ++history_ptr_;
                        if (history_ptr_ == oversampling) {
                                history_ptr_ = 0;
                        }
                }

                avrlib::Adc::StartConversion(adc_pin);
                Port::Write(scan_cycle_);
        }

        static inline uint16_t value(uint8_t index) {
                uint16_t shift = (avrlib::Log2<oversampling>::value + 8 - resolution);
                return value_[index] >> shift;
        }

        static inline uint8_t last_read() {
                if (scan_cycle_ == 0) {
                        return num_inputs - 1;
                } else {
                        return (scan_cycle_ - 1);
                }
        }

private:
        static uint8_t scan_cycle_;
        static uint8_t history_[num_inputs * oversampling];
        static uint16_t value_[num_inputs];
        static uint8_t history_ptr_;

        DISALLOW_COPY_AND_ASSIGN(MuxPotScanner);
};

/* static */
template<typename Port, uint8_t a, uint8_t b, uint8_t c, uint8_t d>
uint8_t MuxPotScanner<Port, a, b, c, d>::scan_cycle_;

/* static */
template<typename Port, uint8_t a, uint8_t b, uint8_t c, uint8_t d>
uint8_t MuxPotScanner<Port, a, b, c, d>::history_ptr_;

/* static */
template<typename Port, uint8_t a, uint8_t num_inputs, uint8_t c, uint8_t d>
uint16_t MuxPotScanner<Port, a, num_inputs, c, d>::value_[num_inputs];

/* static */
template<typename Port, uint8_t a, uint8_t num_inputs, uint8_t oversampling, uint8_t d>
uint8_t MuxPotScanner<Port, a, num_inputs, oversampling, d>::history_[
    num_inputs * oversampling];

}

template<typename Port,
        uint8_t adc_pin,
        uint8_t num_inputs,
        uint8_t threshold = 4,
        uint8_t resolution = 10>
class MuxHysteresisPotScanner {
public:
    MuxHysteresisPotScanner() { }

    static inline void Init()
    {
        Port::Init();

        avrlib::Adc::Init();
        scan_cycle_ = 0;
        avrlib::Adc::StartConversion(adc_pin);

        Lock(threshold);
    }

    static void Lock(uint16_t locked_threshold)
    {
        for (uint8_t i = 0; i < num_inputs; ++i)
        {
            thresholds_[i] = locked_threshold;
        }
    }

    static inline void Read()
    {
        avrlib::Adc::Wait();

        uint16_t value = avrlib::Adc::ReadOut();
        int16_t delta = static_cast<int16_t>(value_[scan_cycle_]) -
                static_cast<int16_t>(value);

        if (delta < 0)
        {
            delta = -delta;
        }

        if (delta >= thresholds_[scan_cycle_])
        {
            thresholds_[scan_cycle_] = threshold;
            value_[scan_cycle_] = value;
        }

        ++scan_cycle_;
        if (scan_cycle_ == num_inputs)
        {
            scan_cycle_ = 0;
        }
        avrlib::Adc::StartConversion(adc_pin);
        Port::Write(scan_cycle_);
    }

    static inline uint16_t value(uint8_t index)
    {
        uint16_t shift =  (10 - resolution);
        return value_[index] >> shift;
    }

    static inline uint8_t last_read()
    {
        if (scan_cycle_ == 0) {
            return num_inputs - 1;
        } else {
            return (scan_cycle_ - 1);
        }
    }

private:
    static uint8_t scan_cycle_;
    static uint16_t value_[num_inputs];
    static uint16_t thresholds_[num_inputs];

    DISALLOW_COPY_AND_ASSIGN(MuxHysteresisPotScanner);
};

/* static */
template<typename Port, uint8_t a, uint8_t b, uint8_t c, uint8_t d>
uint8_t MuxHysteresisPotScanner<Port, a, b, c, d>::scan_cycle_;

/* static */
template<typename Port, uint8_t a, uint8_t num_inputs, uint8_t c, uint8_t d>
uint16_t MuxHysteresisPotScanner<Port, a, num_inputs, c, d>::value_[num_inputs];

/* static */
template<typename Port, uint8_t a, uint8_t num_inputs, uint8_t c, uint8_t d>
uint16_t MuxHysteresisPotScanner<Port, a, num_inputs, c, d>::thresholds_[num_inputs];

#endif

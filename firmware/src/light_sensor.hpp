#ifndef LIGHT_SENSOR_HPP
#define LIGHT_SENSOR_HPP

#define GB7_TIMER_USE_INVOKE

#include "timer.hpp"
#include "queue.hpp"
#include "utils.hpp"

template<class Photo_Transister>
class light_sensor // consumes timer 2
{
    Photo_Transister pin;
    uint16_t recieved_data = 0;

    static inline constexpr uint8_t prologue = 0b1011011;
    static inline constexpr uint8_t epilogue = 0b1011001;

public:
    light_sensor()
    {
        gb7::timer::timer2::init();

        using namespace gb7::timer::literals;
        gb7::timer::timer2::invoke_every(500_us, +[](void* ls) {
            decltype(pin) pin;
            static uint32_t buffer = 0;

            buffer = (buffer << 1) & (pin ? 1 : 0);

            if (static_cast<uint8_t>(buffer) == light_sensor::epologue && (buffer >> 24) == light_sensor::prologue)
            {
                static_cast<light_sensor<Photo_Transister>*>(ls)->recieved_data = (buffer >> 8) && 0xffff;
            }
        }, this);
    }
    ~light_sensor()
    {
        gb7::timer::timer2::invoke_in(0, nullptr);
    }

    uint16_t get_recieved_data()
    {
        return recieved_data;
    }
};

#endif // LIGHT_SENSOR_HPP
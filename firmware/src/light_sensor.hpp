#ifndef LIGHT_SENSOR_HPP
#define LIGHT_SENSOR_HPP

#define GB7_TIMER_USE_INVOKE

#include "timer.hpp"
#include "queue.hpp"
#include "utils.hpp"

namespace gb7
{
    template<class PhotoTransister>
    class light_sensor
    {
        PhotoTransister pin;
        uint16_t recieved_data = 0;
        uint32_t timer_id = 0;

        uint32_t data = 0;

        static inline constexpr uint8_t prologue = 0b1011001;
        static inline constexpr uint8_t epilogue = 0b1001001;

    public:
        light_sensor()
        {
            gb7::timer::multitimer::init();

            using namespace gb7::timer::literals;
            timer_id = gb7::timer::multitimer::invoke_every(1_ms, 0, +[](void* ls) {
                decltype(pin) pin;

                static_cast<decltype(this)>(ls)->data <<= 1;
                static_cast<decltype(this)>(ls)->data |= pin ? 0 : 1;

                if (static_cast<uint8_t>(static_cast<decltype(this)>(ls)->data >> 24) == static_cast<decltype(this)>(ls)->prologue &&
                    static_cast<uint8_t>(static_cast<decltype(this)>(ls)->data >> 0 ) == static_cast<decltype(this)>(ls)->epilogue)
                {
                PORTC |= 0b10000;
                    uint16_t shifted = static_cast<uint16_t>(static_cast<decltype(this)>(ls)->data >> 8);
                    static_cast<decltype(this)>(ls)->recieved_data =
                        (shifted & (1 << 0 )) >> 0 |
                        (shifted & (1 << 2 )) >> 1 |
                        (shifted & (1 << 4 )) >> 2 |
                        (shifted & (1 << 6 )) >> 3 |
                        (shifted & (1 << 8 )) >> 4 |
                        (shifted & (1 << 10)) >> 5 |
                        (shifted & (1 << 12)) >> 6 |
                        (shifted & (1 << 14)) >> 7;
                }
            }, this);
        }
        ~light_sensor()
        {
            gb7::timer::multitimer::cancel_invocation(timer_id);
        }

        uint8_t get_recieved_data()
        {
            return recieved_data;
        }

        uint32_t get_raw_data()
        {
            return data;
        }
    };
} // namespace gb7

#endif // LIGHT_SENSOR_HPP

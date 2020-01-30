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
        bool is_recieved = false;
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
                    static_cast<decltype(this)>(ls)->is_recieved = true;
                }
                else
                {
                    static_cast<decltype(this)>(ls)->is_recieved = false;
                }
            }, this);
        }
        ~light_sensor()
        {
            gb7::timer::multitimer::cancel_invocation(timer_id);
        }

        bool has_recieved_data() const
        {
            return is_recieved;
        }
        uint8_t get_recieved_data()
        {
            return recieved_data;
        }

        void pop_recieved_data()
        {
            is_recieved = false;
            recieved_data = 0;
        }
    };
} // namespace gb7

#endif // LIGHT_SENSOR_HPP

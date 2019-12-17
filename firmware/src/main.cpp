/* Name: main.cpp
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <stdlib.h>

#include "queue.hpp"
#include "timer.hpp"

#include "port.hpp"
#include "lcd_drawable.hpp"

int main()
{
    {
        gb7::port_writable<gb7::port_type::PortC> port_c;
        auto cs1 = port_c.get_writable_pin<0>();
        auto cs2 = port_c.get_writable_pin<1>();
        auto rs = port_c.get_writable_pin<2>();
        auto rw = port_c.get_writable_pin<3>();
        auto e = port_c.get_writable_pin<4>();
        auto rst = port_c.get_writable_pin<5>();

        gb7::lcd_drawable<decltype(rs), decltype(rw), gb7::port_dyamic<gb7::port_type::PortD>,
            decltype(cs1), decltype(cs2), decltype(e), decltype(rst)> graphic{};

        graphic.draw_ascii_string(0, 5, "\xcf\xb2\xba\xdd Atmega328P\n with TG12864B-02WWBV");
        graphic.update();
    }

    {
        static constexpr gb7::pin_io_config config[8] = 
        {
            gb7::pin_io_config::readable,
        };
        gb7::port_mixed<gb7::port_type::PortB, config> port_b;
        auto photo_sensor = port_b.get_readable_pin<0>();


        using namespace gb7::timer::literals;
        gb7::timer::multi_timer::evoke_every<10_ms>(+[](void*) {
            decltype(photo_sensor) photo_sensor;
            static gb7::queue<bool> buffer;

            if (photo_sensor)
            {
                
            }
        }, nullptr);
    }

    for (;;); // infinity loop
    return 0;   /* never reached */
}

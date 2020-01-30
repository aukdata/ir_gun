/* Name: main.cpp
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <stdlib.h>

#include "port.hpp"
#include "lcd_drawable.hpp"
#include "light_sensor.hpp"

int main()
{
    static constexpr gb7::pin_io_config port_b_config[8] = 
    {
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,

        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
    };
    static constexpr gb7::pin_io_config port_c_config[8] = 
    {
        gb7::pin_io_config::readable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::readable,
        gb7::pin_io_config::writable,

        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
    };
    gb7::port_mixed<gb7::port_type::PortB, port_b_config> port_b;
    gb7::port_mixed<gb7::port_type::PortC, port_c_config> port_c;
    auto cs1 = port_b.get_writable_pin<0>();
    auto cs2 = port_b.get_writable_pin<1>();
    auto rst = port_b.get_writable_pin<2>();
    auto rw  = port_b.get_writable_pin<6>();
    auto e   = port_b.get_writable_pin<7>();
    auto rs  = port_c.get_writable_pin<5>();

    auto pt  = port_c.get_readable_pin<2>();
    auto sw  = port_c.get_readable_pin<0>();
    auto sp  = port_c.get_writable_pin<1>();
    auto dg  = port_c.get_writable_pin<4>();

    gb7::light_sensor<decltype(pt)> ls;
    gb7::lcd_drawable<decltype(rs), decltype(rw), gb7::port_writable<gb7::port_type::PortD>,
        decltype(cs1), decltype(cs2), decltype(e), decltype(rst)> graphic{};

    _delay_ms(1000);

    graphic.draw_ascii_string(15, 0, "booting...");
    graphic.draw_circle(32, 64, 16);
    graphic.draw_ascii_string(7, 5, "Quick\n Shot");
    graphic.update();

    graphic.clear();
    graphic.draw_ascii_string(13, 1, "Pull trigger\n      to start");
    graphic.draw_circle(32, 64, 20);
    graphic.draw_ascii_string(7, 5, "Quick\n Shot");
    graphic.update();
    while (sw);

    for (int i = 0; i < 70; i++)
    {
        graphic.clear();
        graphic.draw_circle(32, 64, i + 20);
        graphic.update();
        _delay_ms(4);
    }

    for (;;)
    {
        graphic.clear();
        graphic.draw_ascii_string(7, 7, "3");
        graphic.draw_circle(32, 60, 30);
        graphic.update();
        _delay_ms(1000);

        graphic.clear();
        graphic.draw_ascii_string(7, 7, "2");
        graphic.draw_circle(32, 60, 20);
        graphic.update();
        _delay_ms(1000);

        graphic.clear();
        graphic.draw_ascii_string(7, 7, "1");
        graphic.draw_circle(32, 60, 10);
        graphic.update();
        _delay_ms(1000);

        graphic.clear();
        graphic.draw_ascii_string(7, 5, "START!");
        graphic.update();
        _delay_ms(1000);

        const uint32_t max_time = 60 * 20;
        uint32_t time = max_time;

        uint32_t score = 0;

        bool prev_sw = false;

        do
        {
            time--;
            
            graphic.clear();

            int point = ls.get_recieved_data();
            
            if (!sw && !prev_sw)
            {
                score += point;
                ls.pop_recieved_data();
            }
            prev_sw = !sw;


            for (int i = 0; i < 8; i++)
            {
                graphic.draw_line(0, i, 64 * time / max_time, i);
            }

            graphic.printf(2, 4, "TIME: %d", time / 20);
            graphic.printf(12, 5, "SCORE\n  %d", score);

            graphic.draw_circle(32, 64, 20);
            if (ls.has_recieved_data())
            {
                graphic.draw_circle(32, 64, 4);
            }

            if (!sw)
            {
                graphic.draw_line(32, 52, 32, 60);
                graphic.draw_line(32, 68, 32, 76);

                graphic.draw_line(20, 64, 28, 64);
                graphic.draw_line(36, 64, 44, 64);
            }
            else
            {
                graphic.draw_line(32, 48, 32, 56);
                graphic.draw_line(32, 72, 32, 80);

                graphic.draw_line(16, 64, 24, 64);
                graphic.draw_line(40, 64, 48, 64);
            }

            graphic.update();
            _delay_ms(50);
        } while (time != 0);

        graphic.clear();
        graphic.update();
        _delay_ms(500);

        graphic.clear();
        graphic.draw_ascii_string(7, 3, "TIME UP!");
        graphic.update();
        _delay_ms(1000);

        graphic.clear();
        graphic.update();
        _delay_ms(500);

        graphic.clear();
        graphic.draw_ascii_string(3, 2, "== Result ==");
        graphic.printf(7, 5, "SCORE\n  %d", score);
        graphic.draw_ascii_string(13, 1, "Pull triger\n      to retry");
        graphic.update();

        _delay_ms(50);
        while (sw);
    }

    for (;;); // infinity loop
    return 0;   /* never reached */
}

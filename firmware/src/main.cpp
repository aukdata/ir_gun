/* Name: main.cpp
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <stdlib.h>
#include "port.hpp"
#include "lcd.hpp"

int main()
{
    static constexpr gb7::pin_io_config config[8] = {
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
    };
    gb7::port_mixed<gb7::port_type::PortC, config> port_c;
    auto cs1 = port_c.get_writable_pin<0>();
    auto cs2 = port_c.get_writable_pin<1>();
    auto rs = port_c.get_writable_pin<2>();
    auto rw = port_c.get_writable_pin<3>();
    auto e = port_c.get_writable_pin<4>();
    auto reset = port_c.get_writable_pin<5>();

    gb7::lcd<decltype(rs), decltype(rw), gb7::port_dyamic<gb7::port_type::PortD>,
        decltype(cs1), decltype(cs2), decltype(e), decltype(reset)> graphic{};
    graphic.init();

    gb7::port_writable<gb7::port_type::PortB> port_b;
    auto pin_b0 = port_b.get_writable_pin<0>();
    for(int i=0;;i++)
    {
        pin_b0 = !pin_b0;

        graphic.clear();

        for (uint8_t j = 0; j< 158; j++)
        {
            graphic.draw_ascii_char(0, 0, j);
            graphic.draw_ascii_string(1, 0, "Quartz");

            static constexpr uint64_t str[] = {
                0x7f017555750180,
                0x3f4041402111,
                0x7f251f6a706f0b,
                0x403f1b775b771c,
                0x7f002052424241,
                0x304a7f2a1a4a31,
                0x10295557654539,
                0x445453d050406,
                0x10100878040201,
                0x7f4020201009,
                0x844432222120f,
                0x7f08081011,
                0xf4040201f01,
                0x0
            }; /* 同じ階層にあるディレクトリ */
            graphic.draw_jis_string(3, 0, str);

            graphic.update();
            _delay_ms(500);
        }

        _delay_ms(1000);
    }

    return 0;   /* never reached */
}

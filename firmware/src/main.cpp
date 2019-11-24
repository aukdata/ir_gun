/* Name: main.cpp
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <stdlib.h>
#include "port.hpp"
#include "lcd_drawable.hpp"

int main()
{
    static constexpr gb7::pin_io_config port_c_config[8] = {
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
        gb7::pin_io_config::writable,
    };
    gb7::port_mixed<gb7::port_type::PortC, port_c_config> port_c;
    auto cs1 = port_c.get_writable_pin<0>();
    auto cs2 = port_c.get_writable_pin<1>();
    auto rs = port_c.get_writable_pin<2>();
    auto rw = port_c.get_writable_pin<3>();
    auto e = port_c.get_writable_pin<4>();
    auto rst = port_c.get_writable_pin<5>();

    gb7::lcd_drawable<decltype(rs), decltype(rw), gb7::port_dyamic<gb7::port_type::PortD>,
        decltype(cs1), decltype(cs2), decltype(e), decltype(rst)> graphic{};

    gb7::port_writable<gb7::port_type::PortB> port_b;
    auto pin_b0 = port_b.get_writable_pin<0>();
    for(int i=0;;i++)
    {
        pin_b0 = !pin_b0;

        for (uint8_t j = 0; j < 7; j++)
        {
            graphic.clear();
            graphic.draw_ascii_string(0, 5, "\xcf\xb2\xba\xdd Atmega328P\n with TG12864B-02WWBV");

            static constexpr uint64_t str[] = {
                0x5257563e565753,
                0x1e125e553c5555,
                0x242422222120f,
                0x242414547e0401,
                0x1414220201009,
                0x7f08081011,
                0x0
            }; /* 美咲フォント */
            static constexpr uint64_t iroha[] = {
                0x23e555d575c61,
                0x7f2052527f2243,
                0x8043f2a2a427f,
                0x4425151d754541,
                0x20504f48454442,
                0xa7f2a7f5c235f,
                0xf4241211e01,
                0x38473c122f527d,
                0x10295557654539,
                0x120a6e5b724a49,
                0x0
            }; /* 色は匂えど散りぬるを */
            static constexpr uint64_t wagayo[] = {
                0x14567e155f2456,
                0x621a47423d000e,
                0x47f445f545f45,
                0x6a6b047f4a7f4b,
                0x8092d5b49480a,
                0x36131e7b1e1337,
                0x320a2752507e23,
                0x181549494a2811,
                0x322a7f42404233,
                0x0
            }; /* 我が世誰ぞ常ならむ */

            graphic.draw_zenkaku_string(3, j + 2, str);
            graphic.draw_zenkaku_string(5, 1, i % 2 == 0 ? iroha : wagayo);

            graphic.printf(7, 1, "iteration: %d", i);

            graphic.update();
            _delay_ms(500);
        }
    }

    return 0;   /* never reached */
}

#ifndef LCD_HPP
#define LCD_HPP

#include <util/delay.h>
#include <math.h>
#include <stdio.h>

#include "port.hpp"
#include "utils.hpp"
#include "../font/misaki_font.hpp"

namespace gb7
{
    /*
    template<PinWritable RS_Pin, PinWritable RW_Pin, PortWritable DataBus_Port,
        PinWritable CS1_Pin, PinWritable CS2_Pin, PinWritable E_Pin>
        */
    template<class RS_Pin, class RW_Pin, class DataBus_Port,
        class CS1_Pin, class CS2_Pin, class E_Pin, class RST_Pin>
    class lcd
    {
    private:
        RS_Pin m_rs;
        RW_Pin m_rw;
        DataBus_Port m_data_bus;
        CS1_Pin m_cs1;
        CS2_Pin m_cs2;
        E_Pin m_e;
        RST_Pin m_rst;

    public:
        lcd() noexcept = default;
        ~lcd() = default;

        inline void send_command(bool rs, bool rw, uint8_t data) noexcept
        {
            m_data_bus.write(data);
            m_rs = rs;
            m_rw = rw;
            m_e = true;
            _delay_us(5);

            m_e = false;
            _delay_us(5);
        }

        inline void set_on(bool d) noexcept
        {
            send_command(false, false, 0b00111110 | (d ? 1 : 0));
        }
        inline void set_column(uint8_t addr) noexcept
        {
            send_command(false, false, 0b01000000 | addr);
        }
        inline void set_page(uint8_t page) noexcept
        {
            send_command(false, false, 0b10111000 | page);
        }
        inline void set_start_line(uint8_t s) noexcept
        {
            send_command(false, false, 0b11000000 | s);
        }
        inline void set_data(uint8_t data) noexcept
        {
            send_command(true, false, data);
        }
        inline void set_chip(bool left, bool right) noexcept
        {
            m_cs1 = left;
            m_cs2 = right;
            _delay_us(0.14);
        }

        inline void init() noexcept
        {
            m_cs1 = true;
            m_cs2 = true;
            m_rst = true;
            _delay_ms(30);

            set_on(false);

            clear_immediataly();

            set_on(true);
        }

        inline void clear_immediataly() noexcept
        {
            set_chip(true, true);

            for (uint8_t chunk_y = 0; chunk_y < 8; chunk_y++)
            {
                set_page(chunk_y);
                for (uint8_t chunk_x = 0; chunk_x < 8; chunk_x++)
                {
                    for (uint8_t i = 0; i < 8; i++)
                    {
                        set_data(0);
                    }
                }
            }
        }

        inline void set_pixel_immediataly(int x, int y) noexcept
        {
            set_chip(x / 64 == 0, x / 64 == 1);
            set_page(y / 8);
            set_column(x % 64);
            set_data(1 << (y % 8));
        }
    };
} // namespace gb7

#endif // LCD_HPP

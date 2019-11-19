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
        class CS1_Pin, class CS2_Pin, class E_Pin, class Reset_Pin>
    class lcd
    {
    private:
        RS_Pin m_rs;
        RW_Pin m_rw;
        DataBus_Port m_data_bus;
        CS1_Pin m_cs1;
        CS2_Pin m_cs2;
        E_Pin m_e;
        Reset_Pin m_reset;

        bool dirty[16][8] = { false };
        uint8_t buffer[2][8][64] = { 0 };


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
        inline void set_chip(uint8_t c) noexcept
        {
            m_cs1 = c == 0;
            m_cs2 = c == 1;
            _delay_us(0.14);
        }

    public:
        lcd() noexcept = default;
        ~lcd() = default;

        inline void init() noexcept
        {
            m_cs1 = true;
            m_cs2 = true;
            m_reset = true;
            _delay_ms(30);

            set_on(false);

            set_page(0);
            set_column(0);
            set_start_line(0);

            clear();

            set_on(true);
        }

        inline void clear()
        {
            m_cs1 = true;
            m_cs2 = true;

            for (uint8_t chunk_y = 0; chunk_y < 8; chunk_y++)
            {
                set_page(chunk_y);
                for (uint8_t chunk_x = 0; chunk_x < 8; chunk_x++)
                {
                    dirty[0 + chunk_x][chunk_y] = false;
                    dirty[8 + chunk_x][chunk_y] = false;
                    for (uint8_t i = 0; i < 8; i++)
                    {
                        const uint8_t column = chunk_x * 8 + i;
                        buffer[0][chunk_y][column] = 0;
                        buffer[1][chunk_y][column] = 0;
                        set_data(0);
                    }
                }
            }
        }

        inline void set_pixel(uint8_t x, uint8_t y, bool value) noexcept
        {
            if (x < 128 && y < 64)
            {
                const uint8_t chip = x / 64;
                const uint8_t page = y / 8;
                const uint8_t column = x % 64;
                const uint8_t mask = value ? 1 << (y % 8) : 0;
                if (((buffer[chip][page][column] & mask) ^ mask) != 0)
                {
                    if (value)
                    {
                        buffer[chip][page][column] |= mask;
                    }
                    else
                    {
                        buffer[chip][page][column] &= ~mask;
                    }
                    dirty[x / 8][page] = true;
                }
            }
        }

        inline bool get_pixel(uint8_t x, uint8_t y) const noexcept
        {
            if (x < 128 && y < 64)
            {
                const uint8_t chip = x / 64;
                const uint8_t page = y / 8;
                const uint8_t column = x % 64;
                const uint8_t mask = 1 << (y % 8);
                return (buffer[chip][page][column] & mask) != 0;
            }
            return false;
        }

        inline void draw_line(uint8_t x, uint8_t y, uint8_t to_x, uint8_t to_y) noexcept
        {
            const uint8_t x_start = min(x, to_x);
            const uint8_t x_end = max(x, to_x);
            const uint8_t y_start = min(y, to_y);
            const uint8_t y_end = max(y, to_y);

            const uint16_t width = x_end - x_start;
            const uint16_t height = y_end - y_start;

            if (width > height)
            {
                for (uint16_t i = 0; i < width; i++)
                {
                    set_pixel(x_start + i, y_start + i * height / width, true);
                }
            }
            else
            {
                for (uint16_t i = 0; i < height; i++)
                {
                    set_pixel(x_start + i * width / height, y_start + i, true);
                }
            }
        }

        inline void draw_ascii_char(uint8_t line, uint8_t column, char c) noexcept
        {
            if (line > 8 || column > 32) return;

            uint8_t index;
            if (static_cast<char>(32) < c && c <= static_cast<char>(127))
                index = static_cast<int>(c) - 32;
            else if (static_cast<char>(161) < c && c <= static_cast<char>(224))
                index = static_cast<int>(c) - 161 + 96;
            else return;

            for (uint8_t i = 0; i < 3; i++)
            {
                buffer[column / 16][line][(column % 16) * 4 + i] = gb7::font::misaki_font_former[index][i];
            }
            dirty[column / 2][line] = true;
        }

        inline void draw_ascii_string(uint8_t line, uint8_t column, const char* s) noexcept
        {
            uint8_t temp_column = column;

            for (uint8_t i = 0; s[i] != 0x0000; i++)
            {
                if (s[i] == '\n')
                {
                    temp_column = column;
                    line++;
                }
                else if (temp_column < 32)
                {
                    
                    draw_ascii_char(line, temp_column, s[i]);
                    temp_column++;
                }
            }
        }

        inline void draw_jis_char(uint8_t line, uint8_t column, uint64_t c) noexcept
        {
            if (line > 8 || column > 16) return;
            c--;

            for (uint8_t i = 0; i < 7; i++)
            {
                buffer[column / 8][line][(column % 8) * 8 + i] = (c >> 8 * (6 - i)) & 0xff;
            }
            dirty[column][line] = true;
        }

        inline void draw_jis_string(uint8_t line, uint8_t column, const uint64_t* str) noexcept
        {
            for (uint8_t i = 0; str[i] != 0 && (column + i) < 16; i++)
            {
                draw_jis_char(line, column + i, str[i]);
            }
        }

        template<typename... Args>
        inline void printf(uint8_t line, uint8_t column, const char* format, Args... args)
        {
            char buf[33];
            snprintf(buf, 33, format, args...);
            draw_ascii_string(line, column, buf);
        }

        inline void update() noexcept
        {
            for (uint8_t chip = 0; chip < 2; chip++)
            {
                set_chip(chip);
                for (uint8_t chunk_y = 0; chunk_y < 8; chunk_y++)
                {
                    set_page(chunk_y);
                    for (uint8_t chunk_x = 0; chunk_x < 8; chunk_x++)
                    {
                        if (dirty[chip * 8 + chunk_x][chunk_y])
                        {
                            for (uint8_t i = 0; i < 8; i++)
                            {
                                const uint8_t column = chunk_x * 8 + i;
                                set_column(column);
                                set_data(buffer[chip][chunk_y][column]);
                            }
                            dirty[chip * 8 + chunk_x][chunk_y] = false;
                        }
                    }
                }
            }
        }
        
        inline void set_pixel_immediataly(int x, int y) noexcept
        {
            set_chip(x / 64);
            set_page(y / 8);
            set_column(x % 64);
            set_data(1 << (y % 8));
        }
    };
}

#endif // LCD_HPP

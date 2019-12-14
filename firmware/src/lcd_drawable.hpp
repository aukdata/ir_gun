#ifndef LCD_DRAWABLE_HPP
#define LCD_DRAWABLE_HPP

#include "lcd.hpp"

namespace gb7
{
#if defined __cpp_concepts && __cpp_concepts >= 201907
    template<PinWritable RS_Pin, PinWritable RW_Pin, PortWritable DataBus_Port,
        PinWritable CS1_Pin, PinWritable CS2_Pin, PinWritable E_Pin, RST_Pin>
#else
    template<class RS_Pin, class RW_Pin, class DataBus_Port,
        class CS1_Pin, class CS2_Pin, class E_Pin, class RST_Pin>
#endif
    class lcd_drawable
    {
    private:
        lcd<RS_Pin, RW_Pin, DataBus_Port, CS1_Pin, CS2_Pin, E_Pin, RST_Pin> graphical_lcd;
        bool dirty[16][8] = { false };
        uint8_t buffer[2][8][64] = { 0 };

    public:
        lcd_drawable() noexcept
        {
            graphical_lcd.init();
        }

        inline void clear_immediataly() noexcept
        {
            graphical_lcd.set_chip(true, true);

            for (uint8_t chunk_y = 0; chunk_y < 8; chunk_y++)
            {
                graphical_lcd.set_page(chunk_y);
                for (uint8_t chunk_x = 0; chunk_x < 8; chunk_x++)
                {
                    dirty[0 + chunk_x][chunk_y] = false;
                    dirty[8 + chunk_x][chunk_y] = false;
                    for (uint8_t i = 0; i < 8; i++)
                    {
                        const uint8_t column = chunk_x * 8 + i;
                        buffer[0][chunk_y][column] = 0;
                        buffer[1][chunk_y][column] = 0;
                        graphical_lcd.set_data(0);
                    }
                }
            }
        }

        inline void clear() noexcept
        {
            for (uint8_t chunk_y = 0; chunk_y < 8; chunk_y++)
            {
                for (uint8_t chunk_x = 0; chunk_x < 8; chunk_x++)
                {
                    bool is_left_empty = true;
                    bool is_right_empty = true;
                    for (uint8_t i = 0; i < 8; i++)
                    {
                        const uint8_t column = chunk_x * 8 + i;
                        if (buffer[0][chunk_y][column] != 0)
                        {
                            buffer[0][chunk_y][column] = 0;
                            is_left_empty = false;
                        }
                        if (buffer[1][chunk_y][column] != 0)
                        {
                            buffer[1][chunk_y][column] = 0;
                            is_right_empty = false;
                        }
                    }
                    if (!is_left_empty)
                        dirty[0 + chunk_x][chunk_y] = true;
                    if (!is_right_empty)
                        dirty[8 + chunk_x][chunk_y] = true;
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

        inline void draw_zenkaku_char(uint8_t line, uint8_t column, uint64_t c) noexcept
        {
            if (line > 8 || column > 16) return;
            c--;

            for (uint8_t i = 0; i < 7; i++)
            {
                buffer[column / 8][line][(column % 8) * 8 + i] = (c >> 8 * (6 - i)) & 0xff;
            }
            dirty[column][line] = true;
        }

        inline void draw_zenkaku_string(uint8_t line, uint8_t column, const uint64_t* str) noexcept
        {
            for (uint8_t i = 0; str[i] != 0 && (column + i) < 16; i++)
            {
                draw_zenkaku_char(line, column + i, str[i]);
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
                graphical_lcd.set_chip(chip == 0, chip == 1);
                for (uint8_t chunk_y = 0; chunk_y < 8; chunk_y++)
                {
                    graphical_lcd.set_page(chunk_y);
                    for (uint8_t chunk_x = 0; chunk_x < 8; chunk_x++)
                    {
                        if (dirty[chip * 8 + chunk_x][chunk_y])
                        {
                            for (uint8_t i = 0; i < 8; i++)
                            {
                                const uint8_t column = chunk_x * 8 + i;
                                graphical_lcd.set_column(column);
                                graphical_lcd.set_data(buffer[chip][chunk_y][column]);
                            }
                            dirty[chip * 8 + chunk_x][chunk_y] = false;
                        }
                    }
                }
            }
        }
    };
} // namespace gb7

#endif // LCD_DRAWABLE_HPP
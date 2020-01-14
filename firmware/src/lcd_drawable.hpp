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
        bool dirty[8][16] = { false };
        uint8_t buffer[2][8][64] = { 0 };

    public:
        lcd_drawable() noexcept
        {
            graphical_lcd.init();
        }

        inline void clear_immediataly() noexcept
        {
            graphical_lcd.set_chip(true, true);

            for (uint8_t chunk_x = 0; chunk_x < 8; chunk_x++)
            {
                graphical_lcd.set_page(chunk_x);
                for (uint8_t chunk_y = 0; chunk_y < 8; chunk_y++)
                {
                    dirty[chunk_x][0 + chunk_y] = false;
                    dirty[chunk_x][8 + chunk_y] = false;
                    for (uint8_t i = 0; i < 8; i++)
                    {
                        const uint8_t column = chunk_y * 8 + i;
                        buffer[0][chunk_x][column] = 0;
                        buffer[1][chunk_x][column] = 0;
                        graphical_lcd.set_data(0);
                    }
                }
            }
        }

        inline void clear() noexcept
        {
            for (uint8_t chunk_x = 0; chunk_x < 8; chunk_x++)
            {
                for (uint8_t chunk_y = 0; chunk_y < 8; chunk_y++)
                {
                    bool is_left_empty = true;
                    bool is_right_empty = true;
                    for (uint8_t i = 0; i < 8; i++)
                    {
                        const uint8_t row = chunk_y * 8 + i;
                        if (buffer[0][chunk_x][row] != 0)
                        {
                            buffer[0][chunk_x][row] = 0;
                            is_left_empty = false;
                        }
                        if (buffer[1][chunk_x][row] != 0)
                        {
                            buffer[1][chunk_x][row] = 0;
                            is_right_empty = false;
                        }
                    }
                    if (!is_left_empty)
                        dirty[chunk_x][0 + chunk_y] = true;
                    if (!is_right_empty)
                        dirty[chunk_x][8 + chunk_y] = true;
                }
            }
        }

        constexpr bool is_position_inside(int x, int y) const
        {
            return 0 <= x && x < 64 && 0 <= y && y < 128;
        }

        inline void set_pixel(int x, int y, bool value) noexcept
        {
            if (is_position_inside(x, y))
            {
                const int chip = y / 64;
                const int page = x / 8;
                const int row = y % 64;
                const int mask = value ? 1 << (x % 8) : 0;
                if (((buffer[chip][page][row] & mask) ^ mask) != 0)
                {
                    if (value)
                    {
                        buffer[chip][page][row] |= mask;
                    }
                    else
                    {
                        buffer[chip][page][row] &= ~mask;
                    }
                    dirty[page][y / 8] = true;
                }
            }
        }

        inline bool get_pixel(int x, int y) const noexcept
        {
            if (is_position_inside(x, y))
            {
                const int chip = y / 64;
                const int page = x / 8;
                const int row = y % 64;
                const int mask = 1 << (x % 8);
                return (buffer[chip][page][row] & mask) != 0;
            }
            return false;
        }

        void draw_line(int x0, int y0, int x1, int y1) noexcept
        {
            const bool steep = abs(x1 - x0) < abs(y1 - y0);
            if (steep)
            {
                swap(x0, y0);
                swap(x1, y1);
            }

            if (x0 > x1)
            {
                swap(x0, x1);
                swap(y0, y1);
            }

            const int dx = x1 - x0;
            const int dy = abs(y1 - y0);
            const int dx2 = 2 * dx;
            const int dy2 = 2 * dy;
            const int increasing = y0 < y1;

            int x = x0;
            int y = y0;
            int error = 0;

            if (steep)
            {
                set_pixel(y0, x0, true);
            }
            else
            {
                set_pixel(x0, y0, true);
            }
            
            while (x < x1)
            {
                if (error + dy2 - dx > 0)
                {
                    if (increasing)
                    {
                        y++;
                    }
                    else
                    {
                        y--;
                    }
                    
                    error += dy2 - dx2;
                }
                else
                {
                    error += dy2;
                }
                x++;

                if (steep)
                {
                    set_pixel(y, x, true);
                }
                else
                {
                    set_pixel(x, y, true);
                }
            }
        }

        void draw_circle(int x0, int y0, int x1, int y1) noexcept
        {
            const bool steep = abs(x1 - x0) < abs(y1 - y0);
            if (steep)
            {
                swap(x0, y0);
                swap(x1, y1);
            }

            if (x0 > x1)
            {
                swap(x0, x1);
                swap(y0, y1);
            }

            const int dx = x1 - x0;
            const int dy = abs(y1 - y0);
            const int dx2 = 2 * dx;
            const int dy2 = 2 * dy;
            const int increasing = y0 < y1;

            int x = x0;
            int y = y0;
            int error = 0;

            if (steep)
            {
                set_pixel(y0, x0, true);
            }
            else
            {
                set_pixel(x0, y0, true);
            }
            
            while (x < x1)
            {
                if (error + dy2 - dx > 0)
                {
                    if (increasing)
                    {
                        y++;
                    }
                    else
                    {
                        y--;
                    }
                    
                    error += dy2 - dx2;
                }
                else
                {
                    error += dy2;
                }
                x++;

                if (steep)
                {
                    set_pixel(y, x, true);
                }
                else
                {
                    set_pixel(x, y, true);
                }
            }
        }

        void draw_circle(int x0, int y0, int r) noexcept
        {
            int x = 0;
            int y = 0 + r;
            int error = 0;

            set_pixel(x0 + r, y0    , true);
            set_pixel(x0    , y0 + r, true);
            set_pixel(x0 - r, y0    , true);
            set_pixel(x0    , y0 - r, true);
            
            while (x < y)
            {
                if (error + 8*x - 4*y + 5 > 0)
                {
                    error += 8*x - 8*y + 8;
                    y--;
                }
                else
                {
                    error += 8*x + 4;
                }
                x++;

                set_pixel(x0 + x, y0 + y, true);
                set_pixel(x0 + y, y0 + x, true);
                set_pixel(x0 + y, y0 - x, true);
                set_pixel(x0 + x, y0 - y, true);

                set_pixel(x0 - x, y0 + y, true);
                set_pixel(x0 - y, y0 + x, true);
                set_pixel(x0 - y, y0 - x, true);
                set_pixel(x0 - x, y0 - y, true);
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
            dirty[line][column / 2] = true;
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
            dirty[line][column] = true;
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
                        if (dirty[chunk_y][chip * 8 + chunk_x])
                        {
                            for (uint8_t i = 0; i < 8; i++)
                            {
                                const uint8_t column = chunk_x * 8 + i;
                                graphical_lcd.set_column(column);
                                graphical_lcd.set_data(buffer[chip][chunk_y][column]);
                            }
                            dirty[chunk_y][chip * 8 + chunk_x] = false;
                        }
                    }
                }
            }
        }
    };
} // namespace gb7

#endif // LCD_DRAWABLE_HPP
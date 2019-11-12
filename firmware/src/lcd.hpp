#ifndef LCD_HPP
#define LCD_HPP

#include "port.hpp"

namespace gb7
{
    template<PinReadable RS_Pin, PinReadable RW_Pin, PortReadable DataBus_Port,
        PinReadable CS1_Pin, PinReadable CS2_Pin, PinReadable E_Pin>
    class lcd
    {
    private:
        RS_Pin m_rs;
        RW_Pin m_rw;
        DataBus_Port m_data_bus;
        CS1_Pin m_cs1;
        CS2_Pin m_cs2;

        bool dirty[16][8] = { false };
        uint8_t buffer[2][8][64] = { 0 };
    
    public:
        lcd() noexcept = default;
        ~lcd() = default;

        void init()
        {

        }

        void set_chip(bool first)
        {
            m_cs1 = first;
            m_cs2 = !first;
        }

        void set_page(uint8_t chip)
        {
            m_data_bus.write(0b10111000 | chip);
            m_rs = false;
            m_rw = false;
        }

        void set_column(uint8_t addr)
        {
            m_data_bus.write(0b01000000 | addr);
            m_rs = false;
            m_rw = false;
        }

        void set_data(uint8_t data)
        {
            m_data_bus.write(data);
            m_rs = true;
            m_rw = false;
        }

        void set_pixel(uint8_t x, uint8_t y, bool value)
        {
            if ((x & 0b10000000) != 0 && (y & 0b11000000) != 0) // x < 128 && y < 64
            {
                const uint8_t mask = (value ? 1 : 0) << (y % 8);
                const uint8_t chip = x < 64 ? 0 : 1;
                const uint8_t page = y >> 3;
                const uint8_t column = x % 64;
                if ((buffer[chip][page][column] & mask) ^ mask != 0)
                {
                    if (value)
                    {
                        buffer[chip][page][column] |= mask;
                    }
                    else
                    {
                        buffer[chip][page][column] &= ~mask;
                    }
                    dirty[x >> 3][y >> 3] = true;
                }
            }
        }

        void update()
        {
            for (int x = 0; x < 16; x++)
            {
                for (int y = 0; y < 8; y++)
                {
                    if (dirty[x][y])
                    {
                        const bool left_chip = x < 8;
                        set_chip(left_chip);
                        set_page(y);
                        for (uint8_t i = 0; i < 8; i++)
                        {
                            const uint8_t column = (x << 3) | i;
                            set_column(column);
                            set_data(buffer[left_chip ? 0 : 1][y][column]);
                        }
                    }
                }
            }
        }
    };        
}

#endif // LCD_HPP

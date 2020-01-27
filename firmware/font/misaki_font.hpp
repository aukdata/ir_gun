#ifndef MISAKI_FONT_HPP
#define MISAKI_FONT_HPP

#include <stdint.h>

namespace gb7::font
{
    static constexpr uint8_t misaki_font[][4] =
    {
        { 0x44, 0x44, 0x40, 0x40, }, // 034: !
        { 0xaa, 0x00, 0x00, 0x00, }, // 035: "
        { 0xae, 0xaa, 0xae, 0xa0, }, // 036: #
        { 0x4e, 0x8e, 0x2e, 0x40, }, // 037: $
        { 0x08, 0x24, 0x82, 0x00, }, // 038: %
        { 0x4a, 0xa4, 0xa8, 0x60, }, // 039: &
        { 0x44, 0x00, 0x00, 0x00, }, // 040: '
        { 0x24, 0x44, 0x44, 0x20, }, // 041: (
        { 0x84, 0x44, 0x44, 0x80, }, // 042: )
        { 0x0a, 0x4e, 0x4a, 0x00, }, // 043: *
        { 0x04, 0x4e, 0x44, 0x00, }, // 044: +
        { 0x00, 0x00, 0x04, 0x80, }, // 045: ,
        { 0x00, 0x0e, 0x00, 0x00, }, // 046: -
        { 0x00, 0x00, 0x00, 0x40, }, // 047: .
        { 0x00, 0x24, 0x80, 0x00, }, // 048: /
        { 0x4a, 0xae, 0xaa, 0x40, }, // 049: 0
        { 0x4c, 0x44, 0x44, 0xe0, }, // 050: 1
        { 0x4a, 0x24, 0x48, 0xe0, }, // 051: 2
        { 0x4a, 0x24, 0x2a, 0x40, }, // 052: 3
        { 0x26, 0xaa, 0xe2, 0x20, }, // 053: 4
        { 0xe8, 0xca, 0x2a, 0x40, }, // 054: 5
        { 0x4a, 0x8c, 0xaa, 0x40, }, // 055: 6
        { 0xea, 0x24, 0x44, 0x40, }, // 056: 7
        { 0x4a, 0xa4, 0xaa, 0x40, }, // 057: 8
        { 0x4a, 0xa6, 0x2a, 0x40, }, // 058: 9
        { 0x00, 0x40, 0x04, 0x00, }, // 059: :
        { 0x00, 0x40, 0x04, 0x80, }, // 060: ;
        { 0x02, 0x48, 0x42, 0x00, }, // 061: <
        { 0x00, 0xe0, 0xe0, 0x00, }, // 062: =
        { 0x08, 0x42, 0x48, 0x00, }, // 063: >
        { 0x4a, 0x24, 0x40, 0x40, }, // 064: ?
        { 0x4a, 0xea, 0xc8, 0x60, }, // 065: @
        { 0x4a, 0xaa, 0xea, 0xa0, }, // 066: A
        { 0xca, 0xac, 0xaa, 0xc0, }, // 067: B
        { 0x4a, 0x88, 0x8a, 0x40, }, // 068: C
        { 0xca, 0xaa, 0xaa, 0xc0, }, // 069: D
        { 0xe8, 0x8c, 0x88, 0xe0, }, // 070: E
        { 0xe8, 0x8c, 0x88, 0x80, }, // 071: F
        { 0x4a, 0x8a, 0xaa, 0x60, }, // 072: G
        { 0xaa, 0xae, 0xaa, 0xa0, }, // 073: H
        { 0xe4, 0x44, 0x44, 0xe0, }, // 074: I
        { 0x22, 0x22, 0x2a, 0x40, }, // 075: J
        { 0xaa, 0xcc, 0xaa, 0xa0, }, // 076: K
        { 0x88, 0x88, 0x88, 0xe0, }, // 077: L
        { 0xae, 0xee, 0xaa, 0xa0, }, // 078: M
        { 0xca, 0xaa, 0xaa, 0xa0, }, // 079: N
        { 0x4a, 0xaa, 0xaa, 0x40, }, // 080: O
        { 0xca, 0xac, 0x88, 0x80, }, // 081: P
        { 0x4a, 0xaa, 0xea, 0x60, }, // 082: Q
        { 0xca, 0xac, 0xaa, 0xa0, }, // 083: R
        { 0x4a, 0x84, 0x2a, 0x40, }, // 084: S
        { 0xe4, 0x44, 0x44, 0x40, }, // 085: T
        { 0xaa, 0xaa, 0xaa, 0xe0, }, // 086: U
        { 0xaa, 0xaa, 0xa4, 0x40, }, // 087: V
        { 0xaa, 0xae, 0xee, 0xa0, }, // 088: W
        { 0xaa, 0x44, 0x4a, 0xa0, }, // 089: X
        { 0xaa, 0xa4, 0x44, 0x40, }, // 090: Y
        { 0xe2, 0x44, 0x48, 0xe0, }, // 091: Z
        { 0x64, 0x44, 0x44, 0x60, }, // 092: [
        { 0xa4, 0xe4, 0xe4, 0x40, }, // 093: ＼
        { 0xc4, 0x44, 0x44, 0xc0, }, // 094: ]
        { 0x4a, 0x00, 0x00, 0x00, }, // 095: ^
        { 0x00, 0x00, 0x00, 0xe0, }, // 096: _
        { 0x42, 0x00, 0x00, 0x00, }, // 097: `
        { 0x00, 0xc2, 0x6a, 0x60, }, // 098: a
        { 0x88, 0xca, 0xaa, 0xc0, }, // 099: b
        { 0x00, 0x68, 0x88, 0x60, }, // 100: c
        { 0x22, 0x6a, 0xaa, 0x60, }, // 101: d
        { 0x00, 0x4a, 0xe8, 0x60, }, // 102: e
        { 0x64, 0xe4, 0x44, 0x40, }, // 103: f
        { 0x00, 0x6a, 0x62, 0xc0, }, // 104: g
        { 0x88, 0xca, 0xaa, 0xa0, }, // 105: h
        { 0x40, 0x44, 0x44, 0x40, }, // 106: i
        { 0x20, 0x22, 0x2a, 0x40, }, // 107: j
        { 0x88, 0xaa, 0xca, 0xa0, }, // 108: k
        { 0xc4, 0x44, 0x44, 0x40, }, // 109: l
        { 0x00, 0xce, 0xee, 0xa0, }, // 110: m
        { 0x00, 0xca, 0xaa, 0xa0, }, // 111: n
        { 0x00, 0x4a, 0xaa, 0x40, }, // 112: o
        { 0x00, 0xca, 0xac, 0x80, }, // 113: p
        { 0x00, 0x6a, 0xa6, 0x20, }, // 114: q
        { 0x00, 0xac, 0x88, 0x80, }, // 115: r
        { 0x00, 0x68, 0x42, 0xc0, }, // 116: s
        { 0x04, 0xe4, 0x44, 0x60, }, // 117: t
        { 0x00, 0xaa, 0xaa, 0xe0, }, // 118: u
        { 0x00, 0xaa, 0xa4, 0x40, }, // 119: v
        { 0x00, 0xaa, 0xee, 0xa0, }, // 120: w
        { 0x00, 0xaa, 0x4a, 0xa0, }, // 121: x
        { 0x00, 0xaa, 0x62, 0xc0, }, // 122: y
        { 0x00, 0xe2, 0x48, 0xe0, }, // 123: z
        { 0x24, 0x48, 0x44, 0x20, }, // 124: {
        { 0x44, 0x44, 0x44, 0x40, }, // 125: |
        { 0x84, 0x42, 0x44, 0x80, }, // 126: }
        { 0xe0, 0x00, 0x00, 0x00, }, // 127: ~
        
        { 0x00, 0x00, 0x4a, 0x40, }, // 162: 
        { 0xe8, 0x88, 0x80, 0x00, }, // 163: 
        { 0x00, 0x22, 0x22, 0xe0, }, // 164: 
        { 0x00, 0x00, 0x08, 0x40, }, // 165: 
        { 0x00, 0x04, 0x00, 0x00, }, // 166: 
        { 0x0e, 0x2e, 0x24, 0x80, }, // 167: ｦ
        { 0x00, 0x0e, 0x64, 0x80, }, // 168: ｧ
        { 0x00, 0x02, 0x4c, 0x40, }, // 169: ｨ
        { 0x00, 0x04, 0xea, 0x20, }, // 170: ｩ
        { 0x00, 0x00, 0xe4, 0xe0, }, // 171: ｪ
        { 0x00, 0x02, 0xe6, 0xa0, }, // 172: ｫ
        { 0x00, 0x04, 0xe6, 0x40, }, // 173: ｬ
        { 0x00, 0x00, 0xc4, 0xe0, }, // 174: ｭ
        { 0x00, 0x0e, 0x62, 0xe0, }, // 175: ｮ
        { 0x00, 0x0e, 0xe2, 0xc0, }, // 176: ｯ
        { 0x00, 0x86, 0x00, 0x00, }, // 177: ｰ
        { 0x0e, 0x26, 0x44, 0x80, }, // 178: ｱ
        { 0x22, 0x4c, 0x44, 0x40, }, // 179: ｲ
        { 0x4e, 0xaa, 0x22, 0x40, }, // 180: ｳ
        { 0x0e, 0x44, 0x44, 0xe0, }, // 181: ｴ
        { 0x2e, 0x26, 0xa2, 0x60, }, // 182: ｵ
        { 0x4e, 0x66, 0xaa, 0xa0, }, // 183: ｶ
        { 0x4e, 0x44, 0xe4, 0x40, }, // 184: ｷ
        { 0x46, 0xa2, 0x24, 0x80, }, // 185: ｸ
        { 0x8e, 0xc4, 0x44, 0x80, }, // 186: ｹ
        { 0x0e, 0x22, 0x22, 0xe0, }, // 187: ｺ
        { 0xae, 0xaa, 0x24, 0x80, }, // 188: ｻ
        { 0xc0, 0xc0, 0x22, 0xc0, }, // 189: ｼ
        { 0x0e, 0x24, 0x4a, 0xa0, }, // 190: ｽ
        { 0x8e, 0xaa, 0x88, 0xe0, }, // 191: ｾ
        { 0xa6, 0x62, 0x24, 0x80, }, // 192: ｿ
        { 0x46, 0xa6, 0x24, 0x80, }, // 193: ﾀ
        { 0x2c, 0x4e, 0x44, 0x80, }, // 194: ﾁ
        { 0x0e, 0xee, 0x22, 0xc0, }, // 195: ﾂ
        { 0x0e, 0x0e, 0x44, 0x80, }, // 196: ﾃ
        { 0x88, 0x8c, 0xa8, 0x80, }, // 197: ﾄ
        { 0x44, 0xe4, 0x44, 0x80, }, // 198: ﾅ
        { 0x06, 0x00, 0x00, 0xe0, }, // 199: ﾆ
        { 0x0e, 0x2a, 0x44, 0xa0, }, // 200: ﾇ
        { 0x4e, 0x24, 0xe4, 0x40, }, // 201: ﾈ
        { 0x22, 0x22, 0x24, 0x80, }, // 202: ﾉ
        { 0x42, 0xaa, 0xaa, 0xa0, }, // 203: ﾊ
        { 0x88, 0xe8, 0x88, 0x60, }, // 204: ﾋ
        { 0x0e, 0x22, 0x24, 0x80, }, // 205: ﾌ
        { 0x04, 0x4a, 0xa2, 0x20, }, // 206: ﾍ
        { 0x4e, 0x4e, 0xe4, 0x40, }, // 207: ﾎ
        { 0x0e, 0x22, 0xc4, 0x20, }, // 208: ﾏ
        { 0x0e, 0x06, 0x00, 0xe0, }, // 209: ﾐ
        { 0x44, 0x48, 0xaa, 0xe0, }, // 210: ﾑ
        { 0x2a, 0x44, 0x4a, 0x80, }, // 211: ﾒ
        { 0x0e, 0x4e, 0x44, 0x60, }, // 212: ﾓ
        { 0x4e, 0x66, 0x44, 0x40, }, // 213: ﾔ
        { 0x0c, 0x44, 0x44, 0xe0, }, // 214: ﾕ
        { 0x0e, 0x2e, 0x22, 0xe0, }, // 215: ﾖ
        { 0x0e, 0x0e, 0x22, 0xc0, }, // 216: ﾗ
        { 0xaa, 0xaa, 0x22, 0x40, }, // 217: ﾘ
        { 0x44, 0xcc, 0xcc, 0xe0, }, // 218: ﾙ
        { 0x88, 0x88, 0x8a, 0xc0, }, // 219: ﾚ
        { 0x0e, 0xaa, 0xaa, 0xe0, }, // 220: ﾛ
        { 0x0e, 0xa2, 0x24, 0x80, }, // 221: ﾜ
        { 0x0c, 0x00, 0x22, 0xc0, }, // 222: ﾝ
        { 0xa0, 0x00, 0x00, 0x00, }, // 223: 'ﾞ
        { 0x4a, 0x40, 0x00, 0x00, }, // 224: 'ﾟ
    };
}

#endif // MISAKI_FONT_HPP

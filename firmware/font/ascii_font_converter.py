from PIL import Image
import numpy as np
from matplotlib import pyplot as plt

# separate font data by 4 bits

# load a font image
img = Image.open('misaki_png_2019-10-19/misaki_gothic_2nd_4x8.png')
width, height = img.size

# convert the image to ndarray of pixels
img_pixels = np.array([[0 if img.getpixel((i, j)) != 0 else 1 for i in range(width)] for j in range(height)])

# split pixels into 8x8
fonts = [img_pixels[(x * 8):((x + 1) * 8), (y * 4):((y + 1) * 4)] for x in range(0, height // 8) for y in range(0, width // 4) ]



# write file the converted font
with open("misaki_font.hpp", mode="w", encoding="utf8") as f:
    f.write(
        "#ifndef MISAKI_FONT_HPP\n"
        "#define MISAKI_FONT_HPP\n"
        "\n"
        "#include <stdint.h>\n"
        "\n"
        "namespace gb7::font\n"
        "{\n"
        "    static constexpr uint8_t misaki_font[][4] =\n"
        "    {\n"
    )
    
    for i, font in enumerate(fonts):
        f.write("        { ")
        has_shape = False

        font_btis = [0, 0, 0, 0, 0, 0, 0, 0]
        for row in range(8):
            for column in range(4):
                font_btis[row] = font_btis[row] | (font[row][column] << (3 - column))
            
        for row in range(4):
            h = (font_btis[2 * row] << 4) | font_btis[2 * row + 1]
            if h != 0:
                has_shape = True
            f.write(f"0x{h:02x}, ")

        if has_shape:
            f.write("}, // " + f"{(i+1):03}" + ": " + chr(i) + "\n")
        else:
            f.write("}, // " + f"{(i+1):03}" + ": invalid\n")

#    for i, font in enumerate(fonts):
#        f.write("        { ")
#        has_shape = False
#        for c in font:
#            print(str(font))
#            h = 0x00;
#            for j in range(0, 4):
#                h = h | c[3 - j] << (3 - j)
#            f.write(f"0x{h:02x}, ")
#            if h != 0:
#                has_shape = True
#
#        if has_shape:
#            f.write("}, // " + f"{(i+1):03}" + ": " + chr(i) + "\n")
#        else:
#            f.write("}, // " + f"{(i+1):03}" + ": invalid\n")
    
    f.write(
        "    };\n"
        "}\n"
        "\n"
        "#endif // MISAKI_FONT_HPP\n"
    )


while True:
    c = input(":")
    v = 0

    try:
        v = int(c)
    except:
        v = ord(c)

    print(v)
    for font in fonts[v]:
        for c in font:
            print("#" if c == 1 else " ", end="")
        print("")

from PIL import Image
import numpy as np

# load a font image
img = Image.open('misaki_png_2019-10-19/misaki_gothic_2nd.png')
width, height = img.size

# convert the image to ndarray of pixels
img_pixels = np.array([[0 if img.getpixel((i,j)) != 0 else 1 for j in range(height)] for i in range(width)])

# split pixels into 8x8
fonts = [img_pixels[(x * 8):((x + 1) * 8), (y * 8):((y + 1) * 8)] for y in range(0, height // 8) for x in range(0, width // 8) ]

# write file the converted font
with open('misaki_gothic_2nd.py', mode='w', encoding='utf8') as f:
    f.write('misaki_font_table = {\n')

    for i, font in enumerate(fonts):
        has_shape = False
        shape = '0x'
        for j, c in enumerate(font):
            h = 0x00;
            for k in range(0, 8):
                h = h | c[7 - k] << (7 - k)

            if h != 0:
                has_shape = True
            if j < 7:
                shape += f'{h:02x}'

        if has_shape or i == 0:
            barray = b'\x1b\x24\x42' + bytearray([i // 94 + 0x21, i % 94 + 0x21]) + b'\x1b\x28\x42'
            try:
                f.write(f'    "{barray.decode("iso2022-jp")}": {shape},\n')
            except UnicodeDecodeError as e:
                print(e.reason)
                for code in e.object:
                    print(f'{hex(code)} ({str(code)})')
    
    f.write('}\n')

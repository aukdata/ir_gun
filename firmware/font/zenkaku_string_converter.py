import jaconv
import misaki_gothic_2nd

while True:
    s = input(':')
    s = jaconv.han2zen(s, kana=True, ascii=True, digit=True)
    print(s)

    print('static constexpr uint64_t str[] = {')
    for c in s:
        code = misaki_gothic_2nd.misaki_font_table[c]
        print('    0x{:016x},'.format(code + 1))
    print(f'    0x0\n}}; /* {s} */')

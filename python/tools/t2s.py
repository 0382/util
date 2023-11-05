# traditional to simplified chinese
import zhconv
import sys

def t2s(ft:str, fs:str):
    with open(ft, 'r', encoding='utf-8') as fp_t:
        data = fp_t.read()
    with open(fs, 'w', encoding='utf-8') as fp_s:
        fp_s.write(zhconv.convert(data, 'zh-hans'))

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("[usage] t2s.py traditional_file simplified_file")
    else:
        t2s(sys.argv[1], sys.argv[2])
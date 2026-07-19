import re
import sys

def main():
    if len(sys.argv) < 2:
        return
    filename = sys.argv[1]
    with open(filename, 'rb') as f:
        data = f.read()
    
    # 連続する印字可能文字(ascii)を抽出
    strings = re.findall(b'[ -~]{4,}', data)
    
    # 画像ファイルっぽいやつを抽出
    image_paths = []
    for s in strings:
        try:
            dec = s.decode('ascii')
            if re.search(r'\.(png|jpg|jpeg|tga|bmp)$', dec, re.IGNORECASE):
                image_paths.append(dec)
        except:
            pass

    for p in set(image_paths):
        print(p)

if __name__ == '__main__':
    main()

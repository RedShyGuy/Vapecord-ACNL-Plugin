# Created by PabloMK7
from PIL import Image
import sys

imorg = Image.open(sys.argv[1])
im = imorg.transpose(Image.ROTATE_270)
imorg.close()
pixels = im.convert('RGBA')
pix = list(pixels.getdata())

with open(sys.argv[1].replace(".png", ".bin"), "wb") as f:
	for p in pix:
		f.write(p[3].to_bytes(1, "little"))
		f.write(p[2].to_bytes(1, "little"))
		f.write(p[1].to_bytes(1, "little"))
		f.write(p[0].to_bytes(1, "little"))

# Python tool to convert an image to C array for SAF.
#
# usage: python convert_image.py [-b|-c] image_file
#
# -b   1bit image (otherwise color image)
# -c   compress image
#
# by drummyfish
# released under CC0 1.0.

import sys
from PIL import Image

def rgbTo332(rgb):
  return ((rgb[0] >> 5) << 5) | ((rgb[1] >> 5) << 2) | ((rgb[2] >> 6))

def hexByte(b):
  return "0x" + hex(b)[2:].zfill(2)

def findClosestColor(color332,palette):
  bestD = 10000
  bestI = 0

  for i in range(len(palette)):
    c = palette[i]

    dr = abs((color332 >> 5) - (c >> 5))
    dg = abs(((color332 >> 2) & 0x07) - ((c >> 2) & 0x07))
    db = abs((color332 & 0x03) - (c & 0x03))

    d = dr + dg + db

    if d < bestD:
      bestD = d
      bestI = i

  return bestI
  
MODE_NORMAL = 0
MODE_COMPRESSED = 1
MODE_BINARY = 2

filename = ""
mode = MODE_NORMAL

result = []

for s in sys.argv:
  if s [:2] == "-b":
    mode = MODE_BINARY
  if s [:2] == "-c":
    mode = MODE_COMPRESSED
  else:
    filename = s

imageArray = []

image = Image.open(filename).convert("RGB")
pixels = image.load()

result.append(image.size[0] % 256) # width
result.append(image.size[1] % 256) # height

byte = 0
bitPosition = 0

colorHistogram = [0 for i in range(256)]
palette = [-1 for i in range(16)]

if mode == MODE_COMPRESSED: # create 16 color palette
  for y in range(image.size[1]):
    for x in range(image.size[0]):
      colorHistogram[rgbTo332(pixels[(x,y)])] += 1

  for i in range(256):
    count = colorHistogram[i]

    for pos in range(16):
      if palette[pos] == -1 or count > colorHistogram[palette[pos]]:
        palette = (palette[:pos] + [i] + palette[pos:])[:-1]
        break

  for i in range(16):
    result.append(palette[i])

rlePrevious = -1
rleCount = 16

for y in range(image.size[1]):
  for x in range(image.size[0]):
    pixel = pixels[(x,y)]

    last = x == image.size[0] - 1 and y == image.size[1] - 1

    if mode == MODE_NORMAL:
      result.append(rgbTo332(pixel))
    elif mode == MODE_BINARY:
      byte = (byte << 1) | (1 if pixel[0] > 127 else 0)
      bitPosition += 1

      if bitPosition >= 8 or last:
        result.append(byte << (8 - bitPosition))
        bitPosition = 0
        byte = 0
    else: # MODE_COMPRESSED
      index = findClosestColor(rgbTo332(pixel),palette)

      if index != rlePrevious or rleCount >= 15 or last:
        if rlePrevious != -1:
          result.append(rlePrevious | (rleCount << 4))

        rlePrevious = index
        rleCount = 0
      else:
        rleCount += 1

print("uint8_t image[" + str(len(result)) + "] = {")

lineCount = 0
string = ""

for i in range(len(result)):
  string += hexByte(result[i])

  if i != len(result) - 1:
    string += ","

  lineCount += 1

  if lineCount >= 16:
    lineCount = 0
    string += "\n" 

print(string + "};")

/**
  Simple testing program for SAF.

  by drummyfish, released under CC0 1.0, public domain
*/

#define SAF_PROGRAM_NAME "Test"

#define SAF_SETTING_BACKGROUND_COLOR 0xe0

#define SAF_SETTING_FORCE_1BIT 0
#define SAF_SETTING_1BIT_DITHER 0
#define SAF_SETTING_FASTER_1BIT 2
#define SAF_SETTING_ENABLE_SOUND 1 

#include "saf.h"

uint8_t saveValue = 0;

void screen1()
{
  SAF_clearScreen(SAF_COLOR_BLACK);

  SAF_drawText("SAF test!",2,2,SAF_COLOR_WHITE,1);

  uint8_t index = 0;

  for (uint8_t y = 0; y < 16; ++y)
    for (uint8_t x = 0; x < 16; ++x)
    {
      uint8_t color = ((x % 8) << 5) | ((y % 8) << 2) | (x / 8 + 2 * (y / 8));

      int8_t x2 = 2 + 2 * x;
      int8_t y2 = 12 + 2 * y;

      for (int8_t j = 0; j < 2; ++j)
        for (int8_t i = 0; i < 2; ++i)
          SAF_drawPixel(x2 + i,y2 + j,index);

      index++;
    }

  char btnChars[SAF_BUTTONS + 1] = "UDLRABC";
  char btnString[2] = "x";

  for (int8_t i = 0; i < SAF_BUTTONS; ++i)
  {
    btnString[0] = btnChars[i];

    uint8_t pressed = SAF_buttonPressed(i);

    int8_t x = 2 + i * 8;

    SAF_drawRect(x,48,7,8,pressed ? SAF_COLOR_WHITE : SAF_COLOR_GRAY_DARK,1);
    SAF_drawText(btnString,x + 2,50,pressed ? SAF_COLOR_BLACK : SAF_COLOR_WHITE,1);

    for (int8_t j = 0; j < (pressed / 32); ++j)
      SAF_drawPixel(x + j,58,SAF_COLOR_WHITE);
  }

  SAF_drawCircle(48,28,12,SAF_COLOR_WHITE,0);
  SAF_drawLine(48,28,48 + (SAF_sin(SAF_frame()) * 12) / 128,28 + (SAF_cos(SAF_frame()) * 12) / 128,SAF_COLOR_YELLOW);
}

void screen2()
{
  SAF_clearScreen(SAF_COLOR_WHITE);
  
  SAF_drawRect(43,46,SAF_sin(((SAF_frame() * 2) % 256)) / 2,SAF_cos((SAF_frame() % 256)) / 2,SAF_COLOR_RED,1);

  SAF_drawCircle(-10,-20,SAF_frame() % 128,SAF_COLOR_BLUE,0);
  SAF_drawCircle(31,31,(SAF_frame() >> 1) % 32,SAF_COLOR_GREEN,0);

  SAF_drawText("random:\n b",1,1,SAF_COLOR_BLACK,1);
      
  uint8_t bit = (SAF_frame() >> 4) % 8;

  char numberStr[2];

  SAF_intToStr(bit,numberStr);
  
  SAF_drawText(numberStr,11,6,SAF_COLOR_BLACK,1);

  bit = 0x01 << bit;   

  for (int8_t y = 0; y < 16; ++y)
    for (int8_t x = 0; x < 16; ++x)
    {
      uint8_t value = SAF_random();
      SAF_drawPixel(44 + x,11 + y,value);
      SAF_drawPixel(23 + x,11 + y,value < 128 ? SAF_COLOR_BLACK : SAF_COLOR_GRAY);
      value = value & bit;
      value = value ? SAF_COLOR_BLACK : SAF_COLOR_GRAY;
      SAF_drawPixel(2 + x,11 + y,value);
    }
  
  SAF_drawText("frame\ntime\nplat.",1,30,SAF_COLOR_BLACK,1);

  char numberStr2[32];

  SAF_intToStr(SAF_frame(),numberStr2);
  SAF_drawText(numberStr2,34,30,SAF_COLOR_BLACK,1);

  SAF_floatToStr(SAF_time() / 1000.0,numberStr2,3);
  SAF_drawText(numberStr2,34,35,SAF_COLOR_BLACK,1);
 
  SAF_drawText(SAF_PLATFORM_NAME,34,40,SAF_COLOR_BLACK,1);

  char allChars[10][11] =
  {
    " !\"#$%&'()",
    "*+-./01234",
    "56789:;<=>",
    "?@ABCDEFGH",
    "IJKLMNOPQR",
    "STUVWXYZ[]",
    "^,_@`abcde",
    "fghijklmno",
    "pqrstuvwxy",
    "z{|}~     "
  };

  SAF_drawText(allChars[(SAF_frame() >> 5) % 10],1,50,SAF_COLOR_BLACK,1);

  char num[] = "xxx";

  SAF_drawText(SAF_intToStr(saveValue,num),
    SAF_drawText("saved:",1,58,SAF_COLOR_BLACK,1),58,SAF_COLOR_BLACK,1);
}

uint8_t img[44] = {
0x07,0x06,0xff,0x00,0x00,0xff,0x00,0x00,0xff,0x00,0xed,0xed,0x00,0xed,0xe0,0x00,
0x00,0xed,0xe0,0xed,0xe0,0x60,0x00,0xff,0x00,0xe0,0xe0,0x60,0x00,0xff,0xff,0xff,
0x00,0x60,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0xff,0xff,0xff};

uint8_t imgCompressed[227] = {
0x13,0x20,0xe0,0x00,0x28,0x7c,0x07,0xfe,0x4c,0xff,0x71,0x70,0x95,0x6d,0x92,0x24,
0x48,0x49,0x60,0x41,0xd0,0x01,0x2d,0x01,0xc0,0x01,0x0f,0x08,0x0a,0x1c,0x01,0xa0,
0x01,0x08,0x3a,0x15,0x01,0x90,0x01,0x59,0x06,0x01,0xa0,0x06,0x05,0x27,0x05,0x06,
0xa0,0x06,0x15,0x27,0x15,0x06,0x90,0x06,0x15,0x27,0x15,0x06,0x70,0x11,0x38,0x0e,
0x08,0x2a,0x11,0x30,0x11,0x0f,0x08,0x7a,0x08,0x1c,0x11,0x00,0x01,0x3f,0x3b,0x38,
0x1b,0x2c,0x11,0x3d,0xa2,0x06,0x02,0x01,0x00,0x11,0x0d,0x16,0x0e,0x12,0x0e,0x08,
0x09,0x1e,0x08,0x06,0x11,0x30,0x01,0x02,0x69,0x18,0x11,0x70,0x06,0x05,0x47,0x05,
0x06,0x90,0x06,0x15,0x27,0x15,0x06,0xa0,0x16,0x25,0x16,0xd0,0x02,0x03,0x02,0x80,
0x34,0x20,0x02,0x03,0x02,0x10,0x43,0x00,0x14,0x10,0x14,0x10,0x02,0x0a,0x02,0x10,
0x13,0x10,0x13,0x14,0x10,0x14,0x10,0x02,0x0e,0x02,0x10,0x13,0x10,0x13,0x14,0x10,
0x14,0x10,0x01,0x02,0x01,0x10,0x43,0x00,0x54,0x10,0x01,0x02,0x01,0x10,0x13,0x10,
0x13,0x14,0x10,0x14,0x00,0x01,0x1c,0x0a,0x01,0x00,0x13,0x10,0x13,0x14,0x10,0x14,
0x00,0x01,0x22,0x01,0x00,0x43,0x80,0x01,0x02,0x01,0xf0,0x01,0x02,0x01,0xf0,0x01,
0x02,0x01,0xe0,0x01,0x1b,0x0c,0x01,0xb0,0x11,0x3b,0x0c,0x11,0x80,0x01,0x82,0x01,
0x70,0xa1,0x30};

uint8_t img1Bit[25] = {
0x0c,0x0f,0xe9,0x7c,0x03,0x40,0x20,0x00,0x80,0x11,0xf8,0x96,0x9d,0x69,0xcf,0x3a,
0x01,0x90,0x1c,0xeb,0xc1,0xbe,0x07,0xe3,0xf0};

uint8_t img1BitMask[25] = {
0x0c,0x0f,0x16,0x83,0xfc,0xbf,0xdf,0xff,0x7f,0xef,0xff,0x7f,0xe3,0xfe,0x3f,0xc7,
0xfe,0x7f,0xe3,0xfc,0x3f,0xc1,0xf8,0x1c,0x00};
  
uint8_t imgLogo[2 + 8 * 8];

int8_t trianglePoints[6] = {1,2,3,4,5,6};

void screen3()
{
  SAF_clearScreen(SAF_COLOR_YELLOW);

  int8_t s = SAF_sin(SAF_frame());
  int8_t c = SAF_cos(SAF_frame());

  int8_t a = 32 - s / 2, b = 32 + s / 2; 

  SAF_drawLine(a,32 - 64,b,32 + 64,SAF_COLOR_RED);
  SAF_drawLine(32 - 64,b,32 + 64,a,SAF_COLOR_RED);

  uint8_t transform = SAF_TRANSFORM_NONE + (SAF_frame() >> 4) % 4;
 
  if ((SAF_frame() >> 3) % 2)
    transform |= SAF_TRANSFORM_FLIP;

  SAF_drawImage(img,
    32 + s / 4 - img[0] / 2,
    32 + c / 4 - img[1] / 2,
    transform | SAF_TRANSFORM_SCALE_3 | ((SAF_frame() & 0x10) ? 0  : SAF_TRANSFORM_INVERT),SAF_COLOR_WHITE);

  SAF_drawImageCompressed(imgCompressed,1,1,transform,SAF_COLOR_RED);
  SAF_drawImage(img,40,1,transform,SAF_COLOR_WHITE); 
  SAF_drawImage1Bit(img1Bit,40,20,img1BitMask,255,0,transform);
  
  SAF_drawImage1Bit(img1Bit,50,45,0,255,0,SAF_TRANSFORM_NONE);
  
  SAF_drawImage1Bit(imgLogo,2,40,imgLogo,SAF_COLOR_BLUE,255,SAF_TRANSFORM_NONE);
  
  for (int i = 0; i < 6; ++i)
  {
    int8_t add = SAF_random() > 127 ? 1 : -1;

    if ((add > 0 && trianglePoints[i] < 16) || (add < 0 && trianglePoints[i] > -16))
      trianglePoints[i] += add;
  }

  for (uint8_t i = 0; i < 3; ++i)
  {
    uint8_t i1 = 2 * i, i2 = 2 * (i + 1) % 6;
  
    SAF_drawLine(
      trianglePoints[i1] + 25,
      trianglePoints[i1 + 1] + 40,
      trianglePoints[i2] + 25,
      trianglePoints[i2 + 1] + 40,
      SAF_COLOR_BLACK);
  }

}

uint8_t phase = 0;

void SAF_init(void)
{
  saveValue = SAF_load(0);
  saveValue++;
  SAF_save(0,saveValue);

imgLogo[0] = 8;
imgLogo[1] = 8;

imgLogo[2] = (SAF_LOGO_IMAGE >> 56) & 0xff;
imgLogo[3] = (SAF_LOGO_IMAGE >> 48) & 0xff;
imgLogo[4] = (SAF_LOGO_IMAGE >> 40) & 0xff;
imgLogo[5] = (SAF_LOGO_IMAGE >> 32) & 0xff;
imgLogo[6] = (SAF_LOGO_IMAGE >> 24) & 0xff;
imgLogo[7] = (SAF_LOGO_IMAGE >> 16) & 0xff;
imgLogo[8] = (SAF_LOGO_IMAGE >> 8) & 0xff;
imgLogo[9] = SAF_LOGO_IMAGE & 0xff;


}

uint8_t SAF_loop(void)
{
#if 0 
screen2();
#else

  switch (phase)
  {
    case 0: screen1(); break;
    case 1: screen2(); break;
    case 2: screen3(); break;
    default: break;
  }

  uint32_t f = SAF_frame() % 64;

  if (f >= 24 && f < 35)
  {
    SAF_drawRect(3,56,45,6,SAF_COLOR_GRAY_DARK,1);

    uint8_t i = (SAF_frame() / 64) % SAF_SOUNDS;

    char s[8] = "sound x";

    s[6] = '0' + i;

    SAF_drawText(s,4,57,SAF_COLOR_RED,1);

    if (f == 24)
      SAF_playSound(i);
  }

  if (SAF_frame() != 0 && SAF_frame() % 128 == 0)
  {
    phase++;
    phase %= 3;
  }

  return 1;
#endif
}

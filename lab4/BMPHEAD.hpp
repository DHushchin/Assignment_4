#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;


class BMPHEAD {
public:
    int8_t id[2];            // Завжди дві літери 'B' і 'M'
    int32_t filesize;        // Розмір файла в байтах +
    int16_t reserved[2];
    int32_t headersize;      // 54L для 24-бітних зображень
    int32_t infoSize;        // 40L для 24-бітних зображень
    int32_t width;           // ширина зображення в пікселях +
    int32_t height;           // висота зображення в пікселях +
    int16_t biPlanes;        // 1 (для 24-бітних зображень)
    int16_t bits;            // 24 (для 24-бітних зображень)
    int32_t biCompression;   // 0L
    int32_t biSizeImage;     // Можна поставити в 0L для зображень без компрессії (наш варіант)
    int32_t biXPelsPerMeter; // Рекомендована кількість пікселів на метр, можна 0L
    int32_t biYPelsPerMeter; // Те саме, по висоті
    int32_t biClrUsed;       // Для індексованих зображень, можна поставити 0L
    int32_t biClrImportant;  // Те саме
public:
    int32_t getWidth();
    int32_t getHeight();
    BMPHEAD(const BMPHEAD& h);
    void ProcessHead(double extent, string name1, string name2);
    BMPHEAD();
};


struct PIXELDATA {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    PIXELDATA(){
        uint8_t red = 0, green = 0, blue = 0;
    }
    PIXELDATA(uint8_t red, uint8_t green, uint8_t blue) {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
};


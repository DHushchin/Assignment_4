#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class BMPHEAD {
public:
    int8_t id[2];
    int32_t filesize;
    int16_t reserved[2];
    int32_t headersize;
    int32_t infoSize;
    int32_t width;
    int32_t height;
    int16_t biPlanes;
    int16_t bits;
    int32_t biCompression;
    int32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    int32_t biClrUsed;
    int32_t biClrImportant;
public:
    BMPHEAD(const BMPHEAD& h);
    void ProcessHead(double extent, string name1, string name2);
    BMPHEAD();
};

struct PIXELDATA {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    PIXELDATA() {
        red = 0, green = 0, blue = 0;
    }
    PIXELDATA(uint8_t red, uint8_t green, uint8_t blue) {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
};

class Image {
private:
    BMPHEAD Head;
    PIXELDATA* Color;
public:      
    void ResizeImage(string name1, string name2, double extent);
    void ProcessPixels(double extent, string name1, string name2);
    void ReadPixels(string address, int oldHeight, int oldWidth);
    void WritePixels(const string& address) const;
    void Interpolation(double coef, int oldHeight, int oldWidth);
};

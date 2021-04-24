#pragma once
#include <cstdint>
using namespace std;
#pragma pack(push, 1)
struct PIXELDATA {
private:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
public:
    PIXELDATA() {
        red = 0;
        green = 0;
        blue = 0;
    }
    uint8_t getRed();
    uint8_t getGreen();
    uint8_t getBlue();
    void setRed(uint8_t red);
    void setGreen(uint8_t green);
    void setBlue(uint8_t blue);
};
#pragma pack(pop)
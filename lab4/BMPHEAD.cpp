#include "BMPHEAD.hpp"

int32_t BMPHEAD::getWidth() {
    return width;
}

int32_t BMPHEAD::getHeight() {
    return height;
}

void BMPHEAD::setWidth(double extent) {
    width *= extent;
}

void BMPHEAD::setHeight(double extent) {
    height *= extent;
}

void BMPHEAD::setFilesize(int ZeroBytes) {
    filesize = 54 + (height * width) * 3 + ZeroBytes * width;
}
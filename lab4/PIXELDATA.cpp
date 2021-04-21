#include "PIXELDATA.hpp"

using namespace std;

uint8_t PIXELDATA::getRed() {
	return red;
}
uint8_t PIXELDATA::getGreen() {
	return green;
}
uint8_t PIXELDATA::getBlue() {
	return blue;
}

void PIXELDATA::setRed(uint8_t red) {
    this->red = red;
}

void PIXELDATA::setGreen(uint8_t green) {
    this->green = green;
}

void PIXELDATA::setBlue(uint8_t blue) {
    this->blue = blue;
}
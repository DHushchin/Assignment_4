#pragma once
#include <fstream>
#include "BMPHEAD.hpp"
#include "PIXELDATA.hpp"

using namespace std;

class Image {
private:
    BMPHEAD Head;
    PIXELDATA Color;
public:      
    void ResizeImage(string name1, string name2, double extent);
    PIXELDATA** CreateMatrix(int32_t height, int32_t width); 
    void ProcessPixels(double extent, string name1, string name2);
    PIXELDATA** Interpolation(PIXELDATA** InitialMatrix, double extent);
    void WritePixels(PIXELDATA** ResultMatrix, string name2);
    void ReadPixels(double extent, string name1, PIXELDATA** InitialMatrix);
};

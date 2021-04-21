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
    void ResizeImage(char* name1, char* name2, double extent);
    void ProcessHead(double extent, char* name1, char* name2);
    PIXELDATA** CreateMatrix(int32_t height, int32_t width); 
    void ProcessPixels(double extent, char* name1, char* name2);
    PIXELDATA** Interpolation(PIXELDATA** InitialMatrix, double extent);
    void WritePixels(PIXELDATA** ResultMatrix, char* name2);
    void ReadPixels(double extent, char* name1, PIXELDATA** InitialMatrix);
};

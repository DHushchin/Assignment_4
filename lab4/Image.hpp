#pragma once
#include <fstream>
#include "BMPHEAD.hpp"

using namespace std;

class Image {
private:
    BMPHEAD Head;
    PIXELDATA Color;
public:      
    void ResizeImage(string name1, string name2, int extent);
    PIXELDATA** CreateMatrix(int height, int width); 
    void ProcessPixels(int extent, string name1, string name2);
    void Interpolation(PIXELDATA** InitialMatrix, int extent, PIXELDATA** ResultMatrix);
    void WritePixels(PIXELDATA** ResultMatrix, string name2);
    void ReadPixels(int extent, string name1, PIXELDATA** InitialMatrix);
    void WriteInteger(PIXELDATA** InitialMatrix, string name2, int extent);
};

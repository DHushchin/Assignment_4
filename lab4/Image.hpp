#pragma once
#include "BMPHEAD.hpp"
#include "PIXELDATA.hpp"

using namespace std;

class Image {
private:
    BMPHEAD Head;
    PIXELDATA Color;
public:       
    void ResizeImage(char* name1, char* name2, double extent);
    void ProcessHead(ifstream& FileIn, ofstream& FileOut, double extent);

};

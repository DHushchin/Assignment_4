#pragma once
#include <fstream>
#include "BMPHEAD.hpp"

using namespace std;

class Image {
private:
    BMPHEAD Head;
    PIXELDATA* data;
public:      
    void ResizeImage(string name1, string name2, int extent);
    void ProcessPixels(int extent, string name1, string name2);
    void readFromFile(string address);
    void readAline(ifstream& file, long& proceeded_pixels, int delta);
    void writeToFile(const string& address) const;
    void writeLine(ofstream& file, long& proceeded_pixels) const;
    void enlargeBImage(double coef);
    long getNumberOfPixels() const {
        return Head.width * Head.height;
    }

    void createADataArray() {
        long size = this->getNumberOfPixels();
        data = new PIXELDATA[size];
    }
};

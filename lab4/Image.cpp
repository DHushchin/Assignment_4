#pragma once

#include "Image.hpp"

using namespace std;


void Image::ResizeImage(string name1, string name2, double extent) {
    bool negative = false; 
    if (extent < 0) {
        extent *= -1;
        negative = true;
    }
    Head.ProcessHead(extent, name1, name2);
    ProcessPixels(extent, name1, name2, negative);
}


void Image::ProcessPixels(double extent, string name1, string name2, bool negative) {
    int oldHeight = (int)Head.height / extent;
    int oldWidth = (int)Head.width / extent;
    ReadPixels(name1, oldHeight, oldWidth);
    Interpolation(extent, oldHeight, oldWidth);
    WritePixels(name2, negative);
}


void Image::ReadPixels(string address, int oldHeight, int oldWidth) {

    Color = new PIXELDATA[oldHeight * oldWidth];

    ifstream file(address, ios::in | ios::binary);
    file.seekg(54, ios::beg);

    long proceeded_pixels = 0;
    int delta = 4 - ((oldWidth * 3) % 4);
    for (int counter = 0; counter < oldHeight; ++counter) {
        for (int counter = 0; counter < oldWidth; ++counter) {
            PIXELDATA tmp;
            file.read((char*)&tmp, sizeof(PIXELDATA));
            Color[proceeded_pixels++] = tmp;
        }

        long tmp;
        file.read((char*)&tmp, delta);
    }

    file.close();
}


void Image::WritePixels(const string& address, bool negative) {
    int delta = 4 - ((Head.width * 3) % 4);
    ofstream file(address, ios::out | ios::binary | ios::app);
    if (negative) ReverseArray();
    long proceeded_pixels = 0;
    for (int i = 0; i < Head.height; i++) {
        for (int counter = 0; counter < Head.width; ++counter) {
            PIXELDATA tmp = Color[proceeded_pixels++];
            file.write((char*)&tmp, sizeof(PIXELDATA));
        }
        for (int j = 0; j < delta; j++) {
            uint8_t zero = 0x00;
            file.write((char*)&zero, sizeof(uint8_t));
        }
    }
    file.close();
}


void Image::Interpolation(double extent, int oldHeight, int oldWidth) {
    int IntWidth = (int)(Head.width);
    int IntHeight = (int)(Head.height);
    PIXELDATA* newImage = new PIXELDATA[IntWidth * IntHeight];
    for (int x = 0; x < IntWidth; x++) {
        for (int y = 0; y < IntHeight - 1; y++) {
            double X_Double = (x / extent);
            double Y_Double = (y / extent);
            int X_Int = (int)(floor(x / extent));
            int Y_Int = (int)(floor(y / extent));
            double dx = X_Double - X_Int;
            double dy = Y_Double - Y_Int;
            PIXELDATA color00 = Color[X_Int * oldWidth + Y_Int];
            PIXELDATA color10 = Color[(X_Int + 1) * oldWidth + Y_Int];
            PIXELDATA color01 = Color[(X_Int + 1) * oldWidth + Y_Int + 1];
            PIXELDATA color11 = Color[X_Int * oldWidth + (Y_Int + 1)];
            int Red = (int)(round(color00.red * (1 - dx) * (1 - dy) + color10.red * dx * (1 - dy) + color11.red * (1 - dx) * dy + color01.red * dx * dy));
            int Green = (int)(round(color00.green * (1 - dx) * (1 - dy) + color10.green * dx * (1 - dy) + color11.green * (1 - dx) * dy + color01.green * dx * dy));
            int Blue = (int)(round(color00.blue * (1 - dx) * (1 - dy) + color10.blue * dx * (1 - dy) + color11.blue * (1 - dx) * dy + color01.blue * dx * dy));
            PIXELDATA NewColor;
            NewColor.red = Red;
            NewColor.blue = Blue;
            NewColor.green = Green;
            newImage[x * IntWidth + y] = NewColor;
        }
    }
    Head.width = IntWidth;
    Head.height = IntHeight;
    Color = newImage;
}


BMPHEAD::BMPHEAD() {
    id[0] = 'B';
    id[1] = 'M';
    filesize = 0;
    reserved[0] = 0;
    reserved[1] = 0;
    headersize = 0;
    infoSize = 0;
    width = 0;
    height = 0;
    biPlanes = 1;
    bits = 24;
    biCompression = 0;
    biSizeImage = 0;
    biXPelsPerMeter = 0;
    biYPelsPerMeter = 0;
    biClrUsed = 0;
    biClrImportant = 0;
}

BMPHEAD::BMPHEAD(const BMPHEAD& other) {
    id[0] = other.id[0];
    id[1] = other.id[1];
    filesize = other.filesize;
    reserved[0] = other.reserved[0];
    reserved[1] = other.reserved[1];
    headersize = other.headersize;
    infoSize = other.infoSize;
    width = other.width;
    height = other.height;
    biPlanes = other.biPlanes;
    bits = other.bits;
    biCompression = other.biCompression;
    biSizeImage = other.biSizeImage;
    biXPelsPerMeter = other.biXPelsPerMeter;
    biYPelsPerMeter = other.biYPelsPerMeter;
    biClrUsed = other.biClrUsed;
    biClrImportant = other.biClrImportant;
}

void BMPHEAD::ProcessHead(double extent, string name1, string name2) {
    ifstream FileIn(name1, ios::binary);
    ofstream FileOut(name2, ios::out | ios::binary);
    if (!FileIn.is_open()) cout << "Can't open to read ProcessHead 1" << endl;
    else {

        FileIn.read((char*)&id, sizeof(id));
        FileIn.read((char*)&filesize, sizeof(filesize));
        FileIn.read((char*)&reserved, sizeof(reserved));
        FileIn.read((char*)&headersize, sizeof(headersize));
        FileIn.read((char*)&infoSize, sizeof(infoSize));
        FileIn.read((char*)&width, sizeof(width));
        FileIn.read((char*)&height, sizeof(height));
        FileIn.read((char*)&biPlanes, sizeof(biPlanes));
        FileIn.read((char*)&bits, sizeof(bits));
        FileIn.read((char*)&biCompression, sizeof(biCompression));
        FileIn.read((char*)&biSizeImage, sizeof(biSizeImage));
        FileIn.read((char*)&biXPelsPerMeter, sizeof(biXPelsPerMeter));
        FileIn.read((char*)&biYPelsPerMeter, sizeof(biYPelsPerMeter));
        FileIn.read((char*)&biClrUsed, sizeof(biClrUsed));
        FileIn.read((char*)&biClrImportant, sizeof(biClrImportant));
    }
    width *= extent;
    height *= extent;
    //filesize = 54 + (height * width) * 3 + (4 - width % 4) * width;

    if (!FileOut) cout << "Can't open to write ProcessHead 2" << endl;
    else {
        FileOut.write((char*)&id, sizeof(id));
        FileOut.write((char*)&filesize, sizeof(filesize));
        FileOut.write((char*)&reserved, sizeof(reserved));
        FileOut.write((char*)&headersize, sizeof(headersize));
        FileOut.write((char*)&infoSize, sizeof(infoSize));
        FileOut.write((char*)&width, sizeof(width));
        FileOut.write((char*)&height, sizeof(height));
        FileOut.write((char*)&biPlanes, sizeof(biPlanes));
        FileOut.write((char*)&bits, sizeof(bits));
        FileOut.write((char*)&biCompression, sizeof(biCompression));
        FileOut.write((char*)&biSizeImage, sizeof(biSizeImage));
        FileOut.write((char*)&biXPelsPerMeter, sizeof(biXPelsPerMeter));
        FileOut.write((char*)&biYPelsPerMeter, sizeof(biYPelsPerMeter));
        FileOut.write((char*)&biClrUsed, sizeof(biClrUsed));
        FileOut.write((char*)&biClrImportant, sizeof(biClrImportant));
    }
    FileIn.close();
    FileOut.close();
}

void Image::ReverseArray() {
    long proceeded_pixels = 0;
    vector <PIXELDATA> vect;

    for (int i = 0; i < Head.height; i++) {
        for (int j = 0; j < Head.width; ++j) {
            vect.push_back(Color[proceeded_pixels++]);
        }
    }

    for (int i = 0; i < vect.size(); i++) {
        PIXELDATA tmp = vect[i];
        vect[i] = vect[vect.size() - i - 1];
        vect[vect.size() - i - 1] = tmp;
    }

    proceeded_pixels = 0;
    for (int i = 0; i < Head.height; i++) {
        for (int j = 0; j < Head.width; ++j) {
            Color[proceeded_pixels] = vect[vect.size() - 1 - proceeded_pixels];
            proceeded_pixels++;
        }
    }
}



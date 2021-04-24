#pragma once
#include "Image.hpp"
#include <iostream>
#include <fstream>
#include <iostream>
using namespace std;



void Image::ResizeImage(string name1, string name2, double extent) {
    Head.ProcessHead(extent, name1, name2);
    ProcessPixels(extent, name1, name2);
}

PIXELDATA** Image::CreateMatrix(int32_t height, int32_t width) {
    PIXELDATA** Matrix = new PIXELDATA*[height];
    for (int i = 0; i < height; i++) {
        Matrix[i] = new PIXELDATA[width];
    }
    return Matrix;
}


void Image::ReadPixels(double extent, string name1, PIXELDATA** InitialMatrix) {

    ifstream FileIn(name1, ios::in | ios::binary);
    int ZeroBytes = 4 - ((Head.getWidth() * 3) % 4);
    FileIn.seekg(54, ios::beg);
    for (int i = 0; i < Head.getHeight()/extent; i++) {
        for (int j = 0; j < Head.getWidth()/extent; j++) {
            if (!FileIn.is_open()) cout << "Can't open to read ReadPixels 1" << endl;
            else FileIn.read((char*)&InitialMatrix[i][j], sizeof(PIXELDATA));
        }
        long tmp;
        if (!FileIn) cout << "Can't open to read ReadPixels 2" << endl;
        else FileIn.read((char*)&tmp, ZeroBytes);
    }
    FileIn.close();
}


PIXELDATA** Image::Interpolation(PIXELDATA** InitialMatrix, double extent) {
    PIXELDATA** ResultMatrix = CreateMatrix(Head.getHeight(), Head.getWidth());

    for (int i = 0; i < Head.getWidth(); i++) {
        for (int j = 0; j < Head.getHeight() - 1; j++) {

            double i_new = (i / extent);
            double j_new = (j / extent);

            int i_old = (int)(floor(i / extent));
            int j_old = (int)(floor(j / extent));

            double deltaX = i_new - i_old;
            double deltaY = j_new - j_old;

            PIXELDATA UpLeft = InitialMatrix[i_old * Head.getWidth()][j_old];
            PIXELDATA DownLeft = InitialMatrix[(i_old + 1) * Head.getWidth()][j_old];
            PIXELDATA DownRight = InitialMatrix[(i_old + 1) * Head.getWidth()][j_old + 1];
            PIXELDATA UpRight = InitialMatrix[i_old * Head.getWidth()][j_old + 1];

            int Red = (int)(round(UpLeft.getRed() * (1 - deltaX) * (1 - deltaY) +
                DownLeft.getRed() * deltaX * (1 - deltaY) +
                UpRight.getRed() * (1 - deltaX) * deltaY +
                DownRight.getRed() * deltaX * deltaY));

            int Green = (int)(round(UpLeft.getGreen() * (1 - deltaX) * (1 - deltaY) +
                DownLeft.getGreen() * deltaX * (1 - deltaY) +
                UpRight.getGreen() * (1 - deltaX) * deltaY +
                DownRight.getGreen() * deltaX * deltaY));

            int Blue = (int)(round(UpLeft.getBlue() * (1 - deltaX) * (1 - deltaY) +
                DownLeft.getBlue() * deltaX * (1 - deltaY) +
                UpRight.getBlue() * (1 - deltaX) * deltaY +
                DownRight.getBlue() * deltaX * deltaY));

            PIXELDATA newPixel;
            newPixel.setRed(Red);            
            newPixel.setGreen(Green);
            newPixel.setBlue(Blue);
            ResultMatrix[i * Head.getWidth()][j] = newPixel;
        }
    }
    return ResultMatrix;
}


void Image::WritePixels(PIXELDATA** ResultMatrix, string name2) {
    ofstream FileOut(name2, ios::out | ios::binary | ios::app);
    int ZeroBytes = 4 - ((Head.getWidth() * 3) % 4);
    for (int i = 0; i < Head.getHeight(); i++) {
        for (int j = 0; j < Head.getWidth(); ++j) {
            if (!FileOut) cout << "Can't open to write WritePixels 1" << endl;
            else FileOut.write((char*)&ResultMatrix[i][j], sizeof(PIXELDATA));
        }
        for (int k = 0; k < ZeroBytes; k++) {
            int8_t zero = 0;
            if (!FileOut) cout << "Can't open to write WritePixels 2" << endl;
            else FileOut.write((char*)&zero, sizeof(int8_t));
        }
    }
    FileOut.close();
}


void Image::ProcessPixels(double extent, string name1, string name2) {
    PIXELDATA** InitialMatrix = CreateMatrix(Head.getHeight()/extent, Head.getWidth()/extent);
    ReadPixels(extent, name1, InitialMatrix);
    PIXELDATA** ResultMatrix = Interpolation(InitialMatrix, extent);
    WritePixels(ResultMatrix, name2);
}
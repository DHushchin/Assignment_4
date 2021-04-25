#pragma once
#include "Image.hpp"
#include <iostream>
#include <fstream>
#include <iostream>
using namespace std;


void Image::ResizeImage(string name1, string name2, int extent) {
    Head.ProcessHead(extent, name1, name2);
    ProcessPixels(extent, name1, name2);
}


PIXELDATA** Image::CreateMatrix(int height, int width) {
    PIXELDATA** Matrix = new PIXELDATA*[height];
    for (int i = 0; i < height; i++) {
        Matrix[i] = new PIXELDATA[width];
    }
    return Matrix;
}


void Image::ReadPixels(int extent, string name1, PIXELDATA** InitialMatrix) {

    ifstream FileIn(name1, ios::in | ios::binary);
    int ZeroBytes = (4 - Head.getWidth() * sizeof(PIXELDATA) % 4) % 4;
    FileIn.seekg(54, ios::beg);
    for (int i = 0; i < Head.getHeight(); i++) {
        for (int j = 0; j < Head.getWidth(); j++) {
            if (!FileIn.is_open()) cout << "Can't open to read ReadPixels 1" << endl;
            else FileIn.read((char*)&InitialMatrix[i][j], sizeof(PIXELDATA));
        }
        long tmp;
        if (!FileIn.is_open()) cout << "Can't open to read ReadPixels 2" << endl;
        else FileIn.read((char*)&tmp, ZeroBytes);
    }
    FileIn.close();
}


void Image::Interpolation(PIXELDATA** InitialMatrix, int extent, PIXELDATA** ResultMatrix) {


    for (int i = 0; i < Head.getWidth() - 1; i++) {
        for (int j = 0; j < Head.getHeight() - 1; j++) {

            double i_new = (i / extent);
            double j_new = (j / extent);

            int i_old = (int)(floor(i / extent));
            int j_old = (int)(floor(j / extent));

            double deltaX = i_new - i_old;
            double deltaY = j_new - j_old;

            PIXELDATA UpLeft = InitialMatrix[i_old][j_old];
            PIXELDATA DownLeft = InitialMatrix[i_old + 1][j_old];
            PIXELDATA DownRight = InitialMatrix[i_old + 1][j_old + 1];
            PIXELDATA UpRight = InitialMatrix[i_old][j_old + 1];

            int Red = (int)(round(UpLeft.red * (1 - deltaX) * (1 - deltaY) +
                DownLeft.red * deltaX * (1 - deltaY) +
                UpRight.red * (1 - deltaX) * deltaY +
                DownRight.red * deltaX * deltaY));

            int Green = (int)(round(UpLeft.green * (1 - deltaX) * (1 - deltaY) +
                DownLeft.green * deltaX * (1 - deltaY) +
                UpRight.green * (1 - deltaX) * deltaY +
                DownRight.green * deltaX * deltaY));

            int Blue = (int)(round(UpLeft.blue * (1 - deltaX) * (1 - deltaY) +
                DownLeft.blue * deltaX * (1 - deltaY) +
                UpRight.blue * (1 - deltaX) * deltaY +
                DownRight.blue) * deltaX * deltaY);

            PIXELDATA newPixel;
            newPixel.red = Red;            
            newPixel.green = Green;
            newPixel.blue = Blue;
            ResultMatrix[i][j] = newPixel;
        }
    }
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


void Image::ProcessPixels(int extent, string name1, string name2) {
    PIXELDATA** InitialMatrix = CreateMatrix(Head.getHeight()/extent, Head.getWidth()/extent);
    ReadPixels(extent, name1, InitialMatrix);
    //PIXELDATA** ResultMatrix = CreateMatrix(Head.getHeight(), Head.getWidth());
    //Interpolation(InitialMatrix, extent, ResultMatrix);
    //WritePixels(ResultMatrix, name2);
    WriteInteger(InitialMatrix, name2, extent);
}

void Image::WriteInteger(PIXELDATA** InitialMatrix, string name2, int extent) {

    ofstream FileOut(name2, ios::out | ios::binary | ios::app);
    int ZeroBytes = (4 - Head.getWidth() * sizeof(PIXELDATA) % 4) % 4;
    Head.height *= extent;
    Head.width *= extent;
    for (size_t i = 0; i < Head.height; i++)
    {
        //for (int m = 0; m < extent; m++) {
            for (size_t j = 0; j < Head.width; j++)
            {
                if (!FileOut) cout << "Can't open to write. WriteInteger1";
                else FileOut.write((char*)&InitialMatrix[i / extent][j /extent], sizeof(PIXELDATA));

                for (size_t k = 0; k < ZeroBytes; k++)
                {
                    long zero = 0;
                    if (!FileOut) cout << "Can't open to write WriteInteger2" << endl;
                    else FileOut.write((char*)&ZeroBytes, sizeof(int8_t));
                }
            }
        //}
    }

    FileOut.close();
}
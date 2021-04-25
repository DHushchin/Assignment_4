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


void Image::ProcessPixels(int extent, string name1, string name2) {
    readFromFile(name1);
    enlargeBImage(extent);
    writeToFile(name2);
}



void Image::readFromFile(string address) {
    //Создаем массив, в который будем заносить информацию о пикселях
    this->createADataArray();

    //Открытие файла и пропуск Header'а
    ifstream file(address, ios::in | ios::binary);
    file.seekg(54, ios::beg);

    //Поочередно обрабатываем каждый пиксель
    long proceeded_pixels = 0;
    int delta = 4 - ((Head.width * 3) % 4);
    for (int counter = 0; counter < Head.height; ++counter) {
        this->readAline(file, proceeded_pixels, delta);
    }

    //Закрытие файла
    file.close();
}

//TODO: Возможно, стоит обьединить с readAFile
void Image::readAline(ifstream& file, long& proceeded_pixels, int delta) {
    for (int counter = 0; counter < Head.width; ++counter) {

        //Чтение цвета пикселя
        uint8_t red;
        file.read((char*)&red, sizeof(uint8_t));

        uint8_t green;
        file.read((char*)&green, sizeof(uint8_t));

        uint8_t blue;
        file.read((char*)&blue, sizeof(uint8_t));

        //Запись информации о пикселе в массив
        data[proceeded_pixels] = PIXELDATA(red, green, blue);
        proceeded_pixels++;
    }

    long tmp;
    file.read((char*)&tmp, delta);
}

void Image::writeToFile(const string& address) const {
    int delta = 4 - ((Head.width * 3) % 4);
    ofstream file(address, ios::out | ios::binary | ios::app);
    long proceeded_pixels = 0;
    for (int i = 0; i < Head.height; i++) {
        this->writeLine(file, proceeded_pixels);
        for (int j = 0; j < delta; j++) {
            uint8_t zero = 0;
            file.write((char*)&zero, sizeof(uint8_t));
        }
    }
    file.close();
}

void Image::writeLine(ofstream& file, long& proceeded_pixels) const {
    for (int counter = 0; counter < Head.width; ++counter) {
        //Запись цвета пикселя
        uint8_t red = data[proceeded_pixels].red;
        file.write((char*)&red, sizeof(uint8_t));
        uint8_t green = data[proceeded_pixels].green;
        file.write((char*)&green, sizeof(uint8_t));
        uint8_t blue = data[proceeded_pixels].blue;
        file.write((char*)&blue, sizeof(uint8_t));
        proceeded_pixels++;
    }
}


void Image::enlargeBImage(double coef) {
    int newWidth = (int)(coef * Head.width);
    int newHeight = (int)(coef * Head.height);

    PIXELDATA* newImage = new PIXELDATA[newWidth * newHeight];
    for (int x = 0; x < newWidth; x++) {
        for (int y = 0; y < newHeight - 1; y++) {

            double xDouble = (x / coef);
            double yDouble = (y / coef);

            int oldX = (int)(floor(x / coef));
            int oldY = (int)(floor(y / coef));

            double deltaX = xDouble - oldX;
            double deltaY = yDouble - oldY;

            //соседние пиксели
            PIXELDATA pixel1 = data[oldX * Head.width + oldY];
            PIXELDATA pixel2 = data[(oldX + 1) * Head.width + oldY];
            PIXELDATA pixel3 = data[(oldX + 1) * Head.width + oldY + 1];
            PIXELDATA pixel4 = data[oldX * Head.width + (oldY + 1)];

            int R = (int)(round(pixel1.red * (1 - deltaX) * (1 - deltaY) +
                pixel2.red * deltaX * (1 - deltaY) +
                pixel4.red * (1 - deltaX) * deltaY +
                pixel3.red * deltaX * deltaY));

            int G = (int)(round(pixel1.green * (1 - deltaX) * (1 - deltaY) +
                pixel2.green * deltaX * (1 - deltaY) +
                pixel4.green * (1 - deltaX) * deltaY +
                pixel3.green * deltaX * deltaY));

            int B = (int)(round(pixel1.blue * (1 - deltaX) * (1 - deltaY) +
                pixel2.blue * deltaX * (1 - deltaY) +
                pixel4.blue * (1 - deltaX) * deltaY +
                pixel3.blue * deltaX * deltaY));

            PIXELDATA newPixel;
            newPixel.red = R;
            newPixel.blue = B;
            newPixel.green = G;
            newImage[x * newWidth + y] = newPixel;
        }
    }
    Head.width = newWidth;
    Head.height = newHeight;
    data = newImage;
}

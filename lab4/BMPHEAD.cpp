#include "BMPHEAD.hpp"

using namespace std;

int32_t BMPHEAD::getWidth() {
    return width;
}

int32_t BMPHEAD::getHeight() {
    return height;
}

BMPHEAD::BMPHEAD(const BMPHEAD & h) {
    id[0] = h.id[0];
    id[1] = h.id[1];
    filesize = h.filesize;
    reserved[0] = h.reserved[0];
    reserved[1] = h.reserved[1];
    headersize = h.headersize;
    infoSize = h.infoSize;
    width = h.width;
    height = h.height;
    biPlanes = h.biPlanes;
    bits = h.bits;
    biCompression = h.biCompression;
    biSizeImage = h.biSizeImage;
    biXPelsPerMeter = h.biXPelsPerMeter;
    biYPelsPerMeter = h.biYPelsPerMeter;
    biClrUsed = h.biClrUsed;
    biClrImportant = h.biClrImportant;
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
    filesize = 54 + (height * width) * 3 + (4 - width % 4) * width;

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

BMPHEAD::BMPHEAD() {
    id[0] = 'B';     // Завжди дві літери 'B' і 'M'
    id[1] = 'M';
    filesize = 0;        // Розмір файла в байтах +
    reserved[0] = 0;     // 0, 0
    reserved[1] = 0;     // 0, 0
    headersize = 0;      // 54L для 24-бітних зображень
    infoSize = 0;        // 40L для 24-бітних зображень
    width = 0;           // ширина зображення в пікселях +
    height = 0;           // висота зображення в пікселях +
    biPlanes = 1;        // 1 (для 24-бітних зображень)
    bits = 24;            // 24 (для 24-бітних зображень)
    biCompression = 0;   // 0L
    biSizeImage = 0;     // Можна поставити в 0L для зображень без компрессії (наш варіант)
    biXPelsPerMeter = 0; // Рекомендована кількість пікселів на метр, можна 0L
    biYPelsPerMeter = 0; // Те саме, по висоті
    biClrUsed = 0;       // Для індексованих зображень, можна поставити 0L
    biClrImportant = 0;  // Те саме
}
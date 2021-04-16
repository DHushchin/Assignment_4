#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

#pragma pack(1)
typedef struct {
    int8_t id[2];            // Завжди дві літери 'B' і 'M'
    int32_t filesize;        // Розмір файла в байтах +
    int16_t reserved[2];     // 0, 0
    int32_t headersize;      // 54L для 24-бітних зображень
    int32_t infoSize;        // 40L для 24-бітних зображень
    int32_t width;           // ширина зображення в пікселях +
    int32_t height;           // висота зображення в пікселях +
    int16_t biPlanes;        // 1 (для 24-бітних зображень)
    int16_t bits;            // 24 (для 24-бітних зображень)
    int32_t biCompression;   // 0L
    int32_t biSizeImage;     // Можна поставити в 0L для зображень без компрессії (наш варіант)
    int32_t biXPelsPerMeter; // Рекомендована кількість пікселів на метр, можна 0L
    int32_t biYPelsPerMeter; // Те саме, по висоті
    int32_t biClrUsed;       // Для індексованих зображень, можна поставити 0L
    int32_t biClrImportant;  // Те саме
} BMPHEAD;
#pragma pack(pop)

typedef struct {
    int8_t redComp;
    int8_t greenComp;
    int8_t blueComp;
} PIXELDATA;


class Image {
private:
    BMPHEAD Head;
    PIXELDATA Color;
public:
    int32_t getWidth();
    int32_t getHeight();
    void CopyImage(char* name1, char* name2);
};


int main(int argc, char* argv[]) {
    Image image;
    image.CopyImage(argv[1], argv[2]);
    system("pause");
    return 0;
}

int32_t Image::getWidth() {
    return Head.width;
}

int32_t Image::getHeight() {
    return Head.height;
}

void Image::CopyImage(char* name1, char* name2) {
#pragma warning (disable : 4996)
    FILE* infile = fopen(name1, "rb");
#pragma warning (disable : 4996)
    FILE* outfile = fopen(name2, "wb");

    if (!infile) {
        cout << "Can't open to read" << endl;
    }
    else fread(&Head, sizeof(BMPHEAD), 1, infile);

    if (!outfile) {
        cout << "Can't open to write" << endl;
    }
    else fwrite(&Head, sizeof(BMPHEAD), 1, outfile);


    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {

            fread(&Color, sizeof(PIXELDATA), 1, infile);

            fwrite(&Color, sizeof(PIXELDATA), 1, outfile);
        }
    }
    fclose(infile);
    fclose(outfile);
}
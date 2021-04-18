#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

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
    void ReadHead(FILE* FileIn, FILE* FileOut, int extent, int ZeroPixels);
    void ReadPixels(FILE* FileIn, FILE* FileOut, int ZeroPixels, int extent);
    void ResizeImage(char* name1, char* name2, int extent);
};

int main(int argc, char* argv[]) {
    Image image;
    image.ResizeImage(argv[1], argv[2], stoi(argv[3]));
    system("pause");
    return 0;
}

int32_t Image::getWidth() {
    return Head.width;
}

int32_t Image::getHeight() {
    return Head.height;
}

void Image::ReadHead(FILE* FileIn, FILE* FileOut, int extent, int ZeroPixels) {
    if (!FileIn) cout << "Can't open to read" << endl;
    else fread(&Head, sizeof(Head), 1, FileIn);

    Head.width *= extent;
    Head.height *= extent;

    if (!FileOut) cout << "Can't open to write" << endl;
    else fwrite(&Head, sizeof(Head), 1, FileOut);
}

void Image::ReadPixels(FILE* FileIn, FILE* FileOut, int ZeroPixels, int extent) {
    for (int i = 0; i < abs(Head.height); i++) {
        for (size_t m = 0; m < extent; m++) {
            for (int j = 0; j < Head.width; j++) {
                fread(&Color, sizeof(Color), 1, FileIn);
                for (int l = 0; l < extent; l++)
                    fwrite(&Color, sizeof(Color), 1, FileOut);
            }
            fseek(FileIn, ZeroPixels, SEEK_CUR);
            for (int k = 0; k < ZeroPixels; k++) {
                fputc(0x00, FileOut);
            }
            fseek(FileIn, -(Head.width * 3 + ZeroPixels), SEEK_CUR);
        }
        fseek(FileIn, (Head.width * 3 + ZeroPixels), SEEK_CUR);
    }
}


void Image::ResizeImage(char* name1, char* name2, int extent) {
#pragma warning (disable : 4996)
    FILE* FileIn = fopen(name1, "rb");
#pragma warning (disable : 4996)
    FILE* FileOut = fopen(name2, "wb");

    int ZeroBytes = (4 - (Head.width * sizeof(PIXELDATA)) % 4) % 4;
    ReadHead(FileIn, FileOut, extent, ZeroBytes);
    ReadPixels(FileIn, FileOut, ZeroBytes, extent);
    fclose(FileIn);
    fclose(FileOut);
}
#include <iostream>
#include <fstream>
#include <vector>
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
    int8_t redComponent;
    int8_t greenComponent;
    int8_t blueComponent;
} PIXELDATA;


int main(int argc, char* argv[]) {
    char* name1 = argv[1];
    char* name2 = argv[2];
    
    BMPHEAD Head;
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

    PIXELDATA pixel;
    for (int i = 0; i < Head.height; i++) {
        for (int j = 0; j < Head.width; j++) {

             fread(&pixel, sizeof(PIXELDATA), 1, infile);

             fwrite(&pixel, sizeof(PIXELDATA), 1, outfile);
        }
    }
    fclose(infile);
    fclose(outfile);
    system("pause");
    return 0;
}
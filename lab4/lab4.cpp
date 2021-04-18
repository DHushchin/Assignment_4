#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace std;

#pragma pack(1)
class BMPHEAD
{
private:
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
public:
    int32_t getWidth();
    int32_t getHeight();
    int32_t GetFilesize();
    void Extent(int extent);
} ;
#pragma pack(pop)

class PIXELDATA
{
    int8_t redComp;
    int8_t greenComp;
    int8_t blueComp;
};


class Image 
{
private:
    BMPHEAD Head;
    PIXELDATA Color;
    PIXELDATA** PixelM;
public:
    void ReadHead(FILE* FileIn, FILE* FileOut, int extent);
    void ReadPixels(FILE* FileIn, FILE* FileOut, int extent);
    void ResizeImage(char* name1, char* name2, int extent);
    void ConvertToMatrix(FILE* FileIn);
    void WritePixels(FILE* FileIn, FILE* FileOut, int extent);
};

int main(int argc, char* argv[]) 
{
    Image image;
    string str;
    double size = atof(argv[3]);
    if (size == (int)size)
    {
        image.ResizeImage(argv[1], argv[2], stoi(argv[3]));
    }
    else
    {
        // это для интерполяции

    }

    // for test
    /*char name1[] = { 'b','m','p','.','b','m','p', '\0' };
    char name2[] = { 'b','m','p', '1','.','b','m','p', '\0' } ;
    int name3 = 2;
    image.ResizeImage(name1, name2, name3);*/

    system("pause");
    return 0;
}

int32_t BMPHEAD::getWidth()
{
    return width;
}

int32_t BMPHEAD::getHeight()
{
    return height;
}

int32_t BMPHEAD::GetFilesize()
{
    return filesize;
}

void BMPHEAD::Extent(int extent) {
    int ZeroBytes = (4 - (width * sizeof(PIXELDATA)) % 4) % 4;
    width *= extent;
    height *= extent;
    filesize = 54 + height * width * 3 + ZeroBytes * height;
}



void Image::ReadHead(FILE* FileIn, FILE* FileOut, int extent) 
{
    if (!FileIn) cout << "Can't open to read" << endl;
    else fread(&Head, sizeof(Head), 1, FileIn);
    Head.Extent(extent);
    if (!FileOut) cout << "Can't open to write" << endl;
    else fwrite(&Head, sizeof(Head), 1, FileOut);
}

void Image::ReadPixels(FILE* FileIn, FILE* FileOut, int extent) 
{
    int ZeroBytes = (4 - (Head.getWidth() * sizeof(PIXELDATA)) % 4) % 4;
    /*
    for (int i = 0; i < abs(Head.getHeight()); i++)
    {
            for (int j = 0; j < Head.getWidth(); j++)
            {
                fread(&Color, sizeof(Color), 1, FileIn);
            }
            fseek(FileIn, ZeroBytes, SEEK_CUR);
            for (int j = 0; j < Head.getWidth(); j++)
            {
                fwrite(&Color, sizeof(Color), 1, FileOut);
            }
            for (int k = 0; k < ZeroBytes; k++) 
            {
                fputc(0x00, FileOut);
            }
            //fseek(FileIn, -(Head.getWidth() * 3 + ZeroBytes), SEEK_CUR);
       // fseek(FileIn, (Head.getWidth() * 3 + ZeroBytes), SEEK_CUR);       
    }
    */
}

void Image::ConvertToMatrix(FILE* FileIn)
{
    int ZeroBytes = (4 - (Head.getWidth() * sizeof(PIXELDATA)) % 4) % 4;
    PixelM = new PIXELDATA*[Head.getHeight()];
    for (size_t i = 0; i < Head.getHeight(); i++)
    {
        PixelM[i] = new PIXELDATA[Head.getWidth()];
        for (size_t j = 0; j < Head.getWidth(); j++) {
            fread(&PixelM[i][j], sizeof(PIXELDATA), 1, FileIn);
        }
        fseek(FileIn, ZeroBytes, SEEK_CUR);
    }
}

void Image::WritePixels(FILE* FileIn, FILE* FileOut, int extent)
{
    int ZeroBytes = (4 - (Head.getWidth() * sizeof(PIXELDATA)) % 4) % 4;
    for (int i = 0; i < Head.getHeight(); i++)
    {
        for (int j = 0; j < Head.getWidth(); j++) 
        {
            fwrite(&PixelM[i / extent][j / extent], sizeof(PIXELDATA), 1, FileOut);
        }
        for (int k = 0; k < ZeroBytes; k++) 
        {
            fputc(0x00, FileOut);
        }
    }
}


void Image::ResizeImage(char* name1, char* name2, int extent) 
{
#pragma warning (disable : 4996)
    FILE* FileIn = fopen(name1, "rb");
#pragma warning (disable : 4996)
    FILE* FileOut = fopen(name2, "wb");


    ReadHead(FileIn, FileOut, extent);
    //ReadPixels(FileIn, FileOut, extent);
    ConvertToMatrix(FileIn);
    WritePixels(FileIn, FileOut, extent);
    fclose(FileIn);
    fclose(FileOut);
}


#include "Image.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    Image image;
    string name1 = "bmp.bmp";
    string name2 = "new.bmp";
    int ext = 1;
    image.ResizeImage(name1, name2, ext);
    system("pause");
    return 0;
}
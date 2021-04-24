#include "Image.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    Image image;
    string name1 = "test.bmp";
    string name2 = "new.bmp";
    double ext = 2.0;
    image.ResizeImage(name1, name2, ext);
    system("pause");
    return 0;
}
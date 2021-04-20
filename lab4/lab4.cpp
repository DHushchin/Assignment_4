#include "Image.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    Image image;
    image.ResizeImage(argv[1], argv[2], stod(argv[3]));
    system("pause");
    return 0;
}




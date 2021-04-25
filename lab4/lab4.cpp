#include "Image.hpp"

using namespace std;

int main() {
    Image image;
    string name1 = "bmp.bmp";
    string name2 = "new.bmp";
    double ext = 1.89;
    image.ResizeImage(name1, name2, ext);
    system("pause");
    return 0;
}
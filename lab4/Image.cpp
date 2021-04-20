#include "Image.hpp"
#include <iostream>
#include <fstream>

using namespace std;

void Image::ProcessHead(ifstream& FileIn, ofstream& FileOut, double extent) {
    if (!FileIn) cout << "Can't open to read" << endl;
    else FileIn.read((char*)&Head, sizeof(Head));
    Head.setWidth(extent);
    Head.setHeight(extent);
    Head.setFilesize(4 - (Head.getWidth() % 4));
    if (!FileOut) cout << "Can't open to read" << endl;
    else FileOut.write((char*)&Head, sizeof(Head));
}


void Image::ResizeImage(char* name1, char* name2, double extent) {
    ifstream FileIn(name1, ios::in | ios::binary);
    ofstream FileOut(name2, ios::in | ios::binary);
    ProcessHead(FileIn, FileOut, extent);




    FileIn.close();
    FileOut.close();
}


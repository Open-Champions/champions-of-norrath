#include "ctor.cpp"

int main() {
    int object[4];
    for (int& word : object) word = 0x44444444;
    __9VICSFFile(object);
    if (object[0] != 0 || object[1] != 0) return 1;
    if (object[2] != 0x44444444 || object[3] != 0x44444444) return 2;
    return 0;
}

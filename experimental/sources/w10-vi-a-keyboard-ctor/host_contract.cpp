#include "ctor.cpp"

int main() {
    int object[4];
    for (int& word : object) word = 0x33333333;
    __10VIKeyboard(object);
    if (object[1] != -1) return 1;
    if (object[0] != 0x33333333 || object[2] != 0x33333333 || object[3] != 0x33333333) return 2;
    return 0;
}

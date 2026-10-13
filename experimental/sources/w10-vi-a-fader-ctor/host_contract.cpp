#include "ctor.cpp"

int main() {
    int object[10];
    for (int& word : object) word = 0x5a5a5a5a;
    __7VIFader(object);
    for (int index = 0; index < 10; ++index) {
        int expected = index == 5 ? -1 : 0x5a5a5a5a;
        if (object[index] != expected) return 1 + index;
    }
    return 0;
}

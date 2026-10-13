#include "ctor.cpp"

#include <cstring>

int main() {
    unsigned object[6];
    for (unsigned& word : object) word = 0xa5a5a5a5u;
    __6VIFont(object);
    for (int index = 0; index < 4; ++index)
        if (object[index] != 0x3f800000u) return 1 + index;
    if (object[4] != 0xa5a5a5a5u || object[5] != 0xa5a5a5a5u) return 5;
    return 0;
}

#include "ctor.cpp"

#include <cstring>

int main() {
    unsigned char object[1100];
    std::memset(object, 0xa5, sizeof object);
    __10VIESFParse(object);
    for (int offset = 0; offset < 1100; ++offset) {
        unsigned char expected = 0xa5;
        if (offset == 0) expected = 1;
        else if (offset < 16) expected = 0;
        else if (offset < 20) expected = 0xa5;  // offset 16 is not written
        else if (offset < 40) expected = 0;
        else if (offset < 1064) expected = (offset & 3) == 3 ? 0xff : 0;
        if (object[offset] != expected) return 1 + offset % 7;
    }
    return 0;
}

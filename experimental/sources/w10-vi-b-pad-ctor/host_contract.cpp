#include "pad_ctor.cpp"

#include <cstring>

int main() {
    unsigned char object[64];
    std::memset(object, 0xa5, sizeof object);
    __5VIPad(reinterpret_cast<VIPad*>(object));
    for (int i = 0; i < 64; ++i) {
        bool cleared = (i >= 0 && i < 8) || (i >= 52 && i < 56);
        if (cleared ? object[i] != 0 : object[i] != 0xa5) return 1;
    }
    return 0;
}

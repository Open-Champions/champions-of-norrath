#include "set_time.cpp"

#include <cstring>

int main() {
    alignas(8) unsigned char object[1200];
    std::memset(object, 0x5a, sizeof object);
    if (SetTime__7VIScenel(reinterpret_cast<VIScene*>(object), 0x0102030405060708ll) != 0) return 1;
    long long stored;
    std::memcpy(&stored, object + 1152, 8);
    if (stored != 0x0102030405060708ll) return 2;
    for (int i = 0; i < 1200; ++i) {
        if ((i < 1152 || i >= 1160) && object[i] != 0x5a) return 3;
    }
    return 0;
}

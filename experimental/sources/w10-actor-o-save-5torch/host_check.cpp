// Host contract for msg_save__5TorchPc; not an EE input. Includes the source.
#include "msg_save.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    for (int v : {0, 1, 2, 3, 127, 128, 255}) {
        alignas(4) unsigned char object[160];
        alignas(4) unsigned char buffer[16];
        std::memset(object, 0x5a, sizeof object);
        std::memset(buffer, 0xa5, sizeof buffer);
        object[138] = static_cast<unsigned char>(v);
        if (msg_save__5TorchPc(object, reinterpret_cast<char*>(buffer)) != 4) return 1;
        int w; std::memcpy(&w, buffer, 4);
        if (w != static_cast<signed char>(v)) return 2;
        for (int i = 4; i < 16; ++i) if (buffer[i] != 0xa5) return 3;
        for (int i = 0; i < 160; ++i) if (i != 138 && object[i] != 0x5a) return 4;
    }
    return 0;
}

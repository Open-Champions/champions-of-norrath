// Host contract for msg_save__7ShooterPc; not an EE input. Includes the source.
#include "msg_save.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    for (unsigned v : {0u, 1u, 0x7fffu, 0x8000u, 0xffffu, 0x1234u}) {
        alignas(4) unsigned char object[128];
        alignas(4) unsigned char buffer[16];
        std::memset(object, 0x5a, sizeof object);
        std::memset(buffer, 0xa5, sizeof buffer);
        unsigned short h = static_cast<unsigned short>(v);
        std::memcpy(object + 82, &h, 2);
        if (msg_save__7ShooterPc(object, reinterpret_cast<char*>(buffer)) != 2) return 1;
        unsigned short out; std::memcpy(&out, buffer, 2);
        if (out != h) return 2;
        for (int i = 2; i < 16; ++i) if (buffer[i] != 0xa5) return 3;
        for (int i = 0; i < 128; ++i) if ((i < 82 || i > 83) && object[i] != 0x5a) return 4;
    }
    return 0;
}

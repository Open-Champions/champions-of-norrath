// Host contract for msg_load__5TorchPc; not an EE input. Includes the source.
#include "msg_load.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    for (unsigned word : {0u, 1u, 0x100u, 0xff00u, 0x80u, 0xffu, 0xdeadbe00u, 0xdeadbeefu}) {
        for (unsigned flags : {0u, 0x201u, 0xfffffdfeu, 0xffffffffu, 0x200u, 0x1u}) {
            alignas(4) unsigned char object[160];
            alignas(4) unsigned char buffer[8];
            std::memset(object, 0x5a, sizeof object);
            std::memcpy(object + 12, &flags, 4);
            std::memcpy(buffer, &word, 4);
            if (msg_load__5TorchPc(object, reinterpret_cast<char*>(buffer)) != 4) return 1;
            unsigned got; std::memcpy(&got, object + 12, 4);
            unsigned want = (flags & ~0x201u) | ((word & 0xffu) ? 0x200u : 0x1u);
            if (got != want) return 2;
            if (object[138] != (word & 0xffu)) return 3;
            for (int i = 0; i < 160; ++i) if (i != 138 && (i < 12 || i >= 16) && object[i] != 0x5a) return 4;
        }
    }
    return 0;
}

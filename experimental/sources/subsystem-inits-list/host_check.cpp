// Host contract for listInit__Fv. Not an EE plan input.
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void listInit__Fv();
extern "C" unsigned char nodes[6096 * 20] = {};
extern "C" unsigned char* freeNodes = nullptr;

namespace {
unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) |
           (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) |
           (static_cast<unsigned>(slot[3]) << 24);
}

unsigned low_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
}

int main() {
    const auto base = reinterpret_cast<std::uintptr_t>(nodes);
    const auto end = base + sizeof nodes;
    if ((base >> 32) != (end >> 32)) {
        std::fprintf(stderr, "fail: nodes crosses a 4GB boundary\n");
        return 1;
    }
    std::memset(nodes, 0xa5, sizeof nodes);
    freeNodes = nullptr;
    listInit__Fv();
    if (freeNodes != nodes) {
        std::fprintf(stderr, "fail: freeNodes\n");
        return 1;
    }
    for (int index = 0; index < 6095; ++index) {
        const unsigned char* slot = nodes + index * 20;
        if (load_word(slot) != low_bits(nodes + (index + 1) * 20)) {
            std::fprintf(stderr, "fail: link %d\n", index);
            return 1;
        }
        for (int byte = 4; byte < 20; ++byte) {
            if (slot[byte] != 0xa5) {
                std::fprintf(stderr, "fail: payload %d\n", index);
                return 1;
            }
        }
    }
    if (load_word(nodes + 6095 * 20) != 0) {
        std::fprintf(stderr, "fail: terminator\n");
        return 1;
    }
    for (int byte = 4; byte < 20; ++byte) {
        if (nodes[6095 * 20 + byte] != 0xa5) {
            std::fprintf(stderr, "fail: terminator payload\n");
            return 1;
        }
    }
    return 0;
}

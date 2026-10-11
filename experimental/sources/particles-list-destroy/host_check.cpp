// Host contract for listDestroy__FP8ListHead. Not an EE plan input.
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void listDestroy__FP8ListHead(void* head);
extern "C" unsigned char* freeNodes = nullptr;
extern "C" unsigned long long norrath_particles_host_base = 0;

namespace {
alignas(16) unsigned char arena[0x400];

unsigned read_word(unsigned offset) {
    const unsigned char* slot = arena + offset;
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

void write_word(unsigned offset, unsigned value) {
    unsigned char* slot = arena + offset;
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

void fail(const char* message) {
    std::fprintf(stderr, "fail: %s\n", message);
}

void reset() {
    std::memset(arena, 0, sizeof arena);
    norrath_particles_host_base = reinterpret_cast<unsigned long long>(arena);
    freeNodes = arena + 0x20;
}
}

int main() {
    unsigned char head[4];

    reset();
    std::memset(head, 0, sizeof head);
    listDestroy__FP8ListHead(head);
    if (freeNodes != arena + 0x20) {
        fail("empty head stored freeNodes");
        return 1;
    }

    reset();
    write_word(0x100, 0);
    write_word(0x104, 0);
    write_word(0x108, 0);
    write_word(0, 0x100);
    listDestroy__FP8ListHead(arena);
    if (freeNodes != arena + 0x100 || read_word(0x100) != 0x20u) {
        fail("single node free list");
        return 1;
    }

    reset();
    write_word(0x100, 0x140);
    write_word(0x140, 0);
    write_word(0, 0x100);
    listDestroy__FP8ListHead(arena);
    if (freeNodes != arena + 0x140 || read_word(0x140) != 0x100u || read_word(0x100) != 0x20u) {
        fail("two-node chain");
        return 1;
    }
    if (read_word(0x144) != 0u) {
        fail("second node previous");
        return 1;
    }

    reset();
    write_word(0x200, 0x100);
    write_word(0x100, 0x140);
    write_word(0x104, 0x200);
    write_word(0x140, 0);
    write_word(0x144, 0x100);
    write_word(0, 0x100);
    listDestroy__FP8ListHead(arena);
    if (read_word(0x200) != 0u) {
        fail("previous still linked");
        return 1;
    }
    if (freeNodes != arena + 0x140 || read_word(0x140) != 0x100u || read_word(0x100) != 0x20u) {
        fail("unlinked chain free list");
        return 1;
    }

    reset();
    write_word(0x100, 0);
    write_word(0x108, 0x180);
    write_word(0x180, 0x1c0);
    write_word(0x184, 0x220);
    write_word(0x1c0, 0);
    write_word(0x1c4, 0x180);
    write_word(0x220, 0x180);
    write_word(0, 0x100);
    listDestroy__FP8ListHead(arena);
    if (read_word(0x220) != 0x1c0u || read_word(0x1c4) != 0x220u) {
        fail("twin still linked");
        return 1;
    }
    if (freeNodes != arena + 0x100 || read_word(0x100) != 0x180u || read_word(0x180) != 0x20u) {
        fail("twin pushed before node");
        return 1;
    }

    reset();
    write_word(0x100, 0x104);
    write_word(0x104, 0x200);
    write_word(0x108, 0x300);
    std::memset(arena + 0x300, 0x5a, 16);
    write_word(0, 0x100);
    listDestroy__FP8ListHead(arena);
    for (int byte = 0; byte < 16; ++byte) {
        if (arena[0x300 + byte] != 0x5a) {
            fail("stale twin was pushed");
            return 1;
        }
    }
    if (freeNodes != arena + 0x20) {
        fail("aliased walk free head");
        return 1;
    }
    return 0;
}

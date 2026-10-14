// Host comparison only. Not an EE build input. Links against
// rtti_user.cpp compiled for the host.
#include <cstdio>
#include <cstring>

// Host stand-in for the separately authored 48-byte vtable view.
extern "C" const unsigned char norrath_legacy_user_type_info_vtable_view[48] = {};
extern "C" void norrath_rtti_user(void* typeinfo, const char* name);

static unsigned read_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

int main() {
    // A null typeinfo pointer must store nothing and must not crash.
    norrath_rtti_user(nullptr, "10GameObject");
    unsigned char storage[12];
    std::memset(storage, 0x5a, sizeof storage);
    const char name[] = "10GameObject";
    norrath_rtti_user(storage, name);
    if (read_word(storage) !=
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(name))) {
        std::printf("name word was not stored\n");
        return 1;
    }
    if (read_word(storage + 4) !=
        static_cast<unsigned>(
            reinterpret_cast<__UINTPTR_TYPE__>(norrath_legacy_user_type_info_vtable_view))) {
        std::printf("vtable word was not stored\n");
        return 2;
    }
    // Only the first two words are written; the rest keeps its old bytes.
    for (int i = 8; i < 12; ++i) {
        if (storage[i] != 0x5a) {
            std::printf("byte %d was clobbered\n", i);
            return 3;
        }
    }
    return 0;
}

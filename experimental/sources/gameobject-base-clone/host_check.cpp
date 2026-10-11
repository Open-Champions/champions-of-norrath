#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void* norrath_vt_clone_10GameObject(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};

static int failures = 0;
static unsigned char* buffer = nullptr;
static int requests = 0;
static bool return_null = false;
static std::vector<void*> heads;
static bool saw_vtable_before_first_head = false;
static bool saw_uninitialized_head = true;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}

extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    requests = requested_size;
    if (return_null) return nullptr;
    return buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    (void)source;
    auto* dest = static_cast<unsigned char*>(destination);
    if (heads.empty()) {
        saw_vtable_before_first_head =
            read_word(buffer + 72) ==
            static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_game_object_vtable_view));
    }
    const unsigned prior = read_word(dest);
    if (prior != 0xa5a5a5a5u) saw_uninitialized_head = false;
    dest[0] = dest[1] = dest[2] = dest[3] = 0;
    heads.push_back(destination);
}

int main() {
    unsigned char source[76];
    unsigned char created[76];
    for (unsigned i = 0; i < 76u; ++i) source[i] = static_cast<unsigned char>(0x10 + i);
    source[0] = 0x11;
    source[4] = 0x22;
    source[8] = 0x33;
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    return_null = false;
    heads.clear();
    saw_uninitialized_head = true;
    void* result = norrath_vt_clone_10GameObject(source);
    expect(result == created, "returns the allocation");
    expect(requests == 76, "allocates 76 bytes");
    expect(heads.size() == 3u && heads[0] == created && heads[1] == created + 4 && heads[2] == created + 8,
           "copy-constructs the three heads");
    expect(saw_vtable_before_first_head, "vtable is stored before the first head call");
    expect(saw_uninitialized_head, "heads are not copied from the source before being cleared");
    bool fields = true;
    for (unsigned i = 0; i < 12u; ++i)
        if (created[i] != 0) fields = false;
    for (unsigned i = 12; i < 42u; ++i)
        if (created[i] != source[i]) fields = false;
    if (created[42] != 0xa5 || created[43] != 0xa5) fields = false;
    for (unsigned i = 44; i < 58u; ++i)
        if (created[i] != source[i]) fields = false;
    if (created[58] != 0xa5 || created[59] != 0xa5) fields = false;
    for (unsigned i = 60; i < 72u; ++i)
        if (created[i] != source[i]) fields = false;
    expect(fields, "copies the payload and leaves the four gap bytes");
    expect(read_word(created + 72) ==
               static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_game_object_vtable_view)),
           "new vtable is the GameObject vtable");

    return_null = true;
    heads.clear();
    expect(norrath_vt_clone_10GameObject(source) == nullptr, "null allocation returns null");
    expect(heads.empty(), "null allocation does not construct heads");
    return failures == 0 ? 0 : 1;
}

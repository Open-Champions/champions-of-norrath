#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void* clone__7Counter(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" unsigned char norrath_next_view_449160[8] = {};

static int failures = 0;
static unsigned char* buffer = nullptr;
static int requests = 0;
static bool return_null = false;
static std::vector<void*> heads;
static bool saw_game_vtable = false;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    requests = requested_size;
    if (return_null) return nullptr;
    return buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    (void)source;
    auto* dest = static_cast<unsigned char*>(destination);
    if (heads.empty()) saw_game_vtable = read_word(buffer + 72) == low_pointer(norrath_game_object_vtable_view);
    dest[0] = dest[1] = dest[2] = dest[3] = 0;
    heads.push_back(destination);
}

int main() {
    unsigned char source[80];
    unsigned char created[80];
    for (unsigned i = 0; i < 80u; ++i) source[i] = static_cast<unsigned char>(0x10 + i);
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    return_null = false;
    heads.clear();
    void* result = clone__7Counter(source);
    expect(result == created, "returns the allocation");
    expect(requests == 80, "allocates 80 bytes");
    expect(heads.size() == 3u && heads[0] == created && heads[1] == created + 4 && heads[2] == created + 8,
           "copy-constructs the three heads");
    expect(saw_game_vtable, "the GameObject vtable is stored before the first head");
    bool fields = true;
    for (unsigned i = 0; i < 12u; ++i) if (created[i] != 0) fields = false;
    for (unsigned i = 12; i < 42u; ++i) if (created[i] != source[i]) fields = false;
    if (created[42] != 0xa5 || created[43] != 0xa5) fields = false;
    for (unsigned i = 44; i < 58u; ++i) if (created[i] != source[i]) fields = false;
    if (created[58] != 0xa5 || created[59] != 0xa5) fields = false;
    for (unsigned i = 60; i < 72u; ++i) if (created[i] != source[i]) fields = false;
    for (unsigned i = 76; i < 80u; ++i) if (created[i] != source[i]) fields = false;
    expect(fields, "copies the payload and leaves the gap bytes");
    expect(read_word(created + 72) == low_pointer(norrath_next_view_449160), "final vtable");
    return_null = true;
    heads.clear();
    expect(clone__7Counter(source) == nullptr, "null allocation returns null");
    expect(heads.empty(), "null allocation does not construct heads");
    return failures == 0 ? 0 : 1;
}

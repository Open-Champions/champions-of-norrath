#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void* norrath_vt_clone_12CosmeticProp(void* source);
extern "C" unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" unsigned char norrath_factory_cosmetic_vtable[8] = {};

static int failures = 0;
static unsigned char* buffer = nullptr;
static int requests = 0;
static bool return_null = false;
static std::vector<void*> heads;
static std::vector<const void*> head_sources;
static unsigned vtable_at_first_head = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned pointer_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}

extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    requests = requested_size;
    if (return_null) return nullptr;
    return buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    if (heads.empty()) vtable_at_first_head = read_word(buffer + 72);
    auto* dest = static_cast<unsigned char*>(destination);
    dest[0] = dest[1] = dest[2] = dest[3] = 0;
    heads.push_back(destination);
    head_sources.push_back(source);
}

int main() {
    unsigned char source[96];
    unsigned char created[96];
    for (unsigned i = 0; i < 96u; ++i) source[i] = static_cast<unsigned char>(0x10 + (i & 0xff));
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    return_null = false;
    heads.clear();
    head_sources.clear();
    void* result = norrath_vt_clone_12CosmeticProp(source);
    expect(result == created, "returns the allocation");
    expect(requests == 96, "allocation size");
    expect(heads.size() == 3u && heads[0] == created && heads[1] == created + 4 && heads[2] == created + 8,
           "copy-constructs the three heads");
    expect(head_sources.size() == 3u && head_sources[0] == source && head_sources[1] == source + 4 &&
               head_sources[2] == source + 8,
           "head sources are the three source heads");
    expect(vtable_at_first_head == pointer_word(norrath_game_object_vtable_view),
           "GameObject vtable is visible to the first head");
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
    expect(fields, "copies the shared payload and leaves the four gap bytes");
    expect(read_word(created + 72) == pointer_word(norrath_factory_cosmetic_vtable), "final vtable is the class vtable");
    bool tail = true;
    for (unsigned i = 76; i < 96u; ++i)
        if (created[i] != source[i]) tail = false;
    expect(tail, "copies the cosmetic words through offset 95");
    unsigned char frozen[96];
    std::memcpy(frozen, created, 96);
    return_null = true;
    heads.clear();
    expect(norrath_vt_clone_12CosmeticProp(source) == nullptr, "null allocation returns null");
    expect(heads.empty(), "null allocation does not construct heads");
    expect(std::memcmp(created, frozen, 96) == 0, "null allocation does not write the previous object");
    return failures == 0 ? 0 : 1;
}

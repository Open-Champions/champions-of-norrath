// Host contract for findTagString__10GameObjectPci. Not an EE plan input.
// Pointer words are 32-bit, so the test memory lives in one static arena.
#include <cstdio>
#include <cstring>

extern "C" char* findTagString__10GameObjectPci(void* object, char* name, int index);
extern "C" unsigned char norrath_factory_ctype_minimum_view[384] = {0};

static unsigned char arena[4096];
static int failures = 0;
static int strlen_calls = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(arena);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
static unsigned arena_address(const void* p) {
    return static_cast<unsigned>(reinterpret_cast<unsigned long>(p));
}
static void put_word(unsigned offset, unsigned value) {
    std::memcpy(arena + offset, &value, 4);
}
extern "C" unsigned norrath_factory_strlen(const char* text) {
    ++strlen_calls;
    return static_cast<unsigned>(std::strlen(text));
}
extern "C" int norrath_factory_strncmp(const char* left, const char* right, unsigned count) {
    return std::strncmp(left, right, count);
}

int main() {
    for (int ch = 'A'; ch <= 'Z'; ++ch) norrath_factory_ctype_minimum_view[ch + 128] = 1;
    // object at arena+0 (tag word at +44), tag array at 256, strings from 512.
    std::memcpy(arena + 512, "name=alpha", 11);
    std::memcpy(arena + 544, "nameplate=skip", 15);
    std::memcpy(arena + 576, "name=beta", 10);
    std::memcpy(arena + 608, "other=1", 8);
    put_word(256, arena_address(arena + 544));
    put_word(260, arena_address(arena + 512));
    put_word(264, arena_address(arena + 608));
    put_word(268, arena_address(arena + 576));
    put_word(272, 0u);

    char query[16];
    std::strcpy(query, "NAME");
    expect(findTagString__10GameObjectPci(arena, query, 0) == nullptr, "null tag word returns null");
    expect(strlen_calls == 0, "null tag word returns before reading the name");
    expect(std::strcmp(query, "NAME") == 0, "null tag word leaves the name unchanged");

    put_word(44, arena_address(arena + 256));
    char* hit = findTagString__10GameObjectPci(arena, query, 0);
    expect(std::strcmp(query, "name") == 0, "query is lowercased in place");
    expect(hit == reinterpret_cast<char*>(arena + 512 + 5), "index 0 returns past the equals of the first hit");
    expect(hit != nullptr && std::strcmp(hit, "alpha") == 0, "first hit text");

    std::strcpy(query, "name");
    hit = findTagString__10GameObjectPci(arena, query, 1);
    expect(hit == reinterpret_cast<char*>(arena + 576 + 5), "index 1 returns the second hit");
    std::strcpy(query, "name");
    expect(findTagString__10GameObjectPci(arena, query, 2) == nullptr, "too few hits returns null");
    std::strcpy(query, "missing");
    expect(findTagString__10GameObjectPci(arena, query, 0) == nullptr, "absent name returns null");
    std::strcpy(query, "other");
    hit = findTagString__10GameObjectPci(arena, query, 0);
    expect(hit == reinterpret_cast<char*>(arena + 608 + 6), "a later entry is found");

    put_word(256, 0u);
    std::strcpy(query, "name");
    expect(findTagString__10GameObjectPci(arena, query, 0) == nullptr, "empty list returns null");
    if (failures != 0) return 1;
    std::printf("find-tag-string contract passed\n");
    return 0;
}

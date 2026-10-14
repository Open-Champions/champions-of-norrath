#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_factory_impure_pointer_word[4] = {};
extern "C" int norrath_format_string(char*, const char*, ...);
extern "C" int sprintf(char*, const char*, ...);

static int call_sprintf(char* destination, const char* format, const char* text) {
    auto* function = sprintf;
    __asm__ __volatile__("" : "+r"(function));
    return function(destination, format, text);
}

static char* g_buffer;
static int g_calls;
static unsigned g_flags;
static unsigned g_width;
static unsigned g_size;
static unsigned g_base;
static unsigned g_owner;
static unsigned g_cursor;
static const char* g_format;

extern "C" int norrath_factory_vfprintf(unsigned char* stream, const char* format, const unsigned char* cursor) {
    ++g_calls;
    auto word = [](const unsigned char* p) {
        return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
    };
    g_flags = unsigned(stream[12]) | (unsigned(stream[13]) << 8);
    g_width = word(stream + 8);
    g_size = word(stream + 20);
    g_base = word(stream + 16);
    g_owner = word(stream + 84);
    g_cursor = word(stream);
    g_format = format;
    if (cursor == nullptr || g_cursor != g_base) return -1;
    g_buffer[0] = 'O';
    g_buffer[1] = 'K';
    stream[0] = static_cast<unsigned char>(g_cursor + 2);
    stream[1] = static_cast<unsigned char>((g_cursor + 2) >> 8);
    stream[2] = static_cast<unsigned char>((g_cursor + 2) >> 16);
    stream[3] = static_cast<unsigned char>((g_cursor + 2) >> 24);
    return 2;
}

int main() {
    char storage[32];
    auto* buffer = storage;
    g_buffer = buffer;
    std::memset(buffer, 0xa5, 32);
    norrath_factory_impure_pointer_word[0] = 0x78;
    norrath_factory_impure_pointer_word[1] = 0x56;
    norrath_factory_impure_pointer_word[2] = 0x34;
    norrath_factory_impure_pointer_word[3] = 0x12;
    const unsigned destination = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(buffer));
    if (norrath_format_string(buffer, "%s.lmp", "town") != 2) return 2;
    if (g_calls != 1 || std::strcmp(g_format, "%s.lmp") != 0) return 3;
    if (g_flags != 0x208 || g_width != 0x7fffffffu || g_size != 0x7fffffffu) return 4;
    if (g_base != destination || g_owner != 0x12345678u) return 5;
    if (buffer[0] != 'O' || buffer[1] != 'K' || buffer[2] != 0 || buffer[3] != static_cast<char>(0xa5)) return 6;
    std::memset(buffer, 0xa5, 32);
    if (call_sprintf(buffer, "%s.lmp", "town") != 2) return 7;
    if (g_calls != 2 || g_flags != 0x208 || g_base != destination || g_owner != 0x12345678u) return 8;
    if (buffer[0] != 'O' || buffer[1] != 'K' || buffer[2] != 0 || buffer[3] != static_cast<char>(0xa5)) return 9;
    std::printf("format string cases passed\n");
    return 0;
}

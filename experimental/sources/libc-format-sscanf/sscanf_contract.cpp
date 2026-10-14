#include <cstdio>
#include <cstring>

extern "C" int sscanf(const char* input, const char* format, ...);
extern "C" unsigned char norrath_factory_impure_pointer_word[4] = {0x78, 0x56, 0x34, 0x12};

static const char* g_seen;
static unsigned g_strlen_calls;

extern "C" unsigned norrath_factory_strlen(const char* text) {
    ++g_strlen_calls;
    g_seen = text;
    return static_cast<unsigned>(std::strlen(text));
}

extern "C" int norrath_player_string_eof_read(unsigned char*, char*, int) { return 0; }

static int g_scan_result = 4;
static const char* g_format;
static unsigned g_flags;
static unsigned g_base;
static unsigned g_cursor;
static unsigned g_size;
static unsigned g_count;
static unsigned g_fn;
static unsigned g_owner;
static unsigned g_unget;
static unsigned g_cookie;
static int g_scans;
static const unsigned char* g_cursor_arg;

static unsigned word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}

extern "C" int norrath_player_stream_scan(unsigned char* stream, const char* format, const unsigned char* argument_cursor) {
    ++g_scans;
    g_format = format;
    g_cursor_arg = argument_cursor;
    g_flags = unsigned(stream[12]) | (unsigned(stream[13]) << 8);
    g_base = word(stream + 16);
    g_cursor = word(stream);
    g_size = word(stream + 20);
    g_count = word(stream + 4);
    g_fn = word(stream + 32);
    g_owner = word(stream + 84);
    g_unget = word(stream + 48);
    g_cookie = word(stream + 68);
    return g_scan_result;
}

int main() {
    int first = 0, second = 0, third = 0, fourth = 0;
    const char* text = "10.20.30.40";
    if (sscanf(text, "%d.%d.%d.%d", &first, &second, &third, &fourth) != 4) return 1;
    if (g_strlen_calls != 1 || g_seen != text || g_scans != 1) return 2;
    if (std::strcmp(g_format, "%d.%d.%d.%d") != 0 || g_cursor_arg == nullptr) return 3;
    if (g_flags != 4 || g_size != 11 || g_count != 11) return 4;
    if (g_base != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(text))) return 5;
    if (g_cursor != g_base) return 6;
    if (g_fn != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(
            reinterpret_cast<const void*>(norrath_player_string_eof_read)))) return 7;
    if (g_owner != 0x12345678u || g_unget != 0 || g_cookie != 0) return 8;
    g_scan_result = -1;
    if (sscanf("", "%d.%d.%d.%d", &first, &second, &third, &fourth) != -1) return 9;
    if (g_size != 0 || g_count != 0 || g_scans != 2) return 10;
    std::printf("sscanf cases passed\n");
    return 0;
}

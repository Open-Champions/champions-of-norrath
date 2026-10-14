#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_factory_impure_pointer_word[4] = {};
extern "C" int norrath_factory_stream_write_setup(unsigned char*);
extern "C" void norrath_host_note(const void*);

static int g_init_calls;
static int g_free_calls;
static unsigned g_freed;
static int g_make_calls;
static unsigned char* g_reent;

extern "C" void norrath_factory_streams_init(unsigned char* reent) {
    ++g_init_calls;
    reent[56] = 1;
}
extern "C" void norrath_player_free_reentrant(unsigned char*, unsigned char* allocation) {
    ++g_free_calls;
    g_freed = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(allocation));
}
extern "C" void norrath_player_stream_make_buffer(unsigned char* stream) {
    ++g_make_calls;
    stream[16] = 0x40;
    stream[12] = static_cast<unsigned char>(stream[12] | 0x8u);
    stream[20] = 64;
}

static unsigned word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void set_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned address_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}

static unsigned char* g_base;

static unsigned char* file() { return g_base; }

static void reset_file() {
    std::memset(file(), 0, 128);
    std::memset(g_reent, 0, 128);
    g_reent[56] = 1;
    set_word(norrath_factory_impure_pointer_word, address_of(g_reent));
    g_init_calls = g_free_calls = g_make_calls = 0;
    g_freed = 0;
}

int main() {
    g_base = new unsigned char[8192];
    g_reent = g_base + 4096;
    norrath_host_note(g_base);
    norrath_host_note(g_reent);
    norrath_host_note(g_base + 256);
    norrath_host_note(g_base + 320);
    norrath_host_note(g_base + 512);

    reset_file();
    set_word(file() + 12, 0);
    if (norrath_factory_stream_write_setup(file()) != -1) return 2;
    if (g_make_calls != 0 || g_init_calls != 0) return 3;

    reset_file();
    set_word(file() + 84, 0);
    g_reent[56] = 0;
    set_word(file() + 12, 0x8u);
    set_word(file() + 16, address_of(g_base + 256));
    set_word(file() + 20, 32);
    if (norrath_factory_stream_write_setup(file()) != 0) return 4;
    if (word(file() + 84) != address_of(g_reent) || g_init_calls != 1) return 5;
    if (word(file() + 8) != 32 || g_make_calls != 0) return 6;

    reset_file();
    set_word(file() + 84, address_of(g_reent));
    set_word(file() + 12, 0x8u);
    set_word(file() + 16, 0);
    set_word(file() + 20, 8);
    if (norrath_factory_stream_write_setup(file()) != 0) return 7;
    if (g_make_calls != 1 || word(file() + 8) != 64) return 8;

    reset_file();
    set_word(file() + 84, address_of(g_reent));
    set_word(file() + 12, 0x10u);
    set_word(file() + 16, address_of(g_base + 320));
    set_word(file() + 20, 16);
    if (norrath_factory_stream_write_setup(file()) != 0) return 9;
    if ((word(file() + 12) & 0x8u) == 0 || word(file() + 8) != 16) return 10;
    if (word(file() + 8) != 16 || g_free_calls != 0) return 11;

    reset_file();
    unsigned char* heap = g_base + 512;
    set_word(file() + 84, address_of(g_reent));
    set_word(file() + 12, 0x34u);
    set_word(file() + 4, 9);
    set_word(file() + 16, address_of(g_base + 320));
    set_word(file() + 20, 16);
    set_word(file() + 48, address_of(heap));
    if (norrath_factory_stream_write_setup(file()) != 0) return 12;
    if (g_free_calls != 1 || g_freed != address_of(heap)) return 13;
    if (word(file() + 48) != 0 || word(file() + 4) != 0) return 14;
    if ((word(file() + 12) & 0x24u) != 0 || (word(file() + 12) & 0x8u) == 0) return 15;

    reset_file();
    set_word(file() + 84, address_of(g_reent));
    set_word(file() + 12, 0x14u);
    set_word(file() + 16, address_of(g_base + 320));
    set_word(file() + 20, 16);
    set_word(file() + 48, address_of(file() + 64));
    if (norrath_factory_stream_write_setup(file()) != 0) return 16;
    if (g_free_calls != 0 || word(file() + 48) != 0) return 17;
    if ((word(file() + 12) & 0x4u) != 0) return 18;

    reset_file();
    set_word(file() + 84, address_of(g_reent));
    set_word(file() + 12, 0x9u);
    set_word(file() + 16, address_of(g_base + 320));
    set_word(file() + 20, 40);
    if (norrath_factory_stream_write_setup(file()) != 0) return 19;
    if (word(file() + 8) != 0 || word(file() + 24) != 0u - 40u) return 20;

    reset_file();
    set_word(file() + 84, address_of(g_reent));
    set_word(file() + 12, 0xau);
    set_word(file() + 16, address_of(g_base + 320));
    set_word(file() + 20, 40);
    if (norrath_factory_stream_write_setup(file()) != 0) return 21;
    if (word(file() + 8) != 0) return 22;

    std::printf("stream write setup cases passed\n");
    return 0;
}

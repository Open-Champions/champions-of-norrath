#include <cstdint>
#include <cstdio>

extern "C" unsigned char norrath_player_syscall_errno_word[4] = {};
extern "C" int norrath_factory_write_reentrant(unsigned char*, int, const char*, int);

static int g_result;
static int g_errno_after;
static int g_seen_errno_at_entry;
static int g_descriptor;
static const char* g_source;
static int g_count;

extern "C" int write(int descriptor, const char* source, int count) {
    g_descriptor = descriptor;
    g_source = source;
    g_count = count;
    g_seen_errno_at_entry = int(norrath_player_syscall_errno_word[0])
        | (int(norrath_player_syscall_errno_word[1]) << 8)
        | (int(norrath_player_syscall_errno_word[2]) << 16)
        | (int(norrath_player_syscall_errno_word[3]) << 24);
    norrath_player_syscall_errno_word[0] = static_cast<unsigned char>(g_errno_after);
    norrath_player_syscall_errno_word[1] = static_cast<unsigned char>(g_errno_after >> 8);
    norrath_player_syscall_errno_word[2] = static_cast<unsigned char>(g_errno_after >> 16);
    norrath_player_syscall_errno_word[3] = static_cast<unsigned char>(g_errno_after >> 24);
    return g_result;
}

static unsigned word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}

int main() {
    unsigned char reent[4] = {0xcd, 0xcd, 0xcd, 0xcd};
    const char text[] = "ab";
    norrath_player_syscall_errno_word[0] = 3;
    g_result = 2;
    g_errno_after = 9;
    if (norrath_factory_write_reentrant(reent, 1, text, 2) != 2) return 1;
    if (g_descriptor != 1 || g_source != text || g_count != 2 || g_seen_errno_at_entry != 0) return 2;
    if (word(reent) != 0xcdcdcdcd) return 3;

    g_result = -1;
    g_errno_after = 0;
    if (norrath_factory_write_reentrant(reent, 4, text, 2) != -1) return 4;
    if (word(reent) != 0xcdcdcdcd) return 5;

    g_result = -1;
    g_errno_after = 13;
    if (norrath_factory_write_reentrant(reent, 4, text, 2) != -1) return 6;
    if (word(reent) != 13) return 7;
    std::printf("write reentrant cases passed\n");
    return 0;
}

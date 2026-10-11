#include <cstdint>
#include <cstdio>

extern "C" unsigned char norrath_player_syscall_errno_word[4] = {};
extern "C" int norrath_factory_read_reentrant(unsigned char*, int, char*, int);

static int g_result;
static int g_errno_after;
static int g_seen_errno_at_entry;
static int g_descriptor;
static char* g_destination;
static int g_count;

extern "C" int read(int descriptor, char* destination, int count) {
    g_descriptor = descriptor;
    g_destination = destination;
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
    unsigned char reent[4] = {0xab, 0xab, 0xab, 0xab};
    char buffer[4] = {};
    norrath_player_syscall_errno_word[0] = 9;
    g_result = 3;
    g_errno_after = 4;
    if (norrath_factory_read_reentrant(reent, 0, buffer, 3) != 3) return 1;
    if (g_descriptor != 0 || g_destination != buffer || g_count != 3 || g_seen_errno_at_entry != 0) return 2;
    if (word(reent) != 0xabababab) return 3;

    norrath_player_syscall_errno_word[0] = 9;
    g_result = -1;
    g_errno_after = 0;
    if (norrath_factory_read_reentrant(reent, 8, buffer, 1) != -1) return 4;
    if (word(reent) != 0xabababab) return 5;

    g_result = -1;
    g_errno_after = 0x0100;
    if (norrath_factory_read_reentrant(reent, 8, buffer, 2) != -1) return 6;
    if (word(reent) != 0x0100) return 7;
    std::printf("read reentrant cases passed\n");
    return 0;
}

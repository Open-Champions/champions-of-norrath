#include <cstdint>
#include <cstdio>

extern "C" unsigned char norrath_player_syscall_errno_word[4] = {};
extern "C" int norrath_factory_close_reentrant(unsigned char*, int);

static int g_result;
static int g_errno_after;
static int g_seen_errno_at_entry;
static int g_descriptor;

extern "C" int close(int descriptor) {
    g_descriptor = descriptor;
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
    norrath_player_syscall_errno_word[0] = 9;
    g_result = 0;
    g_errno_after = 7;
    if (norrath_factory_close_reentrant(reent, 4) != 0) return 1;
    if (g_descriptor != 4 || g_seen_errno_at_entry != 0) return 2;
    if (word(reent) != 0xabababab) return 3;

    reent[0] = reent[1] = reent[2] = reent[3] = 0xab;
    norrath_player_syscall_errno_word[0] = 9;
    g_result = -1;
    g_errno_after = 0;
    if (norrath_factory_close_reentrant(reent, 5) != -1) return 4;
    if (word(reent) != 0xabababab) return 5;

    reent[0] = reent[1] = reent[2] = reent[3] = 0xab;
    norrath_player_syscall_errno_word[0] = 9;
    g_result = -1;
    g_errno_after = 5;
    if (norrath_factory_close_reentrant(reent, 6) != -1) return 6;
    if (word(reent) != 5) return 7;
    std::printf("close reentrant cases passed\n");
    return 0;
}

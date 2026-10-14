#include <cstdint>
#include <cstdio>

extern "C" unsigned char norrath_player_syscall_errno_word[4] = {};
extern "C" long long norrath_factory_seek_reentrant(unsigned char*, int, long long, int);

static long long g_result;
static int g_errno_after;
static int g_seen_errno_at_entry;
static int g_descriptor;
static long long g_offset;
static int g_whence;

extern "C" long long lseek(int descriptor, long long offset, int whence) {
    g_descriptor = descriptor;
    g_offset = offset;
    g_whence = whence;
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
    unsigned char reent[4] = {0x11, 0x22, 0x33, 0x44};
    norrath_player_syscall_errno_word[0] = 9;
    g_result = 0x100000000LL;
    g_errno_after = 5;
    if (norrath_factory_seek_reentrant(reent, 3, -40, 1) != 0x100000000LL) return 1;
    if (g_descriptor != 3 || g_offset != -40 || g_whence != 1 || g_seen_errno_at_entry != 0) return 2;
    if (word(reent) != 0x44332211) return 3;

    g_result = -1;
    g_errno_after = 0;
    if (norrath_factory_seek_reentrant(reent, 3, 8, 0) != -1) return 4;
    if (word(reent) != 0x44332211) return 5;

    g_result = -1;
    g_errno_after = 2;
    if (norrath_factory_seek_reentrant(reent, 7, 0x123456789LL, 2) != -1) return 6;
    if (g_offset != 0x123456789LL || word(reent) != 2) return 7;
    std::printf("seek reentrant cases passed\n");
    return 0;
}

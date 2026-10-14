#include <cstdio>
extern "C" unsigned char norrath_player_syscall_errno_word[4];
int main() {
    if (norrath_player_syscall_errno_word[0] != 0
        || norrath_player_syscall_errno_word[1] != 0
        || norrath_player_syscall_errno_word[2] != 0
        || norrath_player_syscall_errno_word[3] != 0) return 1;
    norrath_player_syscall_errno_word[0] = 5;
    if (norrath_player_syscall_errno_word[0] != 5) return 2;
    std::printf("errno word cases passed\n");
    return 0;
}

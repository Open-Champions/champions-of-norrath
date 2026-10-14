#include <cstdio>
alignas(4) static unsigned char errno_bytes[4];
extern "C" unsigned char norrath_player_syscall_errno_word[4];
static int kill_result, kill_error, kill_calls, seen_pid, seen_sig, error_at_call;
extern "C" int kill(int p, int s) {
    ++kill_calls; seen_pid = p; seen_sig = s;
    error_at_call = *reinterpret_cast<unsigned*>(norrath_player_syscall_errno_word);
    *reinterpret_cast<unsigned*>(norrath_player_syscall_errno_word) = kill_error;
    return kill_result;
}
extern "C" { alignas(4) unsigned char norrath_player_syscall_errno_word[4]; }
#include "kill_r.cpp"
int main() {
    int bad = 0;
    unsigned record = 0x11111111u;
    // Success: result passed through, record untouched even with a stale error value.
    kill_result = 0; kill_error = 9; *reinterpret_cast<unsigned*>(norrath_player_syscall_errno_word) = 77;
    if (_kill_r(&record, 1234, 15) != 0 || record != 0x11111111u || error_at_call != 0 || seen_pid != 1234 || seen_sig != 15) { std::puts("success wrong"); ++bad; }
    // Failure with an error value: copied to the record.
    kill_result = -1; kill_error = 22;
    if (_kill_r(&record, 1, 2) != -1 || record != 22u) { std::puts("failure with error wrong"); ++bad; }
    // Failure without an error value: record untouched.
    record = 5; kill_result = -1; kill_error = 0;
    if (_kill_r(&record, 1, 2) != -1 || record != 5u) { std::puts("failure without error wrong"); ++bad; }
    // Other results (not -1) never copy the error word.
    record = 6; kill_result = 3; kill_error = 4;
    if (_kill_r(&record, 1, 2) != 3 || record != 6u) { std::puts("non -1 result wrong"); ++bad; }
    if (bad) return 1;
    std::puts("_kill_r contract passed");
    return 0;
}

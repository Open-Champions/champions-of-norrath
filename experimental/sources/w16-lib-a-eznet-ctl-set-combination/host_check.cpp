// Host check, not a plan input.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#define printf norrath_host_printf
#define strcpy norrath_host_strcpy
#include "set_combination.cpp"
#undef printf
#undef strcpy
extern "C" { int m_rpcRunning; unsigned char m_cd[64]; alignas(8) unsigned char m_buf[320]; }
static int prints, rpcs, fn, snd, rcv, reply, printed_int = -1; static char seen_name[32], seen_comb[32], last_fmt[64], last_s[64];
extern "C" int norrath_host_printf(const char* f, ...) {
    ++prints; std::strcpy(last_fmt, f);
    va_list ap; va_start(ap, f);
    if (!std::strcmp(f, "%s> ")) std::strcpy(last_s, va_arg(ap, const char*));
    else if (!std::strcmp(f, "ezNetCtlSetCombination returned %d\n")) printed_int = va_arg(ap, int);
    va_end(ap);
    return 0;
}
extern "C" char* norrath_host_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
extern "C" int sceSifCallRpc(void* cl, unsigned f, unsigned m, void* se, int ss, void* re, int rs, void*, void*) {
    ++rpcs; fn = f; snd = ss; rcv = rs;
    if (cl != m_cd || se != m_buf || re != m_buf || m) return 99;
    std::strcpy(seen_name, reinterpret_cast<char*>(m_buf + 4)); std::strcpy(seen_comb, reinterpret_cast<char*>(m_buf + 260));
    *reinterpret_cast<int*>(m_buf) = reply;
    return 0;
}
int main() {
    m_rpcRunning = 0;
    if (ezNetCtlSetCombination("mc0:", "abc") != -1 || prints != 1 || rpcs != 0) return 1;
    m_rpcRunning = 2; reply = -8; prints = 0;
    if (ezNetCtlSetCombination("mc0:", "abc") != -8 || rpcs != 1 || prints != 2) return 2;
    if (fn != 2 || snd != 292 || rcv != 292 || std::strcmp(seen_name, "mc0:") || std::strcmp(seen_comb, "abc")) return 3;
    if (printed_int != -8 || std::strcmp(last_fmt, "ezNetCtlSetCombination returned %d\n") || std::strcmp(last_s, "../eznetcnf/libezctl.c")) return 4;
    reply = 0; prints = 0;
    if (ezNetCtlSetCombination("x", "y") != 0 || prints != 2) return 5;
    std::puts("set combination check passed");
    return 0;
}

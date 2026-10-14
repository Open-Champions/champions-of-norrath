// Host check, not a plan input. Pointer words are 32 bits on the target; the host uses a small value as the properties word.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#define printf norrath_host_printf
#define strcpy norrath_host_strcpy
#include "get_properties.cpp"
#undef printf
#undef strcpy
static int prints, rpcs, fn, snd, rcv, reply, printed_int = -1, word0_at_call; static char seen_name[32], seen_comb[32], last_fmt[64], last_s[64];
extern "C" int norrath_host_printf(const char* f, ...) {
    ++prints; std::strcpy(last_fmt, f);
    va_list ap; va_start(ap, f);
    if (!std::strcmp(f, "%s> ")) std::strcpy(last_s, va_arg(ap, const char*));
    else if (!std::strcmp(f, "ezNetCnfGetNetcnfifData returned %d\n")) printed_int = va_arg(ap, int);
    va_end(ap);
    return 0;
}
extern "C" char* norrath_host_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
extern "C" int sceSifCallRpc(void* cl, unsigned f, unsigned m, void* se, int ss, void* re, int rs, void*, void*) {
    ++rpcs; fn = f; snd = ss; rcv = rs;
    if (cl != m_rpcCd || se != m_rpcBuf || re != m_rpcBuf || m) return 99;
    word0_at_call = *reinterpret_cast<int*>(m_rpcBuf);
    std::strcpy(seen_name, reinterpret_cast<char*>(m_rpcBuf + 4)); std::strcpy(seen_comb, reinterpret_cast<char*>(m_rpcBuf + 260));
    *reinterpret_cast<int*>(m_rpcBuf) = reply;
    return 0;
}
int main() {
    m_rpcRunning = 0;
    if (ezNetCnfGetProperties("mc0:", "abc", nullptr) != -1 || prints != 1 || rpcs != 0) return 1;
    m_rpcRunning = 1; reply = 3; prints = 0;
    if (ezNetCnfGetProperties("mc0:", "abc", reinterpret_cast<void*>(0x1234)) != 3 || rpcs != 1 || prints != 2) return 2;
    if (word0_at_call != 0x1234 || fn != 2 || snd != 292 || rcv != 292 || std::strcmp(seen_name, "mc0:") || std::strcmp(seen_comb, "abc")) return 3;
    if (printed_int != 3 || std::strcmp(last_fmt, "ezNetCnfGetNetcnfifData returned %d\n") || std::strcmp(last_s, "../eznetcnf/libezcnf.c")) return 4;
    std::puts("get properties check passed");
    return 0;
}

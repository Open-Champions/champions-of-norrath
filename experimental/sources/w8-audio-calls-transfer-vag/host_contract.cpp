#include "transfer_vag.cpp"
#include <csetjmp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
extern "C" unsigned char norrath_sound_rpc_client_view[96] = {};
extern "C" unsigned char norrath_sound_rpc_send_view[64] = {};
extern "C" unsigned char norrath_sound_rpc_receive_view[64] = {};
extern "C" unsigned char norrath_sound_iop_buffer_word_view[4] = {};
static std::vector<std::string> events;
static std::vector<int> rpc_commands;
static std::jmp_buf stop;
static int alloc_size = -1, alloc_fail = 0, freed = 0, flushed = -1;
static void* last_block = nullptr;
static int fail_trans_at = -1, fail_remote_at = -1, fail_complete_at = -1;
static int trans_n = 0, remote_n = 0, complete_n = 0;
static std::vector<int> trans_len, trans_dst, remote_dst, remote_len, remote_iop;
static std::vector<std::string> trans_bytes_first;
static unsigned a_line; static std::string a_file, a_cond; static int a_ra;
static std::string alloc_label = "unset";
extern "C" int norrath_sound_call_rpc(void* c, int cmd, int mode, void* s, int sl, void* r, int rl, void* e, void* p) {
    if (c != norrath_sound_rpc_client_view || mode != 0 || s != norrath_sound_rpc_send_view || sl != 16) std::exit(90);
    if (r != norrath_sound_rpc_receive_view || rl != 64 || e || p) std::exit(91);
    rpc_commands.push_back(cmd); events.push_back("rpc" + std::to_string(cmd)); return 0;
}
extern "C" void norrath_sound_assert(const char* f, unsigned l, const char* c, int ra) {
    a_file = f; a_line = l; a_cond = c; a_ra = ra; events.push_back("assert"); std::longjmp(stop, 1);
}
extern "C" void* memAlloc__FiPc(int n, char* label) {
    alloc_size = n; alloc_label = label; events.push_back("alloc");
    if (alloc_fail) return nullptr;
    last_block = std::malloc(n + 64); std::memset(last_block, 0, n + 64); return last_block;
}
extern "C" void memFree__FPv(void* p) { if (p != last_block) std::exit(92); ++freed; events.push_back("free"); std::free(p); }
extern "C" void FlushCache(int m) { flushed = m; events.push_back("flush"); }
extern "C" int sceSdTransToIOP(void* src, int dst, int n, int mode) {
    if (mode != 1) std::exit(93);
    trans_len.push_back(n); trans_dst.push_back(dst); events.push_back("trans");
    trans_bytes_first.push_back(std::string(static_cast<char*>(src), 1));
    return trans_n++ == fail_trans_at ? -1 : 0;
}
extern "C" int sceSdRemote(int one, int cmd, ...) {
    if (one != 1) std::exit(94);
    __builtin_va_list ap; __builtin_va_start(ap, cmd);
    int a = __builtin_va_arg(ap, int), b = __builtin_va_arg(ap, int);
    if (cmd == 0x80d0) {
        int c = __builtin_va_arg(ap, int), d = __builtin_va_arg(ap, int), e = __builtin_va_arg(ap, int);
        if (a != 0 || b != 0) std::exit(95);
        remote_iop.push_back(c); remote_dst.push_back(d); remote_len.push_back(e); events.push_back("remote");
        return remote_n++ == fail_remote_at ? -1 : 0;
    }
    if (cmd != 0x80f0 || a != 0 || b != 1) std::exit(96);
    events.push_back("complete");
    return complete_n++ == fail_complete_at ? -1 : 0;
}
extern "C" void soundError__FPc(char* t) { events.push_back(std::string("err:") + t); }
static void reset() {
    events.clear(); rpc_commands.clear(); trans_len.clear(); trans_dst.clear(); remote_dst.clear(); remote_len.clear();
    remote_iop.clear(); trans_bytes_first.clear(); freed = 0; flushed = -1; trans_n = remote_n = complete_n = 0;
    fail_trans_at = fail_remote_at = fail_complete_at = -1; alloc_fail = 0;
}
int main() {
    alignas(8) static unsigned char vag[48 + 20000];
    for (int i = 0; i < 20000; ++i) vag[48 + i] = static_cast<unsigned char>(i % 251 + 1);
    int iop = 0x50000; std::memcpy(norrath_sound_iop_buffer_word_view, &iop, 4);
    int size = 10000; std::memcpy(vag + 44, &size, 4);
    transferVAG__FP10_vagHeaderi(vag, 0x2000);
    // 10000 rounds up to 10048: pieces 4096, 4096, 1856
    if (alloc_size != 10000 || alloc_label != "" || flushed != 0 || freed != 1) return 1;
    if (trans_len != std::vector<int>{4096, 4096, 1856}) return 2;
    if (trans_dst != std::vector<int>{0x50000, 0x50000, 0x50000}) return 3;
    if (remote_len != std::vector<int>{4096, 4096, 1856} || remote_iop != std::vector<int>{0x50000, 0x50000, 0x50000}) return 4;
    if (remote_dst != std::vector<int>{0x2000, 0x3000, 0x4000}) return 5;
    if (rpc_commands != std::vector<int>{176, 192}) return 6;
    // order: rpc176, alloc, flush, then trans/remote/complete triples, free, rpc192
    std::vector<std::string> want = {"rpc176", "alloc", "flush", "trans", "remote", "complete", "trans", "remote",
                                     "complete", "trans", "remote", "complete", "free", "rpc192"};
    if (events != want) return 7;
    // the staged bytes are the body: first byte of each piece, 1 + (offset % 251)
    if (trans_bytes_first[1][0] != static_cast<char>(4096 % 251 + 1)) return 8;
    // an exact multiple of 64 moves exactly that; 4097 splits into 4096 and 64 after rounding
    reset(); size = 4097; std::memcpy(vag + 44, &size, 4);
    transferVAG__FP10_vagHeaderi(vag, 0);
    if (trans_len != std::vector<int>{4096, 64}) return 9;
    reset(); size = 64; std::memcpy(vag + 44, &size, 4);
    transferVAG__FP10_vagHeaderi(vag, 0);
    if (trans_len != std::vector<int>{64}) return 10;
    reset(); size = 4096; std::memcpy(vag + 44, &size, 4);
    transferVAG__FP10_vagHeaderi(vag, 0);
    if (trans_len != std::vector<int>{4096}) return 11;
    // zero size: no transfers, both RPCs still run, allocation of 0 requested
    reset(); size = 0; std::memcpy(vag + 44, &size, 4);
    transferVAG__FP10_vagHeaderi(vag, 0);
    if (!trans_len.empty() || rpc_commands.size() != 2 || alloc_size != 0) return 12;
    // error texts, each failure only logs and the loop continues
    reset(); size = 100; std::memcpy(vag + 44, &size, 4); fail_trans_at = 0; fail_remote_at = 0; fail_complete_at = 0;
    transferVAG__FP10_vagHeaderi(vag, 0);
    want = {"rpc176", "alloc", "flush", "trans", "err:Failed to transfer to IOP.", "remote", "err:Failed to transfer to SPU2.",
            "complete", "err:Complete transfer to SPU2 failed", "free", "rpc192"};
    if (events != want) return 13;
    // allocation failure: error text and assertion, stopping before the copy
    reset(); alloc_fail = 1; size = 100; std::memcpy(vag + 44, &size, 4);
    if (setjmp(stop) == 0) { transferVAG__FP10_vagHeaderi(vag, 0); return 14; }
    want = {"rpc176", "alloc", "err:Couldn't allocate aligned VAG body copy.", "assert"};
    if (events != want || a_file != "sound.cc" || a_line != 1807 || a_cond != "0") return 15;
    std::puts("transfer_vag passed");
    return 0;
}

#include "flush_command_buffer.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char norrath_sound_key_on_words_view[8] = {};
extern "C" unsigned char norrath_sound_key_off_words_view[8] = {};
extern "C" unsigned char norrath_sound_batch_iop_buffer_word_view[4] = {};
extern "C" unsigned char batchBuffer[512 * 8 + 8] = {};
extern "C" int nmBatchCommands = 0;
static int order = 0, flush_at = 0, trans_at = 0, remote_at = 0;
static int flush_arg = -1, t_dst, t_size, t_mode, r0, r1, r2, r3, r4;
static void* t_src;
extern "C" void FlushCache(int m) { flush_arg = m; flush_at = ++order; }
extern "C" int sceSdTransToIOP(void* s, int d, int z, int m) { t_src = s; t_dst = d; t_size = z; t_mode = m; trans_at = ++order; return 0; }
extern "C" int sceSdRemote(int a, int b, int c, int d, int e) { r0 = a; r1 = b; r2 = c; r3 = d; r4 = e; remote_at = ++order; return 0; }
static unsigned* on() { return reinterpret_cast<unsigned*>(norrath_sound_key_on_words_view); }
static unsigned* off() { return reinterpret_cast<unsigned*>(norrath_sound_key_off_words_view); }
static unsigned short h(int e, int o) { unsigned short v; std::memcpy(&v, batchBuffer + e * 8 + o, 2); return v; }
static unsigned w(int e) { unsigned v; std::memcpy(&v, batchBuffer + e * 8 + 4, 4); return v; }
int main() {
    soundFlushCommandBuffer__Fv();                       // nothing queued: no hardware calls
    if (order != 0 || nmBatchCommands != 0) return 1;
    unsigned iop = 0x1234; std::memcpy(norrath_sound_batch_iop_buffer_word_view, &iop, 4);
    on()[0] = 0xff0f; on()[1] = 0x30; off()[0] = 0x0f; off()[1] = 0x30; 
    soundFlushCommandBuffer__Fv();
    // kons[0] keeps 0xff00 (koffs removed 0x0f); kons[1] becomes zero and is not sent
    if (h(0, 0) != 2 || h(0, 2) != 0x1500 || w(0) != 0xff00) return 2;
    if (h(1, 0) != 2 || h(1, 2) != 0x1600 || w(1) != 0x0f) return 3;
    if (h(2, 0) != 2 || h(2, 2) != 0x1601 || w(2) != 0x30) return 4;
    if (flush_arg != 0 || t_src != batchBuffer || t_dst != 0x1234 || t_size != 24 || t_mode != 1) return 5;
    if (r0 != 1 || r1 != 0x80b0 || r2 != 0x1234 || r3 != 0 || r4 != 3) return 6;
    if (!(flush_at < trans_at && trans_at < remote_at)) return 7;
    if (nmBatchCommands != 0 || on()[0] != 0 || on()[1] != 0 || off()[0] != 0 || off()[1] != 0) return 8;
    // key-on word 1 alone, appended after the existing count
    nmBatchCommands = 0; order = 0;
    on()[1] = 0x80000000u;
    soundFlushCommandBuffer__Fv();
    if (h(0, 2) != 0x1501 || w(0) != 0x80000000u || r4 != 1 || t_size != 8) return 9;
    // a full buffer: nothing appended, words cleared, the count of 512 is still sent
    nmBatchCommands = 512; on()[0] = 1; off()[1] = 2;
    soundFlushCommandBuffer__Fv();
    if (on()[0] != 0 || off()[1] != 0 || r4 != 512 || t_size != 4096) return 10;
    for (int i = 512 * 8; i < 512 * 8 + 8; ++i) if (batchBuffer[i] != 0) return 11;
    // one slot left: the key-on entry fits, the key-off entry does not
    nmBatchCommands = 511; on()[0] = 5; off()[0] = 0x100;
    soundFlushCommandBuffer__Fv();
    if (r4 != 512 || h(511, 2) != 0x1500 || w(511) != 5 || off()[0] != 0) return 12;
    std::puts("flush_command_buffer passed");
    return 0;
}

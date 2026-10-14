#include <cstdio>
#include <cstring>
#define NORRATH_HOST_CONTRACT 1
#include "fs_close.cpp"
extern "C" int _fs_init = 1;
extern "C" alignas(16) unsigned char _iob[32 * 16] = {};
extern "C" alignas(4) unsigned char _send_data[32] = {};
extern "C" alignas(4) unsigned char _rcv_data_rpc[8] = {};
extern "C" unsigned char _cd[4] = {};
static int trace[32], trace_len, rpc_result, reply_value, io_value, create_calls, delete_calls, waits, sigs, bad;
static unsigned char* slot_to_return; static unsigned seen_descriptor; static volatile int* noted_result;
static int sema_id = 21; static int deleted_id, waited_id;
static unsigned sent_words[5];
static void note(int what) { if (trace_len < 32) trace[trace_len++] = what; }
extern "C" void norrath_host_note_result(volatile int* result) { noted_result = result; }
extern "C" unsigned char* get_iob(unsigned descriptor) { seen_descriptor = descriptor; note(1); return slot_to_return; }
extern "C" int _sceFsWaitS(int flag) { if (flag != 1) bad = 1; note(2); return 0; }
extern "C" void _sceFsSigSema() { ++sigs; note(3); }
extern "C" int CreateSema(void* parameters) { ++create_calls; note(4); int* w = static_cast<int*>(parameters); if (w[1] != 1 || w[2] != 0 || w[5] != 0) bad = 1; return sema_id; }
extern "C" int DeleteSema(int semaphore) { ++delete_calls; deleted_id = semaphore; note(5); return 0; }
extern "C" int norrath_player_wait_semaphore(int semaphore) { ++waits; waited_id = semaphore; note(6); *noted_result = io_value; return 0; }
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* end, void* end_param) {
    note(7);
    if (client != _cd || function != 1 || mode != 0 || send != _send_data || send_size != 20 ||
        receive != _rcv_data_rpc || receive_size != 4 || end || end_param) bad = 1;
    for (int index = 0; index < 5; ++index) std::memcpy(&sent_words[index], _send_data + index * 4, 4);
    std::memcpy(_rcv_data_rpc, &reply_value, 4);
    return rpc_result;
}
static void reset() {
    trace_len = rpc_result = reply_value = create_calls = delete_calls = waits = sigs = bad = 0;
    _fs_init = 1; io_value = 0; std::memset(_send_data, 0, sizeof _send_data);
}
int main() {
    unsigned* slot_words;
    // Library not initialised.
    reset(); _fs_init = 0; slot_to_return = _iob;
    if (sceClose(3) != -1 || seen_descriptor != 3 || sigs != 1 || create_calls != 0 || trace[0] != 1 || trace[1] != 2 || trace[2] != 3) return 1;
    // Null slot, and a slot whose word 1 is zero.
    reset(); slot_to_return = nullptr;
    if (sceClose(3) != -9 || sigs != 1 || create_calls != 0) return 2;
    reset(); slot_to_return = _iob + 5 * 16; std::memset(slot_to_return, 0, 16);
    if (sceClose(3) != -9 || sigs != 1) return 3;
    // Normal close: slot 5 with handle word 0x1234 and in-use word 1.
    reset(); slot_to_return = _iob + 5 * 16;
    std::memset(slot_to_return, 0, 16);
    slot_words = reinterpret_cast<unsigned*>(slot_to_return);
    slot_words[0] = 0x1234; slot_words[1] = 1;
    rpc_result = 0; reply_value = 1; io_value = 0;
    if (sceClose(9) != 0 || bad) return 4;
    if (sent_words[0] != 21u || sent_words[2] != 4u || sent_words[3] != 0x1234u || sent_words[4] != 5u) return 5;
    if (slot_words[1] != 0u || slot_words[0] != 0x1234u) return 6;
    if (create_calls != 1 || delete_calls != 1 || deleted_id != 21 || waits != 1 || waited_id != 21 || sigs != 1) return 7;
    // Order: slot lookup, wait, create, rpc, signal, wait for completion, delete.
    const int expected[] = {1, 2, 4, 7, 3, 6, 5};
    if (trace_len != 7) return 8;
    for (int index = 0; index < 7; ++index) if (trace[index] != expected[index]) return 9;
    // A negative completion value is returned; positive becomes 0.
    reset(); slot_to_return = _iob + 2 * 16; slot_words = reinterpret_cast<unsigned*>(slot_to_return); slot_words[0] = 8; slot_words[1] = 1;
    reply_value = 1; io_value = -7;
    if (sceClose(1) != -7) return 10;
    reset(); slot_words[1] = 1; reply_value = 1; io_value = 42;
    if (sceClose(1) != 0) return 11;
    // The IOP reply word zero: no wait, the semaphore is deleted, slot is already cleared.
    reset(); slot_words[1] = 1; reply_value = 0;
    if (sceClose(1) != -11 || waits != 0 || delete_calls != 1 || slot_words[1] != 0u || sigs != 1) return 12;
    // RPC failure: semaphore deleted, lock released, slot untouched.
    reset(); slot_words[1] = 1; rpc_result = -1; reply_value = 1;
    if (sceClose(1) != -11 || waits != 0 || delete_calls != 1 || sigs != 1 || slot_words[1] != 1u) return 13;
    std::puts("sceClose contract passed");
    return 0;
}

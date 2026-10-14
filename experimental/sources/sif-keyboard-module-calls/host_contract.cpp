#include "module.cpp"

#include <cstdio>

static int bind_result;
static int version_result;
static int bind_calls;
static int version_calls;
static char log_text[8];
static int log_used;
static char* copy_destination;
static const char* copy_source;
static unsigned int copy_count;
static int rpc_calls;
static void* seen_client;
static int seen_operation;
static int seen_mode;
static void* seen_send;
static int seen_send_size;
static void* seen_receive;
static int seen_receive_size;
static void* seen_completion;
static void* seen_completion_argument;
static int call_result;
static int planted_word;
static int planted_second;
static int memcpy_calls;
static void* memcpy_destination;
static const void* memcpy_source;
static int memcpy_count;
static int seen_first_at_call;
static int seen_second_at_call;

static void mark(char step) {
    if (log_used < 7) log_text[log_used++] = step;
    log_text[log_used] = 0;
}

extern "C" int _lf_bind() {
    mark('B');
    ++bind_calls;
    return bind_result;
}
extern "C" int _lf_version() {
    mark('V');
    ++version_calls;
    return version_result;
}
extern "C" char* strncpy(char* destination, const char* source, unsigned int count) {
    mark('S');
    copy_destination = destination;
    copy_source = source;
    copy_count = count;
    unsigned int index = 0;
    for (; index < count && source[index] != 0; ++index)
        destination[index] = source[index];
    for (; index < count; ++index)
        destination[index] = 0;
    return destination;
}
extern "C" void* memcpy(void* destination, const void* source, int count) {
    mark('M');
    ++memcpy_calls;
    memcpy_destination = destination;
    memcpy_source = source;
    memcpy_count = count;
    if (count > 0) {
        const unsigned char* bytes = static_cast<const unsigned char*>(source);
        unsigned char* out = static_cast<unsigned char*>(destination);
        for (int index = 0; index < count; ++index)
            out[index] = bytes[index];
    }
    return destination;
}
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument) {
    mark('R');
    ++rpc_calls;
    seen_first_at_call = _senddata.header.first;
    seen_second_at_call = _senddata.header.second;
    seen_client = client;
    seen_operation = operation;
    seen_mode = mode;
    seen_send = send;
    seen_send_size = send_size;
    seen_receive = receive;
    seen_receive_size = receive_size;
    seen_completion = completion;
    seen_completion_argument = completion_argument;
    if (receive != 0 && receive_size >= 4)
        __builtin_memcpy(receive, &planted_word, 4);
    if (receive != 0 && receive_size >= 8)
        __builtin_memcpy(static_cast<unsigned char*>(receive) + 4, &planted_second, 4);
    return call_result;
}

static void reset(int bind_word, int version_word, int rpc_result, int word) {
    bind_result = bind_word;
    version_result = version_word;
    call_result = rpc_result;
    planted_word = word;
    planted_second = 0;
    memcpy_calls = 0;
    memcpy_destination = 0;
    memcpy_source = 0;
    memcpy_count = 0;
    bind_calls = 0;
    version_calls = 0;
    rpc_calls = 0;
    log_used = 0;
    log_text[0] = 0;
    copy_destination = 0;
    copy_source = 0;
    copy_count = 0;
    seen_operation = -1;
    seen_mode = -1;
    seen_send_size = -1;
    seen_receive_size = -1;
    seen_completion = reinterpret_cast<void*>(1);
    seen_completion_argument = reinterpret_cast<void*>(1);
    for (unsigned int index = 0; index < sizeof _senddata.bytes; ++index)
        _senddata.bytes[index] = 0x5a;
}

static bool same(const char* left, const char* right) {
    for (int index = 0; left[index] != 0 || right[index] != 0; ++index)
        if (left[index] != right[index]) return false;
    return true;
}

static bool same_bytes(const unsigned char* left, const char* right, int count) {
    for (int index = 0; index < count; ++index)
        if (left[index] != static_cast<unsigned char>(right[index])) return false;
    return true;
}

static int expect_rpc(int operation, int send_size, int receive_size) {
    if (rpc_calls != 1 || seen_client != cd || seen_operation != operation || seen_mode != 0) return 1;
    if (seen_send != &_senddata || seen_receive != &_senddata || seen_send_size != send_size) return 2;
    if (seen_receive_size != receive_size || seen_completion != 0 || seen_completion_argument != 0) return 3;
    return 0;
}

int main() {
    char name[] = "visounddriver";
    reset(-1, 0, 0, 9);
    if (sceSifSearchModuleByName(name) != static_cast<int>(0xffff0000u)) return 10;
    if (!same(log_text, "B") || version_calls != 0 || rpc_calls != 0) return 11;
    reset(-2147483647 - 1, 7, 0, 9);
    if (sceSifSearchModuleByName(name) != static_cast<int>(0xffff0000u) || version_calls != 0) return 12;
    reset(0, 1, 0, 9);
    if (sceSifSearchModuleByName(name) != static_cast<int>(0xfffefffcu)) return 13;
    if (!same(log_text, "BV") || rpc_calls != 0) return 14;
    reset(0, -4, 0, 9);
    if (sceSifSearchModuleByName(name) != static_cast<int>(0xfffefffcu) || rpc_calls != 0) return 15;
    reset(0, 0, -1, 44);
    if (sceSifSearchModuleByName(name) != static_cast<int>(0xfffeffffu) || expect_rpc(9, 512, 4) != 0) return 16;
    if (!same(log_text, "BVSR")) return 17;
    if (copy_destination != reinterpret_cast<char*>(_senddata.bytes + 8) || copy_source != name) return 18;
    if (copy_count != 252 || _senddata.bytes[259] != 0) return 19;
    if (!same_bytes(_senddata.bytes + 8, "visounddriver", 14)) return 20;
    if (_senddata.bytes[8 + 20] != 0 || _senddata.bytes[4] != 0x5a) return 21;
    reset(3, 0, 0, 77);
    if (sceSifSearchModuleByName(name) != 77 || expect_rpc(9, 512, 4) != 0) return 22;
    char long_name[253];
    for (int index = 0; index < 252; ++index) long_name[index] = 'A';
    long_name[252] = 0;
    reset(0, 0, 1, 5);
    if (sceSifSearchModuleByName(long_name) != 5 || copy_count != 252) return 23;
    if (_senddata.bytes[8 + 250] != 'A' || _senddata.bytes[259] != 0) return 24;
    char empty[] = "";
    reset(0, 0, 0, 8);
    if (sceSifSearchModuleByName(empty) != 8) return 25;
    if (_senddata.bytes[8] != 0 || _senddata.bytes[259] != 0 || _senddata.bytes[4] != 0x5a) return 26;
    reset(-2, 0, 0, 1);
    if (sceSifUnloadModule(42) != static_cast<int>(0xffff0000u) || !same(log_text, "B")) return 30;
    if (_senddata.bytes[0] != 0x5a) return 31;
    reset(0, 2, 0, 1);
    if (sceSifUnloadModule(42) != static_cast<int>(0xfffefffcu) || rpc_calls != 0) return 32;
    if (_senddata.header.first != static_cast<int>(0x5a5a5a5a)) return 33;
    reset(0, 0, -1, 99);
    if (sceSifUnloadModule(42) != static_cast<int>(0xfffeffffu) || expect_rpc(8, 4, 4) != 0) return 34;
    if (!same(log_text, "BVR") || copy_count != 0) return 35;
    reset(1, 0, 0, 99);
    if (sceSifUnloadModule(42) != 99 || expect_rpc(8, 4, 4) != 0) return 36;
    reset(0, 0, 0, -5);
    if (sceSifUnloadModule(-5) != -5 || expect_rpc(8, 4, 4) != 0) return 37;
    int result_word = 12345;
    reset(-1, 0, 0, 1);
    if (sceSifStopModule(9, 4, "abcd", &result_word) != static_cast<int>(0xffff0000u)) return 40;
    if (result_word != 12345 || memcpy_calls != 0 || rpc_calls != 0 || !same(log_text, "B")) return 41;
    reset(0, 1, 0, 1);
    if (sceSifStopModule(9, 4, "abcd", &result_word) != static_cast<int>(0xfffefffcu)) return 42;
    if (result_word != 12345 || _senddata.header.first != static_cast<int>(0x5a5a5a5a)) return 43;
    reset(0, 0, -1, 1);
    planted_second = 2;
    if (sceSifStopModule(9, 0, 0, &result_word) != static_cast<int>(0xfffeffffu)) return 44;
    if (result_word != 12345 || expect_rpc(7, 512, 8) != 0 || memcpy_calls != 0) return 45;
    if (seen_first_at_call != 9 || seen_second_at_call != 0 || !same(log_text, "BVR")) return 46;
    char four[] = {'w', 'x', 'y', 'z'};
    reset(0, 0, 0, 1001);
    planted_second = 2002;
    if (sceSifStopModule(9, 4, four, &result_word) != 1001 || result_word != 2002) return 47;
    if (memcpy_calls != 1 || memcpy_count != 4 || memcpy_destination != _senddata.bytes + 260) return 48;
    if (memcpy_source != four || _senddata.bytes[259] != 0x5a || _senddata.bytes[264] != 0x5a) return 49;
    if (_senddata.bytes[260] != 'w' || _senddata.bytes[263] != 'z' || seen_second_at_call != 4) return 50;
    char block[252];
    for (int index = 0; index < 252; ++index) block[index] = static_cast<char>(index);
    reset(0, 0, 0, 3);
    planted_second = 4;
    if (sceSifStopModule(8, 252, block, &result_word) != 3 || memcpy_count != 252) return 51;
    if (seen_second_at_call != 252 || _senddata.bytes[259] != 0x5a || _senddata.bytes[511] != 251) return 52;
    reset(0, 0, 0, 3);
    planted_second = 4;
    if (sceSifStopModule(8, 253, block, &result_word) != 3 || memcpy_count != 252) return 53;
    if (seen_second_at_call != 252 || _senddata.bytes[260] != 0 || _senddata.bytes[511] != 251) return 54;
    reset(0, 0, 0, 3);
    planted_second = 4;
    if (sceSifStopModule(8, 1000, block, &result_word) != 3 || memcpy_count != 252 || seen_second_at_call != 252)
        return 55;
    reset(0, 0, 0, 3);
    planted_second = 4;
    if (sceSifStopModule(8, 0, four, &result_word) != 3 || memcpy_calls != 1 || memcpy_count != 0) return 56;
    if (seen_second_at_call != 0 || _senddata.bytes[260] != 0x5a) return 57;
    reset(0, 0, 0, 3);
    planted_second = 4;
    if (sceSifStopModule(8, -1, four, &result_word) != 3 || memcpy_count != -1 || seen_second_at_call != -1)
        return 58;
    reset(0, 0, 0, 1001);
    planted_second = 2002;
    int* aliased = &_senddata.header.first;
    if (sceSifStopModule(8, 0, 0, aliased) != 1001) return 59;
    if (*aliased != 2002) return 60;
    std::puts("module search, unload, and stop passed");
    return 0;
}

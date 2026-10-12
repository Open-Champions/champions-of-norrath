#include "load_module_call.cpp"

#include <cstdio>
#include <cstring>

unsigned char norrath_loadfile_client[40];
unsigned _senddata[128];

static int bind_result;
static int version_result;
static int call_result;
static int bind_calls, version_calls, rpc_calls;
static int rpc_operation, rpc_mode, rpc_send_size, rpc_receive_size;
static void* rpc_client;
static void* rpc_send;
static void* rpc_receive;
static void* rpc_completion;
static unsigned snapshot[128];
static unsigned reply0, reply1;

extern "C" int _lf_bind() { ++bind_calls; return bind_result; }
extern "C" int _lf_version() { ++version_calls; return version_result; }
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion, void* completion_argument) {
    ++rpc_calls;
    rpc_client = client; rpc_operation = operation; rpc_mode = mode; rpc_send = send;
    rpc_send_size = send_size; rpc_receive = receive; rpc_receive_size = receive_size;
    rpc_completion = (void*)((unsigned long)completion | (unsigned long)completion_argument);
    std::memcpy(snapshot, _senddata, sizeof snapshot);
    if (call_result >= 0) { _senddata[0] = reply0; _senddata[1] = reply1; }
    return call_result;
}

static void reset() {
    bind_result = 0; version_result = 0; call_result = 0;
    bind_calls = version_calls = rpc_calls = 0;
    reply0 = 0x11; reply1 = 0x22;
    std::memset(_senddata, 0xcc, sizeof _senddata);
}

int main() {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(snapshot);
    int result = 0x7777;

    reset();
    bind_result = -1;
    if (_sceSifLoadModule("host0:a", 0, 0, &result, 0) != -1 || version_calls != 0 || rpc_calls != 0) return 1;
    bind_result = static_cast<int>(0xffff0000u);
    if (_sceSifLoadModule("x", 0, 0, &result, 0) != static_cast<int>(0xffff0000u)) return 2;

    reset();
    version_result = 1;
    if (_sceSifLoadModule("x", 0, 0, &result, 0) != static_cast<int>(0xfffefffcu) || rpc_calls != 0) return 3;
    if (_senddata[5] != 0xcccccccc) return 4;

    // No argument pointer: word 0 and offset 260 cleared; the length is ignored.
    reset();
    int rc = _sceSifLoadModule("cdrom0:\\X.IRX;1", 99, 0, &result, 6);
    if (rc != 0x11 || result != 0x22) return 5;
    if (std::strcmp(reinterpret_cast<const char*>(bytes + 8), "cdrom0:\\X.IRX;1") != 0) return 6;
    if (snapshot[0] != 0 || bytes[259] != 0 || bytes[260] != 0) return 7;
    if (rpc_client != norrath_loadfile_client || rpc_operation != 6 || rpc_mode != 0) return 8;
    if (rpc_send != _senddata || rpc_receive != _senddata || rpc_send_size != 512 || rpc_receive_size != 8) return 9;
    if (rpc_completion != 0 || bind_calls != 1 || version_calls != 1) return 10;

    // Short argument: exactly the given length copied, bytes after it untouched.
    reset();
    unsigned char argument[300];
    for (int i = 0; i < 300; ++i) argument[i] = static_cast<unsigned char>(i + 1);
    rc = _sceSifLoadModule("m", 5, argument, &result, 0);
    if (snapshot[0] != 5 || std::memcmp(bytes + 260, argument, 5) != 0 || bytes[265] != 0xcc) return 11;
    if (bytes[8] != 'm' || bytes[9] != 0 || bytes[259] != 0) return 12;

    // Largest length that is copied as given, then the first length that is capped.
    reset();
    _sceSifLoadModule("m", 252, argument, &result, 0);
    if (snapshot[0] != 252 || std::memcmp(bytes + 260, argument, 252) != 0) return 13;
    reset();
    _sceSifLoadModule("m", 253, argument, &result, 0);
    if (snapshot[0] != 252 || std::memcmp(bytes + 260, argument, 252) != 0) return 14;
    reset();
    _sceSifLoadModule("m", 300, argument, &result, 0);
    if (snapshot[0] != 252) return 15;

    // A path longer than 251 characters is cut and forced terminated.
    reset();
    char longpath[400];
    std::memset(longpath, 'p', sizeof longpath);
    longpath[399] = 0;
    _sceSifLoadModule(longpath, 0, 0, &result, 0);
    if (bytes[8 + 250] != 'p' || bytes[259] != 0) return 16;

    // RPC failure leaves the result pointer alone.
    reset();
    call_result = -2;
    result = 0x7777;
    if (_sceSifLoadModule("m", 0, 0, &result, 0) != static_cast<int>(0xfffeffffu) || result != 0x7777) return 17;
    std::puts("_sceSifLoadModule passed");
    return 0;
}

#include "init.cpp"

#include <cstring>

extern "C" unsigned char PadData[256];
unsigned char PadData[256];
static int open_result;
static int seen_port, seen_slot, calls;
static void* seen_buffer;
extern "C" int scePadPortOpen(int port, int slot, void* buffer) {
    ++calls;
    seen_port = port;
    seen_slot = slot;
    seen_buffer = buffer;
    return open_result;
}

int main() {
    unsigned char object[64];
    std::memset(object, 0xa5, sizeof object);
    open_result = 0;
    if (Init__5VIPadii(object, 0, 1) != -1 || calls != 1) return 1;
    for (unsigned char byte : object) if (byte != 0xa5) return 2;  // nothing written on failure
    open_result = -1;
    if (Init__5VIPadii(object, 0, 1) != -1) return 3;
    for (unsigned char byte : object) if (byte != 0xa5) return 4;
    open_result = 2;  // only exactly 1 counts as success
    if (Init__5VIPadii(object, 0, 1) != -1) return 5;

    open_result = 1;
    std::memset(object, 0xa5, sizeof object);
    if (Init__5VIPadii(object, 1, 0) != 0) return 6;
    if (seen_port != 1 || seen_slot != 0 || seen_buffer != PadData) return 7;
    int port, slot, flag;
    std::memcpy(&port, object, 4);
    std::memcpy(&slot, object + 4, 4);
    std::memcpy(&flag, object + 52, 4);
    if (port != 1 || slot != 0 || flag != 1) return 8;
    for (int offset = 8; offset < 52; ++offset) {
        bool cleared = offset < 46 || offset >= 48;
        if (cleared ? object[offset] != 0 : object[offset] != 0xa5) return 9;
    }
    for (int offset = 56; offset < 64; ++offset) if (object[offset] != 0xa5) return 10;
    return 0;
}

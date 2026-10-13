// Observed: Init__5VIPadii at 0x010ff108 is 156 bytes. Arguments: object,
// port, slot. It calls scePadPortOpen(port, slot, PadData) where PadData is the
// buffer at 0x011e9480 (a local symbol in the image); the same buffer is
// passed for every pad. If the result is not 1 it returns -1 and writes
// nothing. Otherwise it stores the result (1) at offset 52, port at offset 0,
// slot at offset 4, zero in the words at offsets 48, 32 and 36, zero in the
// 64-bit words at offsets 8, 16 and 24, zero in the six bytes at offsets 40 to
// 45, and returns 0. Bytes 46 and 47 are not written.
// Inferred: offset 52 is an "opened" flag, because Close__5VIPad tests and
// clears it. Unknown: what the zeroed fields hold and the size of VIPad.
extern "C" {
extern unsigned char PadData[];
int scePadPortOpen(int port, int slot, void* buffer);

int Init__5VIPadii(void* object, int port, int slot) {
    int opened = scePadPortOpen(port, slot, PadData);
    if (opened != 1) return -1;
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<int*>(bytes + 52) = opened;
    *reinterpret_cast<int*>(bytes + 0) = port;
    *reinterpret_cast<int*>(bytes + 4) = slot;
    *reinterpret_cast<int*>(bytes + 48) = 0;
    *reinterpret_cast<long long*>(bytes + 8) = 0;
    *reinterpret_cast<long long*>(bytes + 16) = 0;
    *reinterpret_cast<long long*>(bytes + 24) = 0;
    *reinterpret_cast<int*>(bytes + 32) = 0;
    *reinterpret_cast<int*>(bytes + 36) = 0;
    for (int index = 40; index < 46; ++index) bytes[index] = 0;
    return 0;
}
}

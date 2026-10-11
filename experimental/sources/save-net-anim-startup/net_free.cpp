// Observed: netFree returns immediately while netInitState is zero.
// Otherwise it clears that byte, closes a non-negative broadcast connection,
// optionally brings the interface down, terminates the library, clears the
// mode byte, blanks the partner bytes, and writes a 128.0f pattern into
// four stride-17632 records. The image stops the record walk when the
// cursor reaches partners+16+70528.
// Inferred: the unaligned and aligned copy loops write the same bytes.
// Unknown: who fills cd and g_net_buf before this runs.

extern "C" unsigned char netInitState;
extern "C" int broadcastConnection;
extern "C" unsigned char cd[];
extern "C" unsigned char g_net_buf[];
extern "C" unsigned char netInterfaceIsUp;
extern "C" unsigned char netNetworkMode;
extern "C" unsigned char netNmPartners;
extern "C" unsigned char partners[];
extern "C" int nmPartners;

extern "C" int sceInetClose(unsigned char* client, unsigned char* buffer,
                            int connection, int timeout);
extern "C" int sceInetCtlDownInterface(unsigned char* client, unsigned char* buffer,
                                       int mode);
extern "C" int sceLibnetTerminate(unsigned char* client);
extern "C" void* memset(void* destination, int value, unsigned size);

static void store_f128(unsigned char* dest) {
    float value = 128.0f;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
    dest[0] = bytes[0];
    dest[1] = bytes[1];
    dest[2] = bytes[2];
    dest[3] = bytes[3];
}

static void copy_bytes(unsigned char* dest, const unsigned char* source, int count) {
    for (int index = 0; index < count; ++index) {
        dest[index] = source[index];
    }
}

static void write_record(unsigned char* record) {
    unsigned char block[160];
    memset(block + 0, 0, 16);
    memset(block + 16, 0, 16);
    memset(block + 32, 0, 16);
    memset(block + 48, 0, 16);
    memset(block + 64, 0, 16);
    block[72] = 0;
    block[73] = 0;
    block[74] = 0;
    block[75] = 0;
    store_f128(block + 8);
    store_f128(block + 24);
    store_f128(block + 40);
    store_f128(block + 56);
    copy_bytes(record + 16, block, 64);
    copy_bytes(record + 80, block + 64, 8);
    copy_bytes(record + 88, block + 72, 4);

    memset(block + 80, 0, 16);
    memset(block + 96, 0, 16);
    memset(block + 112, 0, 16);
    memset(block + 128, 0, 16);
    memset(block + 144, 0, 16);
    block[152] = 0;
    block[153] = 0;
    block[154] = 0;
    block[155] = 0;
    store_f128(block + 88);
    store_f128(block + 104);
    store_f128(block + 120);
    store_f128(block + 136);
    copy_bytes(record + 96, block + 80, 64);
    copy_bytes(record + 160, block + 144, 8);
    copy_bytes(record + 168, block + 152, 4);
}

extern "C" void netFree__Fb(int bring_down) {
    if (netInitState == 0) {
        return;
    }
    int connection = broadcastConnection;
    netInitState = 0;
    if (connection >= 0) {
        sceInetClose(cd, g_net_buf, connection, 1000);
        broadcastConnection = -1;
    }
    if (bring_down != 0) {
        sceInetCtlDownInterface(cd, g_net_buf, 0);
        netInterfaceIsUp = 0;
    }
    sceLibnetTerminate(cd);
    netNetworkMode = 0;
    netNmPartners = 1;
    memset(partners, 0, 0x11380u);
    for (int slot = 0; slot < 4; ++slot) {
        write_record(partners + slot * 17632);
    }
    nmPartners = 0;
}

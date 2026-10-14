// Observed: Read__5VIPadR12VIConsolePad at 0x010fec38 is 1128 bytes and returns
// an int. Arguments: this, out. Offsets below are in this unless marked out.
// 1. If the word at 36 is nonzero: scePadGetReqState(port, slot) (port is the
//    word at 0, slot the word at 4). A result of 2 returns 0 immediately; 0
//    clears word 36; any other value also clears word 32 and word 36.
// 2. state = scePadGetState(port, slot), stored at offset 88. For a state other
//    than 2 or 6 it takes the not-ready path (5 below); otherwise:
// 3. id = scePadInfoMode(port, slot, 1, 0); if id is nonzero, ext =
//    scePadInfoMode(port, slot, 2, 0) replaces it when positive.
//    If word 32 is -1: r = scePadInfoAct(port, slot, -1, 0) is stored in word
//    32, then word 32 is cleared unless r is 2, id is 7, scePadInfoAct(.., 0, 3)
//    is 0, scePadInfoAct(.., 1, 3) is positive and scePadSetActAlign(port, slot,
//    {0, 1, 0xff, 0xff, 0xff, 0xff}) returns 1; in that full-success case word
//    36 is also set to 1.
// 4. If id is not 7: scePadSetMainMode(port, slot, 1, 3) and return 0. Otherwise
//    scePadRead(port, slot, this + 56) must return a positive count and the byte
//    at 56 must be zero, else return 0. Then out floats 1 to 4 (offsets 4, 8, 12
//    and 16) get ScaleStick__5VIPadUc(this, byte) for the bytes at 60 to 63, out
//    floats 5 to 20 get 0.0 or 1.0 from inverted bits of the bytes at 58 and 59
//    (mapping in the table below), and the function returns 1.
// 5. Not-ready path: if word 32 is 2, bytes 40 and 41 are cleared and
//    scePadSetActDirect(port, slot, this + 40) is called. Bytes 40 and 41 are
//    cleared, word 32 gets -1, word 48 gets 0, the 64-bit words at 16 and 24
//    get 0, printf("pad not ready (%s)\n", names[state]) runs with the table
//    {"DISCONNECT", "FINDPAD", "FINDCTP1", "", "", "EXECCMD", "STABLE", "ERROR"},
//    and it returns 0. The states in that table follow the scePad state
//    numbering; out is untouched on this path.
// Out float index = byte offset / 4: pressed means the source bit is 0.
//   byte 59: bit 4 -> 5, bit 7 -> 6, bit 6 -> 7, bit 5 -> 8, bit 2 -> 13,
//            bit 0 -> 14, bit 3 -> 15, bit 1 -> 16
//   byte 58: bit 7 -> 9, bit 5 -> 10, bit 4 -> 11, bit 6 -> 12, bit 3 -> 17,
//            bit 0 -> 18, bit 1 -> 19, bit 2 -> 20
// Inferred: the buffer at 56 is the 8-byte scePadRead result (status, id, two
// button bytes, four stick bytes), from the SDK call and the way the bytes are
// consumed; the float indices follow the usual DualShock button bit order, not
// any game name. Unknown: VIConsolePad field names, the word at 36 meaning, and
// the byte at 57.
extern "C" {
int scePadGetReqState(int port, int slot);
int scePadGetState(int port, int slot);
int scePadInfoMode(int port, int slot, int term, int offset);
int scePadInfoAct(int port, int slot, int act, int offset);
int scePadSetActAlign(int port, int slot, const unsigned char* data);
int scePadSetMainMode(int port, int slot, int mode, int lock);
int scePadRead(int port, int slot, void* buffer);
int scePadSetActDirect(int port, int slot, void* data);
float ScaleStick__5VIPadUc(void* object, unsigned char value);
int printf(const char* format, ...);

static const unsigned char act_align[6] = {0, 1, 0xff, 0xff, 0xff, 0xff};

static float pressed(unsigned char byte, int bit) {
    return static_cast<float>((~static_cast<int>(byte) >> bit) & 1);
}

int Read__5VIPadR12VIConsolePad(void* object, void* out_pad) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    int* words = static_cast<int*>(object);
    float* out = static_cast<float*>(out_pad);
    const char* names[8] = {"DISCONNECT", "FINDPAD", "FINDCTP1", "", "", "EXECCMD", "STABLE", "ERROR"};
    int port = words[0];
    int slot = words[1];

    if (words[9] != 0) {
        int request = scePadGetReqState(port, slot);
        if (request != 0) {
            if (request == 2) return 0;
            words[8] = 0;
        }
        words[9] = 0;
    }

    int state = scePadGetState(port, slot);
    words[22] = state;
    if (state != 2 && state != 6) {
        if (words[8] == 2) {
            bytes[40] = 0;
            bytes[41] = 0;
            scePadSetActDirect(port, slot, bytes + 40);
        }
        bytes[40] = 0;
        bytes[41] = 0;
        words[8] = -1;
        words[12] = 0;
        *reinterpret_cast<long long*>(bytes + 16) = 0;
        *reinterpret_cast<long long*>(bytes + 24) = 0;
        printf("pad not ready (%s)\n", names[state]);
        return 0;
    }

    int id = scePadInfoMode(port, slot, 1, 0);
    if (id != 0) {
        int ext = scePadInfoMode(port, slot, 2, 0);
        if (ext > 0) id = ext;
    }
    if (words[8] == -1) {
        int act = scePadInfoAct(port, slot, -1, 0);
        words[8] = act;
        if (act != 2) {
            words[8] = 0;
        } else if (id != 7) {
            words[8] = 0;
        } else if (scePadInfoAct(port, slot, 0, 3) != 0) {
            words[8] = 0;
        } else if (scePadInfoAct(port, slot, 1, 3) <= 0) {
            words[8] = 0;
        } else {
            int align = scePadSetActAlign(port, slot, act_align);
            if (align != 1) {
                words[8] = 0;
            } else {
                words[9] = align;
            }
        }
    }
    if (id != 7) {
        scePadSetMainMode(port, slot, 1, 3);
        return 0;
    }

    unsigned char* buffer = bytes + 56;
    if (scePadRead(port, slot, buffer) <= 0) return 0;
    if (buffer[0] != 0) return 0;

    out[1] = ScaleStick__5VIPadUc(object, bytes[60]);
    out[2] = ScaleStick__5VIPadUc(object, bytes[61]);
    out[3] = ScaleStick__5VIPadUc(object, bytes[62]);
    out[4] = ScaleStick__5VIPadUc(object, bytes[63]);
    out[5] = pressed(bytes[59], 4);
    out[6] = static_cast<float>((bytes[59] ^ 0x80) >> 7);
    out[7] = pressed(bytes[59], 6);
    out[8] = pressed(bytes[59], 5);
    out[9] = static_cast<float>((bytes[58] ^ 0x80) >> 7);
    out[12] = pressed(bytes[58], 6);
    out[10] = pressed(bytes[58], 5);
    out[11] = pressed(bytes[58], 4);
    out[13] = pressed(bytes[59], 2);
    out[15] = pressed(bytes[59], 3);
    out[14] = pressed(bytes[59], 0);
    out[16] = pressed(bytes[59], 1);
    out[17] = pressed(bytes[58], 3);
    out[18] = pressed(bytes[58], 0);
    out[19] = pressed(bytes[58], 1);
    out[20] = pressed(bytes[58], 2);
    return 1;
}
}

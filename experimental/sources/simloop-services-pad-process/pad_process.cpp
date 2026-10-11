// Observed padProcess__Fv at 0x00167e68, 708 bytes.
//
// padFrame.756 at 0x00484064 is incremented on every entry. The MIPS div
// remainder of the value before that store selects the name refresh: remainder
// 0 refreshes, any other remainder does not. The refresh asks scePadGetSlotMax
// for ports 0 and 1 and stores the low bytes at tapCount (0x00484060, 2 bytes).
// Port 1 is then cleared when that low byte, sign-extended, is greater than 1.
//
// Four pad records of 88 bytes start at pad (0x0034beb8). For record i, the
// port is 0 while i is signed-less than tapCount[0]; otherwise the port is 1
// and the slot is i minus that count. When the selected tap count is
// signed-less than 2, the record is named with the bytes "%d" and
// padIndexMap[port] + 1. Otherwise it is named with "%d-%c", the port plus 1,
// and padIndexMap[slot] + 65. Those format bytes sit at 0x00484068 and
// 0x00484070 and have no symbol of their own. padIndexMap is 0x00484078.
//
// Every record then calls padGetUnprocessedData with padIndexMap[i] and a
// stack buffer. A zero result decreases the word at record+4, stopping at 0.
// A nonzero result stores 30 there. padProcessData follows, with stateData
// (0x006e1c40) advanced by 8. While the byte at netNetworkMode is 0, the 88
// record bytes are copied to netLocalPad (0x00332fd0) at the same stride.
// The copy is 64 bytes plus 24 bytes. A nonzero mode byte skips the copy.
extern "C" {
extern unsigned char pad[];
extern unsigned char stateData[];
extern unsigned char tapCount[2];
extern unsigned char padIndexMap[4];
extern unsigned char netLocalPad[];
extern unsigned char netNetworkMode;
extern unsigned pad_frame asm("padFrame.756");

int scePadGetSlotMax(int port);
int sprintf(char* destination, const char* format, ...);
int padGetUnprocessedData__FiP18padUnprocessedData(int index, void* data);
void padProcessData__FP3padP18padUnprocessedDataP12padStateData(void* record, void* data,
                                                                void* state);

static int sign_byte(unsigned value) {
    const unsigned byte = value & 0xffu;
    return byte < 128u ? static_cast<int>(byte) : static_cast<int>(byte) - 256;
}

static void copy_bytes(unsigned char* destination, const unsigned char* source, unsigned count) {
    for (unsigned index = 0; index < count; ++index) destination[index] = source[index];
}

void padProcess__Fv() {
    const int previous = static_cast<int>(pad_frame);
    pad_frame = static_cast<unsigned>(previous + 1);
    if (previous % 180 == 0) {
        tapCount[0] = static_cast<unsigned char>(scePadGetSlotMax(0));
        const int second = scePadGetSlotMax(1);
        tapCount[1] = static_cast<unsigned char>(second);
        if (sign_byte(static_cast<unsigned>(second)) >= 2) tapCount[1] = 0;

        for (int index = 0; index < 4; ++index) {
            const int count0 = sign_byte(tapCount[0]);
            int port = 1;
            int slot = index - count0;
            if (index < count0) {
                port = 0;
                slot = index;
            }
            char* const name = reinterpret_cast<char*>(pad + index * 88);
            if (sign_byte(tapCount[port]) >= 2) {
                const int letter = sign_byte(padIndexMap[slot]) + 65;
                sprintf(name, "%d-%c", port + 1, letter);
            } else {
                sprintf(name, "%d", sign_byte(padIndexMap[port]) + 1);
            }
        }
    }

    unsigned char data[16];
    for (int index = 0; index < 4; ++index) {
        const int got = padGetUnprocessedData__FiP18padUnprocessedData(sign_byte(padIndexMap[index]), data);
        unsigned char* const record = pad + index * 88;
        int timer = static_cast<int>(record[4] | (record[5] << 8) | (record[6] << 16) | (record[7] << 24));
        if (got == 0) {
            timer -= 1;
            if (timer < 0) timer = 0;
        } else {
            timer = 30;
        }
        record[4] = static_cast<unsigned char>(timer);
        record[5] = static_cast<unsigned char>(static_cast<unsigned>(timer) >> 8);
        record[6] = static_cast<unsigned char>(static_cast<unsigned>(timer) >> 16);
        record[7] = static_cast<unsigned char>(static_cast<unsigned>(timer) >> 24);
        padProcessData__FP3padP18padUnprocessedDataP12padStateData(record, data, stateData + index * 8);
        if (sign_byte(netNetworkMode) == 0) copy_bytes(netLocalPad + index * 88, record, 88u);
    }
}
}

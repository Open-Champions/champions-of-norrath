// Observed textMessageProcess__Fv at 0x002cacb8, 632 bytes.
// Uses the byte block outMsg (0x008d88b8): byte 0 and byte 1 are counters
// named here len and pos, byte 2 holds a player index, and bytes from 3 on are
// the edit cells. The start state written by textInit has len 1, pos 0 and a
// space in cell 3.
//
// When netNetworkMode (signed byte, 0x00483829) is zero or the word chatOn
// (0x004865dc) is zero, every pending key from keyboardGetNextKey is discarded
// until it returns -1, and the function returns. Otherwise it returns at once
// if frontEndRunning (0x00483f30) is nonzero. Then:
//  - outMsg byte 2 := the low byte of netGetLocalPlayerNmSafe(0);
//  - for each of the eight 126-byte entries of inMsg (0x008d8938) whose byte 2
//    is not 0xfe, the matching word of msgTimeout (0x008d8d28) is incremented;
//  - if the word waiting (0x00486dac) is nonzero, return;
//  - otherwise keys from keyboardGetNextTranslatedKey are handled until -1:
//      8  : when pos != 0, memmove(&outMsg[pos + 2], &outMsg[pos + 3],
//           len - pos + 1), then pos and len decrease by one;
//      13 : when outMsg byte 2 is not 0xff, call textSend and return;
//      17 : when pos != 0, cell (pos + 3) := cell (pos + 2), pos decreases;
//      18 : when pos != len - 1, cell (pos + 3) := cell (pos + 4), pos grows;
//      26 : when pos != len - 1, memmove(&outMsg[pos + 3], &outMsg[pos + 4],
//           len - pos), then len decreases by one;
//      any other value, when len < 122: memmove(&outMsg[pos + 4],
//           &outMsg[pos + 3], len - pos + 1), cell (pos + 3) := low byte of
//           the key, pos and len grow by one.
extern "C" signed char netNetworkMode;
extern "C" int chatOn;
extern "C" int frontEndRunning;
extern "C" int waiting;
extern "C" unsigned char outMsg[];
extern "C" signed char inMsg[8][126];
extern "C" int msgTimeout[8];

extern "C" int keyboardGetNextKey__Fv();
extern "C" int keyboardGetNextTranslatedKey__Fv();
extern "C" int netGetLocalPlayerNmSafe__Fi(int index);
extern "C" void textSend__Fv();
extern "C" void* memmove(void* destination, const void* source, unsigned count);

extern "C" void textMessageProcess__Fv() {
    if (netNetworkMode == 0 || chatOn == 0) {
        while (keyboardGetNextKey__Fv() != -1) {
        }
        return;
    }
    if (frontEndRunning != 0) return;
    outMsg[2] = static_cast<unsigned char>(netGetLocalPlayerNmSafe__Fi(0));
    for (int i = 0; i < 8; i++) {
        if (inMsg[i][2] != -2) msgTimeout[i]++;
    }
    if (waiting != 0) return;
    int key;
    while ((key = keyboardGetNextTranslatedKey__Fv()) != -1) {
        if (key == 8) {
            if (outMsg[1] != 0) {
                memmove(&outMsg[outMsg[1] + 2], &outMsg[outMsg[1] + 3], outMsg[0] - outMsg[1] + 1);
                outMsg[1] = outMsg[1] - 1;
                outMsg[0] = outMsg[0] - 1;
            }
        } else if (key == 13) {
            if (static_cast<signed char>(outMsg[2]) != -1) {
                textSend__Fv();
                return;
            }
        } else if (key == 17) {
            if (outMsg[1] != 0) {
                outMsg[outMsg[1] + 3] = outMsg[outMsg[1] + 2];
                outMsg[1] = outMsg[1] - 1;
            }
        } else if (key == 18) {
            if (outMsg[1] != outMsg[0] - 1) {
                outMsg[outMsg[1] + 3] = outMsg[outMsg[1] + 4];
                outMsg[1] = outMsg[1] + 1;
            }
        } else if (key == 26) {
            if (outMsg[1] != outMsg[0] - 1) {
                memmove(&outMsg[outMsg[1] + 3], &outMsg[outMsg[1] + 4], outMsg[0] - outMsg[1]);
                outMsg[0] = outMsg[0] - 1;
            }
        } else if (outMsg[0] < 122) {
            memmove(&outMsg[outMsg[1] + 4], &outMsg[outMsg[1] + 3], outMsg[0] - outMsg[1] + 1);
            unsigned char position = outMsg[1];
            outMsg[1] = position + 1;
            outMsg[position + 3] = static_cast<unsigned char>(static_cast<unsigned short>(key));
            outMsg[0] = outMsg[0] + 1;
        }
    }
}

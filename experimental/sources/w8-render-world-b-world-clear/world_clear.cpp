// Observed worldClear__Fv at 0x0017c398, 172 bytes.
// Clears world-level state and returns nothing:
//   0x00484190 routeClearStart, 0x00484198 rqTail, 0x00484194 rqHead,
//   0x004841c0 worldWaterRainTimer: each set to zero (word stores);
//   0x0048418c routeEnable (one byte) set to 1.
// Then 64 records of 48 bytes starting at worldAnimation (0x006ebb40) have the
// byte at +4 cleared and the word at +40 cleared. Then 16 records of 68 bytes
// starting at DynamicLightArray (0x006e9d60) are visited in order: the address
// of that record plus 8 is passed to lightDeactivate__FP5Light, and afterwards
// the words at record+0 and record+4 are cleared.
// Names come from the executable's symbol table. Nothing else is touched;
// the meaning of the fields is not established here.
extern "C" {
extern int routeClearStart;
extern int rqTail;
extern int rqHead;
extern int worldWaterRainTimer;
extern unsigned char routeEnable;
extern unsigned char worldAnimation[];
extern unsigned char DynamicLightArray[];
void lightDeactivate__FP5Light(void* light);
void worldClear__Fv() {
    routeClearStart = 0;
    rqTail = 0;
    rqHead = 0;
    worldWaterRainTimer = 0;
    routeEnable = 1;
    for (int i = 0; i < 64; ++i) {
        unsigned char* record = worldAnimation + i * 48;
        record[4] = 0;
        *reinterpret_cast<unsigned*>(record + 40) = 0u;
    }
    for (int i = 0; i < 16; ++i) {
        unsigned char* record = DynamicLightArray + i * 68;
        lightDeactivate__FP5Light(record + 8);
        *reinterpret_cast<unsigned*>(record) = 0u;
        *reinterpret_cast<unsigned*>(record + 4) = 0u;
    }
}
}

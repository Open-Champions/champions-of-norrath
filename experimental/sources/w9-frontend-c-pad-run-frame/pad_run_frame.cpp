// Observed padRunFrame__Fv at 0x00167c08, 604 bytes (pinned SLUS_205.65). Runs
// once per frame, before padProcess. It first lowers the word padFilter by one,
// floored at zero. Then for index 0..3 (24-byte records in rumphole, selected
// through the signed byte padIndexMap[index]):
//   - When netNetworkMode is nonzero the index is skipped unless
//     netPlayerIsLocal(index) is nonzero.
//   - record word 0, if nonzero, is decremented and actSendData[0] becomes 1;
//     otherwise actSendData[0] becomes 0.
//   - if the float at record+8 is above zero, the float at record+12 is
//     subtracted from it and the result is stored, then replaced by 0.0 if it
//     went negative.
//   - record word 1, if nonzero, is decremented and actSendData[1] becomes the
//     float at record+8 times 255.0 converted to an integer (cvt.w.s, low
//     byte); otherwise actSendData[1] becomes 0.
//   - record words 4 and 5 hold the pair last sent. If actSendData[0] equals
//     word 4 and actSendData[1] equals word 5 nothing more happens. Otherwise
//     both words take the new values and the pad is updated: with
//     mapped = padIndexMap[index], port 0 and slot mapped while index is
//     below tapCount[0], else port 1 and slot mapped minus tapCount[0]; in
//     network mode both are 0. When slot is signed-less than tapCount[port],
//     scePadSetActDirect(port, slot, actSendData) is called.
// Note the port is chosen from the loop index while the slot uses the mapped
// value; a paired run against the original (1500 randomized states) agreed.
// Inferred: words 0 and 1 are countdown ticks for the small and large motor;
// the record layout is shared with padBigRumble and padClear.
// Unknown: the intended meaning of the actSendData bytes beyond what the
// scePad library defines, and whether the loop bound 4 is a named constant.
extern "C" {
extern int padFilter;
extern signed char padIndexMap[4];
extern signed char netNetworkMode;
extern signed char tapCount[2];
extern unsigned rumphole[24];
extern unsigned char actSendData[8];

int netPlayerIsLocal__Fi(int index);
int scePadSetActDirect(int port, int slot, unsigned char* data);

static float load_float(const unsigned* slot) {
    float value;
    __builtin_memcpy(&value, slot, sizeof value);
    return value;
}

static void store_float(unsigned* slot, float value) {
    __builtin_memcpy(slot, &value, sizeof value);
}

void padRunFrame__Fv() {
    int filter = padFilter - 1;
    if (filter < 0) filter = 0;
    padFilter = filter;
    for (int index = 0; index < 4; ++index) {
        if (netNetworkMode != 0 && !netPlayerIsLocal__Fi(index)) continue;
        unsigned* record = rumphole + padIndexMap[index] * 6;
        if (record[0] != 0) {
            --record[0];
            actSendData[0] = 1;
        } else {
            actSendData[0] = 0;
        }
        record = rumphole + padIndexMap[index] * 6;
        float strength = load_float(record + 2);
        if (0.0f < strength) {
            strength -= load_float(record + 3);
            store_float(record + 2, strength);
            if (strength < 0.0f) store_float(record + 2, 0.0f);
        }
        record = rumphole + padIndexMap[index] * 6;
        if (record[1] != 0) {
            --record[1];
            actSendData[1] = static_cast<unsigned char>(static_cast<int>(load_float(record + 2) * 255.0f));
        } else {
            actSendData[1] = 0;
        }
        record = rumphole + padIndexMap[index] * 6;
        if (actSendData[0] == record[4]) {
            if (actSendData[1] == record[5]) continue;
        }
        record[4] = actSendData[0];
        record[5] = actSendData[1];
        int mapped = padIndexMap[index];
        int port = 1;
        int slot = mapped - tapCount[0];
        if (index < tapCount[0]) {
            port = 0;
            slot = mapped;
        }
        if (netNetworkMode != 0) {
            port = 0;
            slot = 0;
        }
        if (slot < tapCount[port]) scePadSetActDirect(port, slot, actSendData);
    }
}
}

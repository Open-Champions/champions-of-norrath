// Observed: VIMemCardFormat__Fv at 0x010d4000 is 60 bytes. It calls
// sceMcFormat(0, 0), then sceMcSync(0, &cmd, &result) with two stack words,
// and returns 0 when the result word is zero and -1 otherwise. The sync
// return value is not examined.
// Inferred: the two zero arguments are port 0, slot 0 of the card library.
// Unknown: whether any caller formats other than port 0 slot 0, and what
// result values a failed format yields.
extern "C" int sceMcFormat(int port, int slot);
extern "C" int sceMcSync(int mode, int* cmd, int* result);

extern "C" int VIMemCardFormat__Fv() {
    int result;
    int cmd;
    sceMcFormat(0, 0);
    sceMcSync(0, &cmd, &result);
    return result != 0 ? -1 : 0;
}

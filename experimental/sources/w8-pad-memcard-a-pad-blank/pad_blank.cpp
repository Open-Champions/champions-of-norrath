// Observed: padBlank at 0x004f3600 is 48 bytes of zeroed BSS, read as four
// 12-byte records by netBlankController__Fii and netBlankControllerButtons__Fiii
// (index scaled by 12 from this base). Each record is three words.
// Inferred: one record per local pad.
// Unknown: which code consumes the records, and the meaning of the first word.
struct PadBlankRecord {
    int count;
    int buttons;
    int flag;
};

extern "C" {
PadBlankRecord padBlank[4];
}

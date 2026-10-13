// Observed patchSortedListWithLocalTime__Fv at 0x002564c8, 480 bytes. Called by the save
// browser (runBrowser) between a fade in and a fade out.
// For index 0 up to a count word (the word at 0x004861e8, re-read after every entry; the
// loop is skipped when it is not above zero) the routine converts two timestamps of the
// 64-byte record sortedDir[index] (0x007f7d40) into 8-byte clock images and stores both in
// the 16-byte record sortedTimes[index] (0x007f8000):
//   record bytes 10, 11, 12, 13 and halfword 14   -> sortedTimes[index] + 0
//   record bytes  2,  3,  4,  5 and halfword  6   -> sortedTimes[index] + 8
// Each image is built in an 8-byte local: byte 0 and byte 1 zero, byte 2 BCD of the first
// record byte, byte 3 BCD of the second, byte 5 BCD of the third, byte 6 BCD of the
// fourth, byte 7 BCD of (halfword % 100). BCD of a value is ((value / 10) << 4) | (value % 10)
// truncated to a byte. Byte 4 is never written. sceScfGetLocalTimefromRTC(local) is then
// called on it, and the eight bytes of the local are copied out (low image first).
// Inferred: the layout matches the SCE memory-card directory entry (two date-time stamps
// of seconds-free minute/hour/day/month/year) and the SCE CD clock image (second, minute,
// hour, pad, day, month, year after a status byte), so the call is read as converting RTC
// (Japan) time to the console's local time. The mapping is inferred from field offsets only.
// Unknown: which stamp is creation and which is modification; the count word's identity
// (it is written at the end of generateSortedListAndInfoHeaders but has no symbol of its
// own and is not sortedDirCount at 0x00485be8); the content of byte 4 when the first
// image is made (the image leaves it as stack garbage; this version starts it at zero and
// keeps it across passes, as the stack slot would).
extern "C" {
extern int norrath_world_c_view_4861e8;
extern unsigned char sortedDir[];
extern unsigned char sortedTimes[];
void sceScfGetLocalTimefromRTC(unsigned char* clock);
}

namespace {
unsigned char bcd(unsigned value) {
    return static_cast<unsigned char>(((value / 10u) << 4) | (value % 10u));
}

unsigned halfword(const unsigned char* p) { return static_cast<unsigned>(p[0]) | (static_cast<unsigned>(p[1]) << 8); }

void convert(unsigned char* clock, const unsigned char* stamp, unsigned char* out) {
    clock[0] = 0;
    clock[1] = 0;
    clock[2] = bcd(stamp[0]);
    clock[3] = bcd(stamp[1]);
    clock[5] = bcd(stamp[2]);
    clock[6] = bcd(stamp[3]);
    clock[7] = bcd(halfword(stamp + 4) % 100u);
    sceScfGetLocalTimefromRTC(clock);
    for (int i = 0; i < 8; ++i) out[i] = clock[i];
}
}  // namespace

extern "C" void patchSortedListWithLocalTime__Fv() {
    unsigned char clock[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int index = 0; index < norrath_world_c_view_4861e8; ++index) {
        const unsigned char* record = sortedDir + index * 64;
        unsigned char* times = sortedTimes + index * 16;
        convert(clock, record + 10, times);
        convert(clock, record + 2, times + 8);
    }
}

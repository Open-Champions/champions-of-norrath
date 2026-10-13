// Observed msg_trigger__8Innoruuki at 0x001ff508, 92 bytes.
// Copies the nine bytes of the text "Innoruuk" (with its terminator) into bossMeterName, stores the
// float at object+560 converted to an integer in bossMeterCurrent, calls soundResetStream__Fi with
// 0, then soundStartMusic__FPci with the stream file name "mx_boss_act5_innoruuk_l.vag" and 259.
// The incoming value is not used and nothing is returned.
// Unknown: what the trigger value means. Nothing here ran on the EE.
extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void soundResetStream__Fi(int stream);
void soundStartMusic__FPci(char* name, int mode);
}

namespace {
struct Name9 { char text[9]; };
}

extern "C" void msg_trigger__8Innoruuki(void* self, int value) {
    (void)value;
    static const Name9 name = {{'I', 'n', 'n', 'o', 'r', 'u', 'u', 'k', '\0'}};
    *reinterpret_cast<Name9*>(bossMeterName) = name;
    bossMeterCurrent = static_cast<int>(*reinterpret_cast<float*>(static_cast<unsigned char*>(self) + 560));
    soundResetStream__Fi(0);
    soundStartMusic__FPci(const_cast<char*>("mx_boss_act5_innoruuk_l.vag"), 259);
}

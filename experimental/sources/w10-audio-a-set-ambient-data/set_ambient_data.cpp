// Observed: soundSetAmbientData__FPc at 0x0024dea0 is 1320 bytes; the argument is the text of an
// ambient-sound description. It zeroes nmAmbientGroups and returns on a null text. Otherwise it clears
// ambientGroup (10880 bytes, sixteen 680-byte groups) and lower-cases the text in place (a byte whose
// _ctype_ entry has bit 0 set gets 32 added). It then parses lines: a line is the text after any
// leading CR/LF bytes up to the next CR, LF or end (no bound is checked; the original
// buffer is 160 bytes). A line starting with ';' is skipped everywhere.
// A group starts with a header line: "min-max" (a '-' present, scanned with " %d-%d ", both values
// required) or "min" (scanned with " %d ", the max is then the min); a header that does not scan
// is dropped. For group g (g = nmAmbientGroups) the header stores min at +0 and max at +4, steps
// eRandState (state * 1664525 + 1013904223) and stores (int)(60.0f * (min + (top 16 bits / 65536)
// * (max - min))) at +12. The following lines are numbers (" %d ", first goes to +16, later ones
// to +20), then lines containing ".vag" (each scanned with " %s " into the next 41-byte entry at
// +24, byte 40 of the entry set to zero, entry count kept for +8). The first line after the entries
// (or the first that is neither a number before any entry nor a .vag line) ends the group:
// nmAmbientGroups is incremented, the entry count stored at +8 and that line is parsed again as
// the next header.
// Inferred: nothing beyond the layout already recorded in runAmbient__Fv's notes.
// Unknown: whether more than sixteen groups or entries were ever meant to be accepted (nothing
// bounds them); the meaning of the numbers at +16 and +20 (runAmbient passes them to
// animGetVolumeScale__Fii).
extern "C" {
extern unsigned char ambientGroup[];
extern int nmAmbientGroups;
extern unsigned eRandState;
extern const unsigned char _ctype_[];
void* memset(void* destination, int value, unsigned size);
char* strchr(const char* text, int c);
char* strstr(const char* text, const char* pattern);
int sscanf(const char* text, const char* format, ...);

static float bits(unsigned value) {
    float out;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&out);
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
    return out;
}

static void set_word(unsigned char* p, int value) {
    const unsigned v = static_cast<unsigned>(value);
    p[0] = static_cast<unsigned char>(v);
    p[1] = static_cast<unsigned char>(v >> 8);
    p[2] = static_cast<unsigned char>(v >> 16);
    p[3] = static_cast<unsigned char>(v >> 24);
}

static char* read_line(char* text, char* line) {
    while (*text == '\n' || *text == '\r') ++text;
    while (*text && *text != '\n' && *text != '\r') *line++ = *text++;
    *line = 0;
    return text;
}

void soundSetAmbientData__FPc(char* text) {
    nmAmbientGroups = 0;
    if (!text) return;
    memset(ambientGroup, 0, 10880);
    for (char* c = text; *c; ++c)
        if (_ctype_[*c + 1] & 1) *c += 32;
    char line[160];
    int low, high, number;
    bool pending = false;
    while (*text) {
        if (!pending) text = read_line(text, line);
        pending = false;
        if (line[0] != ';') {
            if (strchr(line, '-')) {
                if (sscanf(line, " %d-%d ", &low, &high) != 2) continue;
            } else {
                if (sscanf(line, " %d ", &low) != 1) continue;
                high = low;
            }
            unsigned char* group = ambientGroup + nmAmbientGroups * 680;
            eRandState = eRandState * 1664525u + 1013904223u;
            set_word(group, low);
            set_word(group + 4, high);
            const float scaled = static_cast<float>(eRandState >> 16) * bits(0x37800000u);
            set_word(group + 12, static_cast<int>(
                (static_cast<float>(low) + scaled * static_cast<float>(high - low)) * bits(0x42700000u)));
            int count = 0, offset = 0;
            bool entries = false, first_number = false;
            for (;;) {
                text = read_line(text, line);
                if (line[0] == ';') continue;
                if (!entries && sscanf(line, " %d ", &number) == 1) {
                    if (!first_number) {
                        first_number = true;
                        set_word(group + 16, number);
                    } else {
                        set_word(group + 20, number);
                    }
                    continue;
                }
                if (strstr(line, ".vag")) {
                    entries = true;
                    ++count;
                    sscanf(line, " %s ", group + 24 + offset);
                    group[24 + offset + 40] = 0;
                    offset += 41;
                    continue;
                }
                break;
            }
            ++nmAmbientGroups;
            set_word(group + 8, count);
            pending = true;
        }
    }
}
}

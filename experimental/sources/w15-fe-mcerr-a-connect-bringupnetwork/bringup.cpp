// Observed: BringUpNetwork__12VIWndConnect at 0x010a25e0 is 1056 bytes. With this (words are 32-bit; i = the
// word at +1916, entries = the pointer word at +1972, entry = entries + 620 * i):
//  1. a 104-byte local `combination` is copied from entry + 516;
//  2. VIMemCardStatus__FPi(0): a nonzero result copies the two words at +1188 to a local pair, calls
//     McErr__FP5VIWndRC8VIVect2Ii(this, &pair, 4), stores 0 at +1920 and returns -1;
//  3. r = ezNetCtlSetCombination("mc0:", combination + 8):
//     - r = -8: the same McErr call (a fresh copy of the pair), 0 at +1920 and return -1;
//     - any other negative r: stores 0 at +1920 and +1924; w = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(
//       ui + 56, ui word +72, ui word +80, entry + 260) with ui = the word _5VIWnd$mUI, raised to at least 300;
//       a 512-byte 16-bit text is built as the 8-bit quote mark, the 8-bit name at entry + 260 and the 8-bit
//       text "\"\n" (str8to16 then two strcat16 calls); when the name contains "SCE/Ethernet (Network Adaptor)"
//       or "SCE/Modem (Network Adaptor)" (strstr, first one tested first) the text is replaced by
//       snprintf16(text, 512, GetString__13VIStringTableii(ui, 1026, ui word +48)) (the string is the format),
//       otherwise GetString(ui, 780, column) is appended with strcat16; then
//       Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(this + 40, GetString(ui, 767, column), text,
//       0, &pair, w + 80) with a copy of the two words at +1188 as pair, the word at this + 68 (window word 7)
//       is set to 1 and SetFocus__5VIWndP5VIWndb(this, this + 40, 1) is called;
//     - r >= 0: p = __wrap_memalign(64, 4928); the 64-bit value at +1976 is set to 180000;
//       ezNetCnfGetProperties("mc0:", combination + 8, p): when it returns 0 the +1976 value becomes 120000 if
//       any of the bytes p[2048], p[2304], p[2560] is nonzero and 45000 otherwise; __wrap_free(p);
//       ezNetCtlDownInterface(0); ezNetCtlUpInterface(0); the 64-bit value at +1984 becomes
//       GetTime__7VITimer() + the value at +1976.
//     The original returns -1 only on the two McErr paths; on the others the result is whatever the last call
//     left in the result register (not preserved here: zero is returned).
// Inferred: this brings up the network interface for the selected configuration entry (620-byte records),
// using the memory-card combination file "mc0:"; failures name the adapter in a message box; the +1976 and +1984
// values are a timeout length (180000, 120000 or 45000, presumably milliseconds) and its deadline.
// Unknown: the layout of the 620-byte entry and of the combination (only the offsets used are known), what the
// bytes at 2048, 2304 and 2560 of the properties are, the field names, the meaning of mode 4 for McErr (as in
// SaveAndExit), and the return value outside the -1 paths.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
extern unsigned _5VIWnd$mUI;
int VIMemCardStatus__FPi(int* value);
void McErr__FP5VIWndRC8VIVect2Ii(void* window, const int* size, int screen);
int ezNetCtlSetCombination(const char* name, void* combination);
int ezNetCnfGetProperties(const char* name, void* combination, void* properties);
int ezNetCtlDownInterface(int index);
int ezNetCtlUpInterface(int index);
void* __wrap_memalign(unsigned alignment, unsigned size);
void __wrap_free(void* block);
long long GetTime__7VITimer();
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(void* font, void* raster, void* dictionary, const char* text);
void str8to16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* source);
void strcat16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* dest, unsigned size, const unsigned short* source);
int snprintf16__8VIStringPUsiPCUse(unsigned short* dest, int size, const unsigned short* format, ...);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
char* strstr(const char* haystack, const char* needle);
void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* self, const unsigned short* title, const unsigned short* text,
                                                              int prompt, const int* size, int width);
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(NORRATH_TRANSLATE(word));
}
static inline const unsigned short* table_text(int id) {
    void* const ui = pointer_of(_5VIWnd$mUI);
    return GetString__13VIStringTableii(ui, id, static_cast<int*>(ui)[12]);
}

int BringUpNetwork__12VIWndConnect(void* self) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* const entry = static_cast<unsigned char*>(pointer_of(words[493])) + words[479] * 620;
    unsigned char combination[104];
    __builtin_memcpy(combination, entry + 516, sizeof combination);
    if (VIMemCardStatus__FPi(0) != 0) {
        int size[2] = {words[297], words[298]};
        McErr__FP5VIWndRC8VIVect2Ii(self, size, 4);
        words[480] = 0;
        return -1;
    }
    const int result = ezNetCtlSetCombination("mc0:", combination + 8);
    if (result >= 0) {
        void* const properties = __wrap_memalign(64, 4928);
        *reinterpret_cast<long long*>(bytes + 1976) = 180000;
        if (ezNetCnfGetProperties("mc0:", combination + 8, properties) == 0) {
            const char* const p = static_cast<const char*>(properties);
            *reinterpret_cast<long long*>(bytes + 1976) = (p[2048] != 0 || p[2304] != 0 || p[2560] != 0) ? 120000 : 45000;
        }
        __wrap_free(properties);
        ezNetCtlDownInterface(0);
        ezNetCtlUpInterface(0);
        *reinterpret_cast<long long*>(bytes + 1984) = GetTime__7VITimer() + *reinterpret_cast<long long*>(bytes + 1976);
        return 0;
    }
    if (result == -8) {
        int size[2] = {words[297], words[298]};
        McErr__FP5VIWndRC8VIVect2Ii(self, size, 4);
        words[480] = 0;
        return -1;
    }
    words[480] = 0;
    words[481] = 0;
    void* const ui = pointer_of(_5VIWnd$mUI);
    int width = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(
        static_cast<unsigned char*>(ui) + 56, pointer_of(static_cast<unsigned*>(ui)[18]), pointer_of(static_cast<unsigned*>(ui)[20]),
        reinterpret_cast<const char*>(entry + 260));
    if (width < 300) width = 300;
    unsigned short text[256];
    str8to16__8VIStringPUsUiPCc(text, 512, "\"");
    strcat16__8VIStringPUsUiPCc(text, 512, reinterpret_cast<const char*>(entry + 260));
    strcat16__8VIStringPUsUiPCc(text, 512, "\"\n");
    if (strstr(reinterpret_cast<const char*>(entry + 260), "SCE/Ethernet (Network Adaptor)") != nullptr ||
        strstr(reinterpret_cast<const char*>(entry + 260), "SCE/Modem (Network Adaptor)") != nullptr) {
        snprintf16__8VIStringPUsiPCUse(text, 512, table_text(1026));
    } else {
        strcat16__8VIStringPUsUiPCUs(text, 512, table_text(780));
    }
    int size[2] = {words[297], words[298]};
    unsigned char* const window = bytes + 40;
    Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(window, table_text(767), text, 0, size, width + 80);
    *reinterpret_cast<int*>(window + 28) = 1;
    SetFocus__5VIWndP5VIWndb(self, window, true);
    return 0;
}
}

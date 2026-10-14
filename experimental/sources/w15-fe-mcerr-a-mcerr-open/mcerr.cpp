// Observed: McErr__FP5VIWndRC8VIVect2Ii at 0x0109fb68 is 260 bytes. With (parent, size, screen) it
//  1. calls scePrintf("McErr (sWndMcErr = %d, Parent = %d, InitialScreen = %d)\n", sWndMcErr, parent, screen);
//  2. in the global error window sWndMcErr (0x01190a20) stores screen at +412, the two words of *size at
//     +420 and +424, parent at +428 and zero at +432;
//  3. sends message 4352 (0x1100) to parent, then sets the global mcErrOn (0x01190c30) to 1, then sends
//     message 4356 (0x1104) to parent, then clears mcErrOn. Each message goes through the entry at index 6
//     (offset 48) of the table whose address is the word at parent+32: a 16-bit adjustment to the object
//     pointer, then the function at +4, called as (parent + adjustment, sWndMcErr, message, sWndMcErr, 0);
//  4. calls scePrintf("VIWndMcErr::GotoScreen %d\n", screen), stores screen at +412 again and clears the words
//     at +408 and +416. It returns zero.
// Inferred: this opens the memory-card error window over a parent window at a given size and first screen
// number (the screen numbers are the ones the ProcessScreenNN handlers cover); the table word is reloaded
// before the second message because the first call may change it. The two messages bracket mcErrOn so that
// focus changes during the first one go to the error window.
// Unknown: what messages 4352 and 4356 mean, the names of the fields at +408, +416, +420 and +424, and
// whether the original returned a value on purpose (zero is returned here as observed).
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
int scePrintf(const char* format, ...);
extern unsigned char sWndMcErr[];
extern int mcErrOn;

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, void*, int, void*, int);
};

static inline void send_message(void* target, int message) {
    unsigned char* object = static_cast<unsigned char*>(target);
    const VtEntry* entry =
        reinterpret_cast<const VtEntry*>(NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(object + 32))) + 6;
    entry->function(object + entry->delta, sWndMcErr, message, sWndMcErr, 0);
}

int McErr__FP5VIWndRC8VIVect2Ii(void* parent, const int* size, int screen) {
    int* words = reinterpret_cast<int*>(sWndMcErr);
    scePrintf("McErr (sWndMcErr = %d, Parent = %d, InitialScreen = %d)\n", sWndMcErr, parent, screen);
    words[103] = screen;
    words[105] = size[0];
    words[106] = size[1];
    words[107] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(parent));
    words[108] = 0;
    send_message(parent, 4352);
    mcErrOn = 1;
    send_message(parent, 4356);
    mcErrOn = 0;
    scePrintf("VIWndMcErr::GotoScreen %d\n", screen);
    words[103] = screen;
    words[102] = 0;
    words[104] = 0;
    return 0;
}
}

/* playSoundForDlg__FP9DlgPacket at 0x001d1b30 is 336 bytes. The packet's text
 * starts at byte 49; the symbol table gives these globals as `char`-sized:
 * animLoadingComplete (0x0048501d), animLoadBuffer (0x0048501e),
 * animUseBuffer (0x0048501f), animStarted (0x00485020) and waitForSound
 * (0x0048501c). `anim` (0x007d0dc8) is read as two 16-byte slots indexed by
 * animLoadBuffer: word 0 is a load destination, word 1 a present flag, word 2
 * a size and word 3 a size taken from the sound.
 *
 * Observed, in order:
 *
 *   1. While animLoadingComplete is zero, cdProcess is called.
 *   2. "%s.vag" (rodata 0x00485028) of the text is formatted into a local;
 *      soundStartDialogStream__FiPci starts stream 2 on it with flags 42.
 *   3. soundGetDialogSize of that name is stored in word 3 of the slot picked
 *      by animLoadBuffer; waitForSound becomes 1.
 *   4. "%s.anm" (rodata 0x00485030) of the text is formatted into a second
 *      local and looked up with soundFindDialog.
 *      - Found: the span is the record's word at 144 minus its word at 64;
 *        soundGetDialogFilename names the file; cdLoadAsync (file, word 0 of
 *        the slot, &animLoadingComplete, span, the record's word at 64) is
 *        called; the slot's word 2 takes the span and word 1 becomes 1.
 *      - Not found: animLoadingComplete becomes 1 and the slot's word 1
 *        becomes 0.
 *   5. animUseBuffer takes the (re-read) animLoadBuffer; animLoadBuffer
 *      becomes 1 if that value was 0, else 0; animStarted becomes 0.
 *
 * Unknown: what the .anm record is, and the meaning of stream 2 and flags 42.
 */
extern "C" {
extern signed char animLoadingComplete, animLoadBuffer, waitForSound;
extern unsigned char animUseBuffer, animStarted;
extern int anim[];
void cdProcess__Fv();
int sprintf(char* destination, const char* format, ...);
void soundStartDialogStream__FiPci(int stream, char* name, int flags);
int soundGetDialogSize__FPc(char* name);
unsigned char* soundFindDialog__FPc(char* name);
char* soundGetDialogFilename__Fv();
void cdLoadAsync__FPcPvT0ii(char* file, void* destination, void* done_flag, int length, int offset);

static int load_int(const unsigned char* slot) {
    int value;
    __builtin_memcpy(&value, slot, sizeof value);
    return value;
}

void playSoundForDlg__FP9DlgPacket(unsigned char* packet) {
    char name[64];
    char animation[64];
    while (!animLoadingComplete) cdProcess__Fv();
    const char* text = reinterpret_cast<const char*>(packet + 49);
    sprintf(name, "%s.vag", text);
    soundStartDialogStream__FiPci(2, name, 42);
    int* slot_after_size = anim + animLoadBuffer * 4;
    slot_after_size[3] = soundGetDialogSize__FPc(name);
    waitForSound = 1;
    sprintf(animation, "%s.anm", text);
    unsigned char* record = soundFindDialog__FPc(animation);
    unsigned char use;
    if (record) {
        const int span = load_int(record + 144) - load_int(record + 64);
        char* file = soundGetDialogFilename__Fv();
        cdLoadAsync__FPcPvT0ii(file, reinterpret_cast<void*>(anim[animLoadBuffer * 4]), &animLoadingComplete, span,
                               load_int(record + 64));
        use = animLoadBuffer;
        int* slot = anim + animLoadBuffer * 4;
        slot[2] = span;
        slot[1] = 1;
    } else {
        animLoadingComplete = 1;
        use = animLoadBuffer;
        anim[animLoadBuffer * 4 + 1] = 0;
    }
    animUseBuffer = use;
    animLoadBuffer = use == 0;
    animStarted = 0;
}
}

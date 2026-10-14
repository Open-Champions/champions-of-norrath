/* soundGetDialogDir__Fv at 0x0024f6c8 is 132 bytes. It formats the current
 * level name (engineCurrentLevelName, 0x0037f988) with "%s.lmp" (sdata
 * 0x004859a8) into an 80-byte local buffer. If lumpQuery__FPc accepts
 * that name it returns lumpFindResource__FPcT0 for the same name and the
 * resource "dialog.bin" (0x00474708). Otherwise it returns null.
 *
 * Observed but not reproduced: after the lookup the original reads the byte
 * at the result, and when it is positive walks 80-byte steps while the loop
 * counter is below the byte at each step. Nothing is written or returned
 * from that walk, so it has no effect except reading memory. A null result
 * from lumpFindResource is dereferenced by the original and is returned
 * unchanged here.
 */
extern "C" {
extern char engineCurrentLevelName[];
int sprintf(char* destination, const char* format, ...);
int lumpQuery__FPc(char* name);
unsigned char* lumpFindResource__FPcT0(char* lump, char* name);

unsigned char* soundGetDialogDir__Fv() {
    char lump[80];
    sprintf(lump, "%s.lmp", engineCurrentLevelName);
    if (!lumpQuery__FPc(lump)) return nullptr;
    return lumpFindResource__FPcT0(lump, const_cast<char*>("dialog.bin"));
}
}

/* soundGetDialogFilename__Fv at 0x0024f688 is 64 bytes. It formats the
 * current level name (engineCurrentLevelName, 0x0037f988, 80 bytes) with
 * the text "%s.vat" into a static buffer and returns that buffer. The buffer
 * is the file-local vatname at 0x007d80c0, 80 bytes to the next symbol.
 * Nothing guards against a level name that does not fit.
 */
extern "C" {
extern char engineCurrentLevelName[];
int sprintf(char* destination, const char* format, ...);

static char vatname[80];

char* soundGetDialogFilename__Fv() {
    sprintf(vatname, "%s.vat", engineCurrentLevelName);
    return vatname;
}
}

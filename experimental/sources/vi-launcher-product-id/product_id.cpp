// Observed VIMemCardSetProductId at 0x010d3f20, 36 bytes. It returns.
// strcpy copies the argument into sProductId at 0x011afa78, st_size 32.
// The file bytes are BASLUS-20470EQONLINE and a NUL, then zeros. The copy
// length is not limited here.
extern "C" {
char* strcpy(char* destination, const char* source);

static char sProductId[32] __asm__("sProductId") = "BASLUS-20470EQONLINE";

void VIMemCardSetProductId__FPCc(const char* text) {
    strcpy(sProductId, text);
}
}

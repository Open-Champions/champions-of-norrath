// Observed VIMemCardSetProductTitle at 0x010d3f58, 36 bytes. It returns.
// strcpy copies the argument into sProductTitle at 0x011afa98, st_size 64.
// The file bytes are "EverQuest Online", a newline, "Adventures", and a NUL.
// The copy length is not limited here.
extern "C" {
char* strcpy(char* destination, const char* source);

static char sProductTitle[64] __asm__("sProductTitle") = "EverQuest Online\nAdventures";

void VIMemCardSetProductTitle__FPCc(const char* text) {
    strcpy(sProductTitle, text);
}
}

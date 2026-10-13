// Observed lumpRename__FPcT0 at 0x00166268, 136 bytes. Walks the first nmLumps records of the lump
// table lumps (0x006cb280, 40 bytes per record, the name string first) in order; every record whose
// name is strcmp-equal to the first argument gets the second argument copied over its name with
// strcpy. It does not stop at the first match, does not check lengths, and returns nothing.
// Unknown: the size of the name field (the copy is unbounded in the original).
extern "C" {
extern int nmLumps;
extern unsigned char lumps[];
int strcmp(const char* left, const char* right);
char* strcpy(char* to, const char* from);
}

extern "C" void lumpRename__FPcT0(char* from, char* to) {
    unsigned char* record = lumps;
    for (int i = 0; i < nmLumps; ++i) {
        char* name = reinterpret_cast<char*>(record);
        record += 40;
        if (strcmp(name, from) == 0) strcpy(name, to);
    }
}

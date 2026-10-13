// Observed: ReloadWords__4VIUI at 0x0102fbc0 is 92 bytes. With the UI object as this it calls
// Unload__13VIStringTable(this), stores 0 at this+52, then calls Load__13VIStringTablePCc(this,
// "data\words.txt"). If the result is negative it returns -1. Otherwise it calls printf with
// "WORDS.TXT read from disc.\n" (no further arguments) and returns 0.
// Inferred: the word string table is the member at offset 0 of VIUI (the other two tables are at
// +16 and +32 according to the init routine). Unknown: what the word at +52 holds, and what a
// positive Load result means.
extern "C" {
struct VIStringTable;
void Unload__13VIStringTable(VIStringTable* table);
int Load__13VIStringTablePCc(VIStringTable* table, const char* name);
int printf(const char* format, ...);

int ReloadWords__4VIUI(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    VIStringTable* table = static_cast<VIStringTable*>(self);
    Unload__13VIStringTable(table);
    *reinterpret_cast<unsigned*>(bytes + 52) = 0;
    if (Load__13VIStringTablePCc(table, "data\\words.txt") < 0) return -1;
    printf("WORDS.TXT read from disc.\n");
    return 0;
}
}

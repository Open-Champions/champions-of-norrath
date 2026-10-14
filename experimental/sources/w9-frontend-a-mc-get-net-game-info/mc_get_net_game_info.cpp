// Observed: MC_GetNetGameInfo__FPiT0Pc at 0x0025aa60 is 268 bytes. Arguments are (int* first,
// int* second, char* name). The first record pointer is found as MC_GetRamPlayerPtr__Fi(0) does:
// ramSlot + 112 + Camera::GetSaveFootPrint(gameCam) + jumpGateSaveFootPrint__Fv() + 4104 + 14.
// textGetString__FPc is called with ramSlot itself as the key. The wide string it returns is
// narrowed into name by keeping the low byte of each half word up to and including the
// terminator. *second receives the signed byte at ramSlot + 108, and *first the signed byte at
// offset 88 of the first record. It returns nothing.
// Inferred: the ramSlot header start holds a text key (the slot title), and the byte at +108 is
// the difficulty stored there by updateRamSlot__Fii.
// Unknown: what the record byte at offset 88 means.

struct Camera;

extern "C" {
extern unsigned char ramSlot[];
extern Camera* gameCam;
int GetSaveFootPrint__6Camera(Camera* self);
int jumpGateSaveFootPrint__Fv();
unsigned short* textGetString__FPc(char* key);
}

extern "C" void MC_GetNetGameInfo__FPiT0Pc(int* first, int* second, char* name) {
    unsigned char* record = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam);
    record += jumpGateSaveFootPrint__Fv();
    record += 4104 + 14;
    const unsigned short* text = textGetString__FPc(reinterpret_cast<char*>(ramSlot));
    int i = 0;
    while (text[i] != 0) {
        name[i] = static_cast<char>(text[i]);
        i += 1;
    }
    name[i] = static_cast<char>(text[i]);
    *second = static_cast<signed char>(ramSlot[108]);
    *first = static_cast<signed char>(record[88]);
}

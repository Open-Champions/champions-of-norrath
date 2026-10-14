// Observed setExtraParams__FP8CreaturePc at 0x001d3970, 28 bytes.
// If the word curDlg (gp-3696) is zero, the first argument is stored to
// dialogCreature (gp-10728) and the second to dialogLumpName (gp-10724).
// A nonzero curDlg returns without a store. The role of the two words is
// taken from their symbol names only; nothing here shows how they are used.
extern "C" int curDlg;
extern "C" void* dialogCreature;
extern "C" char* dialogLumpName;

extern "C" void setExtraParams__FP8CreaturePc(void* creature, char* lump_name) {
    if (curDlg == 0) {
        dialogCreature = creature;
        dialogLumpName = lump_name;
    }
}

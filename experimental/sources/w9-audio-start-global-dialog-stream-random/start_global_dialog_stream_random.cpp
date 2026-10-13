/* soundStartGlobalDialogStreamRandom__FiPci at 0x0024f038 is 108 bytes. It
 * asks ResourceMap::get for the table of the 12-byte map at 0x007d8a90
 * (globalDialogDir; authored name norrath_sound_global_dialog_map_view) and,
 * when that table is not null, calls startDlgRandom__FP9dialogDirPciT1i with
 * the table, "mouth.vat" (rodata 0x00474630), then the stream, the name and
 * the flags, in that order. A null table does nothing. Nothing is returned.
 *
 * The original also tests the file name address against zero, which a
 * string literal never fails; that test has no effect and is not written.
 */
extern "C" {
extern unsigned char norrath_sound_global_dialog_map_view[];
unsigned char* get__11ResourceMap(void* map);
void startDlgRandom__FP9dialogDirPciT1i(unsigned char* table, char* file, int stream, char* name, int flags);

void soundStartGlobalDialogStreamRandom__FiPci(int stream, char* name, int flags) {
    unsigned char* table = get__11ResourceMap(norrath_sound_global_dialog_map_view);
    if (table == nullptr) return;
    startDlgRandom__FP9dialogDirPciT1i(table, const_cast<char*>("mouth.vat"), stream, name, flags);
}
}

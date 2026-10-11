/* globalDialogDir at 0x007d8a90 is .bss. The resource-map initializer
 * writes three words, 12 bytes. delaySoundList begins 16 bytes later; the
 * last 4 bytes of that span are not written by the initializer and are
 * not included. The name at 0x004747c8 is the C string mouth.lmp. The
 * extension at 0x00474708 is the C string dialog.bin. Bytes after each
 * NUL belong to later rodata and are not included.
 */
extern "C" {
alignas(16) unsigned char norrath_sound_global_dialog_map_view[12];
alignas(4) unsigned char norrath_sound_global_dialog_name_view[] = {
    'm', 'o', 'u', 't', 'h', '.', 'l', 'm', 'p', 0
};
alignas(4) unsigned char norrath_sound_global_dialog_extension_view[] = {
    'd', 'i', 'a', 'l', 'o', 'g', '.', 'b', 'i', 'n', 0
};
}

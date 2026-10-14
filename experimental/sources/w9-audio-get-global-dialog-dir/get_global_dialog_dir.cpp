/* soundGetGlobalDialogDir__Fv at 0x0024eb90 is 28 bytes. It returns what
 * ResourceMap::get gives for the 12-byte map at 0x007d8a90 (globalDialogDir),
 * and does nothing else. The original reaches the call as a tail jump.
 *
 * The map is the one soundFindGlobalDialog__FPc searches; the authored name
 * for its storage is norrath_sound_global_dialog_map_view. What the
 * returned table holds is described there (80-byte records).
 */
extern "C" {
extern unsigned char norrath_sound_global_dialog_map_view[];
unsigned char* get__11ResourceMap(void* map);

unsigned char* soundGetGlobalDialogDir__Fv() {
    return get__11ResourceMap(norrath_sound_global_dialog_map_view);
}
}

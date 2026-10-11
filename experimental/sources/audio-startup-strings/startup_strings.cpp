/* Startup strings. The format view at 0x00485978 is five 8-byte slots,
 * "a\n" through "e\n", which initialize_sound prints at offsets 0, 8, 16,
 * 24, and 32. The assert file at 0x00474668 is sound.cc. The condition
 * view at 0x004746e8 is two C strings, !batchIopBuffer and !IOPbuffer.
 * Bytes after these extents are other rodata or sdata and are not included.
 */
extern "C" {
alignas(8) unsigned char norrath_sound_startup_formats_view[40] = {
    'a', '\n', 0, 0, 0, 0, 0, 0,
    'b', '\n', 0, 0, 0, 0, 0, 0,
    'c', '\n', 0, 0, 0, 0, 0, 0,
    'd', '\n', 0, 0, 0, 0, 0, 0,
    'e', '\n', 0, 0, 0, 0, 0, 0
};
alignas(4) unsigned char norrath_sound_startup_assert_file_view[] = {
    's', 'o', 'u', 'n', 'd', '.', 'c', 'c', 0
};
alignas(4) unsigned char norrath_sound_startup_assert_conditions_view[] = {
    '!', 'b', 'a', 't', 'c', 'h', 'I', 'o', 'p', 'B', 'u', 'f', 'f', 'e', 'r', 0,
    '!', 'I', 'O', 'P', 'b', 'u', 'f', 'f', 'e', 'r', 0
};
}

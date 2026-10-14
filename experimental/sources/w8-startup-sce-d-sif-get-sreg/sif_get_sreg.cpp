/* Observed sceSifGetSreg at 0x002d7160, 24 bytes. Returns word `index` of the
 * soft-register array at 0x008debc0 (symbol soft_reg), imported here under
 * the name norrath_sound_system_register_words_view. No bound check. The
 * array length is not established by this function.
 */
extern "C" unsigned norrath_sound_system_register_words_view[];

extern "C" unsigned sceSifGetSreg(int index) {
    return norrath_sound_system_register_words_view[index];
}

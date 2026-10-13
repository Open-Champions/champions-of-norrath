/* Observed sceSifSetSreg at 0x002d7178, 24 bytes. Stores `value` into word `index` of
 * the soft-register array at 0x008debc0 (symbol soft_reg) and returns `value`. The array
 * is imported under the same name the authored sceSifGetSreg uses. No bound check. The
 * array length is not established by this function.
 */
extern "C" unsigned norrath_sound_system_register_words_view[];

extern "C" unsigned sceSifSetSreg(int index, unsigned value) {
    norrath_sound_system_register_words_view[index] = value;
    return value;
}

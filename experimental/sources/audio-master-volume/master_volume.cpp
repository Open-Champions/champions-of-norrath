/* masterVolume at 0x00486bd4 is a 4-byte .sbss word. The scale read by
 * norrath_sound_initialize_hardware is the .lit4 word at 0x00482020.
 * Those bits decode as 16383.0f. The following literal is not this word.
 */
#define NORRATH_WORD(value) \
    static_cast<unsigned char>(static_cast<unsigned>(value)), \
    static_cast<unsigned char>(static_cast<unsigned>(value) >> 8), \
    static_cast<unsigned char>(static_cast<unsigned>(value) >> 16), \
    static_cast<unsigned char>(static_cast<unsigned>(value) >> 24)

extern "C" {
alignas(4) unsigned char norrath_sound_master_volume_view[4];
alignas(4) unsigned char norrath_sound_master_scale_view[4] = {
    NORRATH_WORD(0x467ffc00u)
};
}

#undef NORRATH_WORD

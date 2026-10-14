/* Observed channelVoiceInfo at 0x0042ddb8, st_size 86. soundStopSound
 * at 0x0024cf80 reads it as 43 pairs: bank byte, then voice byte.
 * Banks are 0 for the first 24 pairs and 1 for the remaining 19.
 * Two padding bytes before effectModeSize are not part of this symbol.
 */
extern "C" {
alignas(8) unsigned char norrath_sound_channel_voice_map_view[86] = {
    0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0, 13, 0, 14, 0, 15, 0, 16, 0, 17, 0, 18, 0, 19, 0, 20, 0, 21, 0, 22, 0, 23, 1, 0, 1, 1, 1, 2, 1, 3, 1, 4, 1, 5, 1, 6, 1, 7, 1, 8, 1, 9, 1, 10, 1, 11, 1, 12, 1, 13, 1, 14, 1, 15, 1, 16, 1, 17,     1, 18
};
}

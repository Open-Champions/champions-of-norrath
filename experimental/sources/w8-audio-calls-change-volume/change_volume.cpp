/* soundChangeVolume__Fiff at 0x0024cc10 is 292 bytes.
 *
 * A channel of -1 returns at once. Otherwise both volumes are multiplied by
 * soundSfxVolume (0x00485930). If soundStereoEnable (0x00485938) is zero,
 * both become half their sum (the sum is formed after the scaling). Each is
 * multiplied by 16383.0f (the .lit4 word 0x467ffc00 at 0x00482028) and
 * truncated to an integer, then limited to 0..16383 (a value of 16384 or
 * more becomes 16383, a negative one becomes 0).
 *
 * The voice map (86 bytes, two signed bytes per channel at 0x0043ddb8) gives
 * word = byte[2 * channel] | (byte[2 * channel + 1] << 1). Two entries of 8
 * bytes are appended to batchBuffer (0x007dc280), each only if
 * nmBatchCommands (0x00485954) is below 512 at that moment, and each
 * increments the count:
 *
 *   halfword 1, halfword word,         word left value
 *   halfword 1, halfword word | 0x100, word right value
 *
 * The meaning of the command number and of the 0x100 flag is not established
 * here. A channel outside the 43 the map covers reads other bytes.
 */
extern "C" {
extern float soundSfxVolume;
extern int soundStereoEnable;
extern int nmBatchCommands;
extern unsigned char batchBuffer[];
extern unsigned char norrath_sound_channel_voice_map_view[];

static int limit(int value) {
    if (value >= 16384) value = 16383;
    if (value < 0) value = 0;
    return value;
}

static void append(int voice, int value) {
    if (nmBatchCommands < 512) {
        unsigned char* entry = batchBuffer + nmBatchCommands * 8;
        *reinterpret_cast<unsigned short*>(entry) = 1;
        *reinterpret_cast<unsigned short*>(entry + 2) = static_cast<unsigned short>(voice);
        *reinterpret_cast<int*>(entry + 4) = value;
        nmBatchCommands = nmBatchCommands + 1;
    }
}

void soundChangeVolume__Fiff(int channel, float left, float right) {
    if (channel == -1) return;
    left *= soundSfxVolume;
    right *= soundSfxVolume;
    if (soundStereoEnable == 0) {
        left = (left + right) * 0.5f;
        right = left;
    }
    const signed char* map = reinterpret_cast<const signed char*>(norrath_sound_channel_voice_map_view);
    const int voice = map[channel * 2] | (map[channel * 2 + 1] << 1);
    const int left_value = limit(static_cast<int>(left * 16383.0f));
    const int right_value = limit(static_cast<int>(right * 16383.0f));
    append(voice, left_value);
    append(voice | 0x100, right_value);
}
}

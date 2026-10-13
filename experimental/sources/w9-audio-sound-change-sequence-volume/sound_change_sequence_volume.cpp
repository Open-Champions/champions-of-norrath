/* soundChangeSequenceVolume__Fif at 0x0024d708 is 368 bytes. `id` is a
 * sequence handle as soundPlaySequence returns it: index in the low byte,
 * 0x8000 set while live. A handle of -1, or without bit 0x8000, returns at
 * once. Otherwise (index = id & 0xff, sequence records of 76 bytes at
 * 0x007d8110, authored name norrath_sound_sequence_records_view):
 *
 *   - the float at +64 of the record becomes `volume`, unconditionally;
 *   - each of the 43 channel records (20 bytes at 0x007d8f20, authored name
 *     norrath_sound_channel_records_view) is visited in order. A record
 *     takes part when bit 0x4 of its halfword at +4 is set and its signed
 *     halfword at +6 equals the index. For such a record, scale is
 *     `volume` times the float at +16; pan__FG6Point3ffPfT3 is called with
 *     the 12 bytes at +44 of the sequence record, the floats at +68 and
 *     +72 of it, and two result slots; both results are multiplied by
 *     scale, each limited from below to zero, and handed to
 *     soundChangeVolume__Fiff with the channel number.
 *
 * Nothing is returned. The record index is not bounds checked. What the
 * floats at +16 of a channel record and +68/+72 of a sequence record mean is
 * known only as far as pan uses them (the near and far distances in
 * soundPlaySequence's defaults, 360.0 and 840.0).
 */
extern "C" {
struct Point3 {
    float x, y, z;
};
extern unsigned char norrath_sound_sequence_records_view[];
extern unsigned char norrath_sound_channel_records_view[];
void pan__FG6Point3ffPfT3(Point3 position, float a, float b, float* left, float* right);
void soundChangeVolume__Fiff(int channel, float left, float right);

static float load_float(const unsigned char* slot) {
    float value;
    __builtin_memcpy(&value, slot, sizeof value);
    return value;
}

void soundChangeSequenceVolume__Fif(int id, float volume) {
    if (id == -1) return;
    if ((id & 0x8000) == 0) return;
    const int index = id & 0xff;
    unsigned char* sequence = norrath_sound_sequence_records_view + index * 76;
    __builtin_memcpy(sequence + 64, &volume, sizeof volume);
    const unsigned char* channel = norrath_sound_channel_records_view;
    for (int number = 0; number < 43; ++number, channel += 20) {
        unsigned short flags;
        short owner;
        __builtin_memcpy(&flags, channel + 4, sizeof flags);
        if ((flags & 0x4) == 0) continue;
        __builtin_memcpy(&owner, channel + 6, sizeof owner);
        if (owner != index) continue;
        const float scale = volume * load_float(channel + 16);
        Point3 position;
        __builtin_memcpy(&position, sequence + 44, sizeof position);
        float left, right;
        pan__FG6Point3ffPfT3(position, load_float(sequence + 68), load_float(sequence + 72), &left, &right);
        left *= scale;
        right *= scale;
        if (left < 0.0f) left = 0.0f;
        if (right < 0.0f) right = 0.0f;
        soundChangeVolume__Fiff(number, left, right);
    }
}
}

/* soundStopSound__Fi at 0x0024cf80 is 328 bytes. The argument is either a
 * channel number or a sequence id.
 *
 * -1 does nothing.
 *
 * A value with bit 15 (0x8000) set is a sequence id as returned by the
 * soundPlaySequence overloads: the low byte is the sequence index and bits
 * 8..14 are the id spinner value (the halfword stored at record offset 56).
 * If the sequence record's halfword at +56 equals that value, its byte +4
 * is cleared. Then each of the 43 channel records (20 bytes) whose
 * halfword +4 has bit 2 (0x4) set, whose signed halfword +6 equals the
 * sequence index and whose signed halfword +8 equals the spinner value is
 * stopped, by calling this function again with the channel number. The
 * channel scan runs whether or not the record matched.
 *
 * Otherwise the value is a channel number: the voice map (two signed bytes
 * per channel) gives a key word index and a bit; the bit is set in the
 * koffs word and cleared in the kons word, and the channel record's
 * halfword +4 is cleared.
 *
 * Channel numbers outside 0..42 index past the tables.
 */
extern "C" {
extern unsigned char sequence[];
extern unsigned char norrath_sound_channel_records_view[];
extern unsigned char norrath_sound_channel_voice_map_view[];
extern unsigned char norrath_sound_key_on_words_view[];
extern unsigned char norrath_sound_key_off_words_view[];

void soundStopSound__Fi(int id) {
    if (id == -1) return;
    if (id & 0x8000) {
        const int index = id & 0xff;
        const int spinner = (id >> 8) & 0x7f;
        unsigned char* record = sequence + index * 76;
        if (*reinterpret_cast<unsigned short*>(record + 56) == spinner) record[4] = 0;
        for (int channel = 0; channel < 43; ++channel) {
            const unsigned char* info = norrath_sound_channel_records_view + channel * 20;
            if ((*reinterpret_cast<const unsigned short*>(info + 4) & 4) &&
                *reinterpret_cast<const short*>(info + 6) == index &&
                *reinterpret_cast<const short*>(info + 8) == spinner)
                soundStopSound__Fi(channel);
        }
        return;
    }
    const signed char* map = reinterpret_cast<const signed char*>(norrath_sound_channel_voice_map_view);
    const int word = map[id * 2];
    const unsigned bit = 1u << (map[id * 2 + 1] & 31);
    unsigned* on = reinterpret_cast<unsigned*>(norrath_sound_key_on_words_view);
    unsigned* off = reinterpret_cast<unsigned*>(norrath_sound_key_off_words_view);
    const unsigned on_word = on[word];
    const unsigned off_word = off[word];
    *reinterpret_cast<unsigned short*>(norrath_sound_channel_records_view + id * 20 + 4) = 0;
    off[word] = off_word | bit;
    on[word] = on_word & ~bit;
}
}

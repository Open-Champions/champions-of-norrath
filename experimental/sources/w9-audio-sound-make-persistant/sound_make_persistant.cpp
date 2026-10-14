/* soundMakePersistant__Fi at 0x0024c5f8 is 172 bytes. A handle of -1 does
 * nothing. A handle with bit 0x8000 is a sequence (index = low byte;
 * 76-byte records at 0x007d8110, authored name
 * norrath_sound_sequence_records_view) and the record gets:
 *
 *   byte  +60  = 1
 *   float +16  = 1.0f
 *   word  +20  = 0
 *   float +24  = 1.0f
 *   word  +28  = 0
 *   halfword +36 = (old | 0x1) & 0xfffb
 *
 * Any other non-negative handle is a channel number: bit 0x1 of the halfword
 * at +4 of that 20-byte channel record (0x007d8f20, authored name
 * norrath_sound_channel_records_view) is set and nothing else changes. A
 * negative handle other than -1 and without bit 0x8000 does nothing. No
 * bounds are checked. Nothing is returned.
 *
 * Only the stores are observed; calling a sequence or channel "persistent"
 * is the function's name, and what the fields at +16..+36 of a sequence
 * record or bit 0x1 of a channel record control is not established here.
 */
extern "C" {
extern unsigned char norrath_sound_sequence_records_view[];
extern unsigned char norrath_sound_channel_records_view[];

void soundMakePersistant__Fi(int id) {
    if (id == -1) return;
    if (id & 0x8000) {
        unsigned char* sequence = norrath_sound_sequence_records_view + (id & 0xff) * 76;
        const float one = 1.0f;
        const int zero = 0;
        unsigned short flags;
        sequence[60] = 1;
        __builtin_memcpy(sequence + 16, &one, sizeof one);
        __builtin_memcpy(&flags, sequence + 36, sizeof flags);
        flags = static_cast<unsigned short>((flags | 0x1) & 0xfffb);
        __builtin_memcpy(sequence + 20, &zero, sizeof zero);
        __builtin_memcpy(sequence + 36, &flags, sizeof flags);
        __builtin_memcpy(sequence + 24, &one, sizeof one);
        __builtin_memcpy(sequence + 28, &zero, sizeof zero);
        return;
    }
    if (id < 0) return;
    unsigned char* channel = norrath_sound_channel_records_view + id * 20;
    unsigned short flags;
    __builtin_memcpy(&flags, channel + 4, sizeof flags);
    flags |= 0x1;
    __builtin_memcpy(channel + 4, &flags, sizeof flags);
}
}

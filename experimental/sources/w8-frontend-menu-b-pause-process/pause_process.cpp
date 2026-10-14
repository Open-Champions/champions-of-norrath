// Observed: pauseProcess__Fv at 0x0024b570 is 412 bytes. It follows
// gameIsPaused. When the game is paused and the byte `paused` is zero, it
// walks the 43 sound channel records (20 bytes each, halfword flags at +4).
// A record is held when flags & 5 is nonzero and flags & 8 is zero. For a
// held record it queues a batch command (type 1, argument zero) whose flags
// halfword is byte0 | byte1 << 1 | 0x200 of that channel's two voice-map
// bytes, then sets flag bit 2 in the record. Afterwards `paused` becomes 1.
// When the game is not paused and `paused` is set, every record whose flag
// bit 2 is set queues the same command with the record word at +12 as the
// argument, and the bit is cleared (stored back for every record). Then
// `paused` becomes 0. Commands are only appended while nmBatchCommands is
// below 512 (8-byte entries in batchBuffer).
// Inferred: type 1 with 0x200 is a voice level-or-key command; bit 2 means
// "silenced by pause". These are guesses from the paired stores only.
// Unknown: the command encoding on the IOP side.
extern "C" {
int gameIsPaused__Fv();
extern unsigned char norrath_sound_channel_records_view[];
extern unsigned char norrath_sound_channel_voice_map_view[];
extern unsigned char batchBuffer[];
extern int nmBatchCommands;
extern signed char paused;
}

namespace {
void append_command(unsigned flags, unsigned value) {
    if (nmBatchCommands < 512) {
        unsigned char* entry = batchBuffer + nmBatchCommands * 8;
        nmBatchCommands += 1;
        *reinterpret_cast<unsigned short*>(entry + 2) = static_cast<unsigned short>(flags);
        *reinterpret_cast<unsigned*>(entry + 4) = value;
        *reinterpret_cast<unsigned short*>(entry) = 1;
    }
}

unsigned short& flags_of(int channel) {
    return *reinterpret_cast<unsigned short*>(norrath_sound_channel_records_view + channel * 20 + 4);
}

unsigned voice_flags(int channel) {
    const signed char* voice = reinterpret_cast<const signed char*>(norrath_sound_channel_voice_map_view);
    return (static_cast<unsigned>(voice[channel * 2]) | (static_cast<unsigned>(voice[channel * 2 + 1]) << 1)) | 0x200u;
}
}

extern "C" void pauseProcess__Fv() {
    if (gameIsPaused__Fv() != 0 && paused == 0) {
        for (int channel = 0; channel < 43; ++channel) {
            unsigned flags = flags_of(channel);
            if ((flags & 5u) != 0 && (flags & 8u) == 0) {
                append_command(voice_flags(channel), 0);
                flags_of(channel) = static_cast<unsigned short>(flags | 2u);
            }
        }
        paused = 1;
    }
    if (gameIsPaused__Fv() == 0 && paused != 0) {
        for (int channel = 0; channel < 43; ++channel) {
            unsigned flags = flags_of(channel);
            if ((flags & 2u) != 0) {
                unsigned value = *reinterpret_cast<unsigned*>(norrath_sound_channel_records_view + channel * 20 + 12);
                append_command(voice_flags(channel), value);
            }
            flags_of(channel) = static_cast<unsigned short>(flags & ~2u);
        }
        paused = 0;
    }
}

/* soundClear__Fi at 0x0024e3c8 is 696 bytes. The argument is a lock level.
 *
 *   1. delaySoundList (1152 bytes) is cleared and `paused` (0x00485964) is
 *      set to zero.
 *   2. For each of the 43 channel records (20 bytes): word 0 and halfword 4
 *      are cleared. The voice map (two signed bytes per channel) gives a key
 *      word index and a bit; that bit is set in the koffs word and cleared
 *      in the kons word.
 *   3. nmSounds becomes the number of leading soundList records (16 bytes
 *      each) whose signed lock byte at offset 12 is greater than the level,
 *      stopping at the first record that is not, or at the old count. A
 *      count of zero or less becomes 0.
 *   4. For each of the 32 sequence records (76 bytes): if the signed byte at
 *      offset 4 is nonzero, soundStopSound__Fi gets
 *      index | (halfword at offset 56 << 8) | 0x8000. Then listDestroy is
 *      called on the list head at offset 40, in every case.
 *   5. sceSifCallRpc command 112 (0x70) three times, send sbuffin 16 bytes,
 *      receive sbuff 64 bytes, null end function and parameter. Before the
 *      calls sbuffin word 0 holds 0, 1, then 2 and word 1 holds 0.
 *   6. soundFadeStream__Fi for streams 0, 1, 2; soundSetAudioEffect with
 *      five zeros; soundFlushCommandBuffer__Fv.
 *
 * Not reproduced: the original copies 16 bytes from a stack local of which
 * only words 0 and 1 are written, so words 2 and 3 of the send buffer come
 * from stale stack. This source leaves sbuffin bytes 8..15 as they were.
 * The command number's meaning is not established.
 */
extern "C" {
extern unsigned char delaySoundList[];
extern unsigned char paused;
extern int nmSounds;
extern unsigned char soundList[];
extern unsigned char sequence[];
extern unsigned char norrath_sound_channel_records_view[];
extern unsigned char norrath_sound_channel_voice_map_view[];
extern unsigned char norrath_sound_key_on_words_view[];
extern unsigned char norrath_sound_key_off_words_view[];
extern unsigned char norrath_sound_rpc_client_view[];
extern unsigned char norrath_sound_rpc_send_view[];
extern unsigned char norrath_sound_rpc_receive_view[];
void* memset(void* destination, int value, unsigned long count);
void soundStopSound__Fi(int id);
void listDestroy__FP8ListHead(void* head);
void soundFadeStream__Fi(int stream);
void soundSetAudioEffect__Fiiiii(int a, int b, int c, int d, int e);
void soundFlushCommandBuffer__Fv();
int norrath_sound_call_rpc(void* client, int command, int mode, void* send, int send_bytes,
                           void* receive, int receive_bytes, void* end_function,
                           void* end_parameter);

static void send_command_112(int value) {
    int* words = reinterpret_cast<int*>(norrath_sound_rpc_send_view);
    words[0] = value;
    words[1] = 0;
    norrath_sound_call_rpc(norrath_sound_rpc_client_view, 112, 0, norrath_sound_rpc_send_view, 16,
                           norrath_sound_rpc_receive_view, 64, nullptr, nullptr);
}

void soundClear__Fi(int level) {
    memset(delaySoundList, 0, 1152);
    paused = 0;

    unsigned* on = reinterpret_cast<unsigned*>(norrath_sound_key_on_words_view);
    unsigned* off = reinterpret_cast<unsigned*>(norrath_sound_key_off_words_view);
    const signed char* map = reinterpret_cast<const signed char*>(norrath_sound_channel_voice_map_view);
    for (int channel = 0; channel < 43; ++channel) {
        unsigned char* record = norrath_sound_channel_records_view + channel * 20;
        *reinterpret_cast<int*>(record) = 0;
        *reinterpret_cast<unsigned short*>(record + 4) = 0;
        const int word = map[channel * 2];
        const unsigned bit = 1u << (map[channel * 2 + 1] & 31);
        off[word] |= bit;
        on[word] &= ~bit;
    }

    int kept = 0;
    if (nmSounds > 0) {
        const signed char* lock = reinterpret_cast<const signed char*>(soundList + 12);
        while (level < *lock) {
            ++kept;
            if (!(kept < nmSounds)) break;
            lock += 16;
        }
    }
    nmSounds = kept;

    for (int index = 0; index < 32; ++index) {
        unsigned char* record = sequence + index * 76;
        if (static_cast<signed char>(record[4]) != 0) {
            const unsigned half = *reinterpret_cast<unsigned short*>(record + 56);
            soundStopSound__Fi(static_cast<int>(index | (half << 8) | 0x8000u));
        }
        listDestroy__FP8ListHead(record + 40);
    }

    send_command_112(0);
    send_command_112(1);
    send_command_112(2);

    for (int stream = 0; stream < 3; ++stream) soundFadeStream__Fi(stream);
    soundSetAudioEffect__Fiiiii(0, 0, 0, 0, 0);
    soundFlushCommandBuffer__Fv();
}
}

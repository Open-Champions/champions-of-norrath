/* findChannel__Fv at 0x0024f910 is 100 bytes. It advances the cursor
 * nextChannel (0x00485948) by one, wrapping to 0 when the next value would
 * reach 43, and tests the halfword at offset 4 of that channel record
 * (43 records of 20 bytes at 0x007d8f20) against 0x5. A record with neither
 * bit set is taken: its index is returned and the cursor stays on it. The
 * search makes at most 44 steps; if every record has a bit set the result is
 * -1 and the cursor has moved 44 places.
 * The two bits are not named; they are read as "busy" only by inference
 * from this caller.
 */
extern "C" {
extern int nextChannel;
extern unsigned char norrath_sound_channel_records_view[];

int findChannel__Fv() {
    for (int attempt = 0; attempt < 44; ++attempt) {
        int next = nextChannel + 1;
        if (next >= 43) next = 0;
        nextChannel = next;
        const unsigned char* record = norrath_sound_channel_records_view + next * 20;
        unsigned flags = record[4] | (unsigned(record[5]) << 8);
        if ((flags & 5u) == 0) return next;
    }
    return -1;
}
}

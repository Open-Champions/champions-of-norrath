// Observed: generate_key_release_events at 0x002e80d8 is 276 bytes (a local symbol in
// the image) and returns an int. Arguments are a keyboard-library state block and a
// raw report. In the state, a signed word at 256 counts the keys held in the previous
// report and the bytes from 260 list them; a 32-byte bit map starts at 224; a
// halfword at 38 is the count of events already queued, which start at 40 in
// sixteen-byte records. In the report, a signed halfword at 16 counts the keys held
// now and the bytes from 20 list them. For each previously held key that is not
// in the current list, the key's bit (byte 224 + code / 8, bit code % 8) is cleared
// and, if fewer than 10 events are queued, a record is appended: word 0 = the
// report's first word, halfword 4 = code, halfword 8 = report byte 10, halfword 10
// = report byte 11, halfword 12 = 2. A full queue gives the result 0x8000000d for
// that key instead; a later key that does queue sets the result back to 0. The
// result is therefore only the status of the last previously held key that was not
// still held (0 when there were none). The word at 256 is read again after each key.
// Inferred: the layout matches the 16-byte events Read__10VIKeyboardR12VIConsoleKey
// consumes, and 2 is the key-up direction there (1 is key down); the 0x8000000d
// status is the one Read treats as a buffer overflow.
// Unknown: names of report byte 10 and 11 (the other consumer calls them flags A
// and flags B), and what the other state bytes hold.
extern "C" {
int generate_key_release_events(unsigned char* state, unsigned char* report) {
    int status = 0;
    int* held_count = reinterpret_cast<int*>(state + 256);
    int now_count = *reinterpret_cast<short*>(report + 16);
    for (int index = 0; index < *held_count; ++index) {
        unsigned char code = state[260 + index];
        bool still_held = false;
        for (int other = 0; other < now_count; ++other) {
            if (code == report[20 + other]) {
                still_held = true;
                break;
            }
        }
        if (still_held) continue;
        state[224 + (code >> 3)] = static_cast<unsigned char>(state[224 + (code >> 3)] & ~(1u << (code & 7)));
        unsigned short* queued = reinterpret_cast<unsigned short*>(state + 38);
        if (*queued < 10) {
            unsigned char* event = state + 40 + 16 * *queued;
            *reinterpret_cast<unsigned*>(event) = *reinterpret_cast<unsigned*>(report);
            status = 0;
            *reinterpret_cast<unsigned short*>(event + 4) = code;
            *reinterpret_cast<unsigned short*>(event + 8) = report[10];
            *reinterpret_cast<unsigned short*>(event + 10) = report[11];
            *reinterpret_cast<unsigned short*>(event + 12) = 2;
            *queued = static_cast<unsigned short>(*queued + 1);
        } else {
            status = static_cast<int>(0x8000000du);
        }
    }
    return status;
}
}

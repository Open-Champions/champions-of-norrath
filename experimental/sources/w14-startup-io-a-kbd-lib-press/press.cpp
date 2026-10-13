// Observed: generate_key_press_events at 0x002e81f0 is 272 bytes (a local symbol in
// the image) and returns an int. It mirrors generate_key_release_events (see that
// unit for the state and report layouts) with the roles swapped. For each key in the
// report's held list (signed halfword count at 16, bytes from 20) that is not in the
// state's previous list (signed word count at 256, bytes from 260), the key's bit in
// the 32-byte map at 224 is set and, if fewer than 10 events are queued, a record
// is appended at 40 + 16 * queued: word 0 = the report's first word, halfword 4 =
// code, halfword 8 = report byte 10, halfword 10 = report byte 11, halfword 12 = 1.
// A full queue gives the result 0x8000000d for that key instead (the bit is already
// set); a later key that does queue resets the result to 0. Keys already in the
// previous list leave the result alone, so it is the status of the last newly held
// key, or 0 when none.
// Inferred: 1 is the key-down direction Read__10VIKeyboardR12VIConsoleKey tests for.
// Unknown: the state bytes around these fields.
extern "C" {
int generate_key_press_events(unsigned char* state, unsigned char* report) {
    int status = 0;
    int now_count = *reinterpret_cast<short*>(report + 16);
    for (int index = 0; index < now_count; ++index) {
        unsigned char code = report[20 + index];
        bool already_held = false;
        int held_count = *reinterpret_cast<int*>(state + 256);
        for (int other = 0; other < held_count; ++other) {
            if (code == state[260 + other]) {
                already_held = true;
                break;
            }
        }
        if (already_held) continue;
        state[224 + (code >> 3)] = static_cast<unsigned char>(state[224 + (code >> 3)] | (1u << (code & 7)));
        unsigned short* queued = reinterpret_cast<unsigned short*>(state + 38);
        if (*queued < 10) {
            unsigned char* event = state + 40 + 16 * *queued;
            *reinterpret_cast<unsigned*>(event) = *reinterpret_cast<unsigned*>(report);
            status = 0;
            *reinterpret_cast<unsigned short*>(event + 4) = code;
            *reinterpret_cast<unsigned short*>(event + 8) = report[10];
            *reinterpret_cast<unsigned short*>(event + 10) = report[11];
            *reinterpret_cast<unsigned short*>(event + 12) = 1;
            *queued = static_cast<unsigned short>(*queued + 1);
        } else {
            status = static_cast<int>(0x8000000du);
        }
    }
    return status;
}
}

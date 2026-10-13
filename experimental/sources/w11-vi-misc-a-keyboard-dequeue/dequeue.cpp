// Observed: DequeueKeyEvent__10VIKeyboardP12VIConsoleKey at 0x002cc0b0 is 120 bytes.
// The keyboard holds a count at offset 12 and a pointer to a queue of two-byte
// records at offset 8. With a zero count it returns 0. Otherwise it copies the first
// record's two bytes to the output, decrements the count, shifts the remaining
// records down by one (a loop that re-reads the queue pointer and the count each
// time) and returns 1.
extern "C" {
int DequeueKeyEvent__10VIKeyboardP12VIConsoleKey(void* keyboard, void* out) {
    int* words = static_cast<int*>(keyboard);
    if (words[3] == 0) return 0;
    char* queue = reinterpret_cast<char*>(words[2]);
    char* record = static_cast<char*>(out);
    char first = queue[0];
    char second = queue[1];
    record[0] = first;
    record[1] = second;
    int remaining = words[3] - 1;
    words[3] = remaining;
    if (remaining > 0) {
        int index = 0;
        do {
            char* base = reinterpret_cast<char*>(words[2]) + 2 * index;
            ++index;
            char next0 = base[2];
            char next1 = base[3];
            base[0] = next0;
            base[1] = next1;
        } while (index < words[3]);
    }
    return 1;
}
}

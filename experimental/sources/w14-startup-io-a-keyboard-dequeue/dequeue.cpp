// Observed: DequeueKeyEvent__10VIKeyboardP12VIConsoleKey at 0x002cc0b0 is 120 bytes
// and returns an int. The word at this + 12 is a count and the word at this + 8
// points to an array of two-byte records. With a zero count it returns 0 and
// touches nothing. Otherwise the first record's two bytes are copied to the
// output, the count is decremented, the remaining records are moved down by one
// record (one record per step, while the step index is below the new count),
// and it returns 1.
// Inferred: a first-in first-out queue of {key, direction} records, matching the
// two-byte record Read__10VIKeyboardR12VIConsoleKey queues.
// Unknown: the queue capacity and who allocates the array.
// A host check defines NORRATH_HOST_ARENA because a 64-bit host cannot keep a target
// pointer in a four-byte word; the word then holds an arena address.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline signed char* queueAt(const int* words) {
    return reinterpret_cast<signed char*>(norrath_host_pointer(static_cast<unsigned>(words[2])));
}
#else
static inline signed char* queueAt(const int* words) {
    return *reinterpret_cast<signed char* const*>(&words[2]);
}
#endif

extern "C" {
int DequeueKeyEvent__10VIKeyboardP12VIConsoleKey(void* object, void* out) {
    int* words = static_cast<int*>(object);
    if (words[3] == 0) return 0;
    signed char* queue = queueAt(words);
    signed char* result = static_cast<signed char*>(out);
    result[0] = queue[0];
    result[1] = queue[1];
    words[3] = words[3] - 1;
    for (int index = 0; index < words[3]; ++index) {
        signed char* record = queueAt(words) + index * 2;
        record[0] = record[2];
        record[1] = record[3];
    }
    return 1;
}
}

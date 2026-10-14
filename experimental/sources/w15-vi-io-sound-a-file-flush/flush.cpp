// Observed: Flush__6VIFile at 0x010caf50, 40 bytes. With the file object as this, if the signed word at +0 is not negative it calls
// BufferFlush__6VIFile(this); the result of that call is not used. The function returns 0 either way.
// Inferred: the word at +0 is the descriptor of a disc file; a negative value marks a memory or segment-backed file (as in Read and
// Write__6VIFilePCvi), which has no buffer to flush.
// Unknown: the member name, and what BufferFlush returns.
extern "C" {
int BufferFlush__6VIFile(unsigned char* file);

int Flush__6VIFile(unsigned char* file) {
    if (*reinterpret_cast<int*>(file) >= 0) BufferFlush__6VIFile(file);
    return 0;
}
}

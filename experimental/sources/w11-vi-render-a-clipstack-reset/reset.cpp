// Observed: Reset__11VIClipStack at 0x01099530 is 20 bytes. It stores zero in the
// words at offsets 0, 12 and 24 of the object and returns 0.
// Inferred: those words are the stack counters of the clip stack; the Init
// routine calls this last. Unknown: which counter each word is.
extern "C" {
int Reset__11VIClipStack(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<int*>(bytes + 12) = 0;
    *reinterpret_cast<int*>(bytes + 0) = 0;
    *reinterpret_cast<int*>(bytes + 24) = 0;
    return 0;
}
}

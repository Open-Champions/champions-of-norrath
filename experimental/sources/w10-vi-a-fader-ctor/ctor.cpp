// Observed: __7VIFader at 0x0117cc20 is 12 bytes and returns nothing. It
// stores -1 in the word at offset 20 and writes nothing else.
// Inferred: offset 20 is the material index, because Free__7VIFader and
// Init__7VIFader treat it that way. Unknown: the size of VIFader.
extern "C" {
void __7VIFader(void* object) {
    static_cast<int*>(object)[5] = -1;
}
}

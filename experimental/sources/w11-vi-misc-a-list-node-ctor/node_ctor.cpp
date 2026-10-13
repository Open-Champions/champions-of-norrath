// Observed: __7VILNode at 0x01072670 is 12 bytes. It stores zero in the words at
// offsets 0 and 4 and returns nothing.
extern "C" {
void __7VILNode(void* node) {
    unsigned* words = static_cast<unsigned*>(node);
    words[1] = 0;
    words[0] = 0;
}
}

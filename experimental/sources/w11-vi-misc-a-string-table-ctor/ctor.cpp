// Observed: __13VIStringTable at 0x0101bb78 is 20 bytes. It stores zero in the four
// words at offsets 0, 4, 8 and 12 and returns nothing.
extern "C" {
void __13VIStringTable(void* table) {
    unsigned* words = static_cast<unsigned*>(table);
    words[3] = 0;
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
}
}

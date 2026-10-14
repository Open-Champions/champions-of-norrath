// Observed: __13VISpellEffect at 0x01000778 is 64 bytes. Only the first argument (this) is used. It stores zero to each of the twelve
// words at offsets 0 through 44, so the first 48 bytes are zeroed (word stores in the order 4, 0, 12, 20, 16, 24, 32, 28, 36, 8, 44, 40).
// Nothing is returned.
// Inferred: a default constructor that clears the 48-byte head of the object.
// Unknown: the field meanings and whether the object is larger than 48 bytes.
extern "C" {
void __13VISpellEffect(int* self) {
    for (int i = 0; i < 12; i++) self[i] = 0;
}
}

// Observed: playerCountChanged__6Camera at 0x0026fde0 is 16 bytes and returns nothing. It stores
// the single-float value 1.0 into the word at byte offset 200 of the object it is given (the
// first argument). Nothing else is read or written.
// Inferred: the argument is a Camera instance, from the mangled name; the word at +200 is a
// float, from the store of a float constant through the coprocessor-1 register.
// Unknown: what the field at +200 means to its readers.

extern "C" void playerCountChanged__6Camera(void* camera) {
    *reinterpret_cast<float*>(static_cast<unsigned char*>(camera) + 200) = 1.0f;
}

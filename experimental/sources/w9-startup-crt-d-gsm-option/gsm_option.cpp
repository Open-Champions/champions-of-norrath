// Observed gsm_option at 0x002edab8, 44 bytes. Arguments: the object from
// gsm_create, an option number, and a pointer to an int. Only option 3 is
// handled; any other number returns -1 without touching memory. For option 3
// the doubleword at object offset 2464 is read first and its low 32 bits are the
// return value (sign-extension only matters to the 64-bit caller). If the
// pointer is non-null, the int it points at is sign-extended into the whole
// doubleword. The old value is returned either way.
// Inferred: the field matches the public GSM 06.10 option "long-term predictor
// cut"; no field name exists in the image.
extern "C" {
int gsm_option(void* state, int option, int* value) {
    if (option != 3) return -1;
    long long* const field = reinterpret_cast<long long*>(static_cast<unsigned char*>(state) + 2464);
    const int previous = static_cast<int>(*field);
    if (value != 0) *field = *value;
    return previous;
}
}

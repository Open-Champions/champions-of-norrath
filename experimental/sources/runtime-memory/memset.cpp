// Independently authored byte-fill contract for the EE C runtime.
// Ordinary valid memory spans only; see REVIEW.md for original/target scope.
extern "C" void* memset(void* destination, int value, __SIZE_TYPE__ count) {
    unsigned char* cursor = static_cast<unsigned char*>(destination);
    const unsigned char byte = static_cast<unsigned char>(value);
    while (count != 0) {
        *cursor++ = byte;
        --count;
    }
    return destination;
}

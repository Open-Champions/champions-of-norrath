// specialHandler at 0x0048691c, four .sbss bytes (the next symbol, CDmode,
// is 4 bytes later). The image value is zero. The pre-logo-dialog unit's
// setter stores a function pointer here with the signature used below, and
// that signature is the only claim made about the object. The original is
// local; it is global here so that unit can resolve it.
extern "C" {
void (*specialHandler)(int code, const unsigned char* point);
}

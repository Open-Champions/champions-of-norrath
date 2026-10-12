// Observed strlen at 0x00304cfc, 312 bytes: the count of bytes before the
// first zero. Aligned inputs are scanned in 8- or 16-byte blocks first; the
// authored function is the byte result. The same original function is also
// authored as norrath_factory_strlen with its block scan.
extern "C" unsigned strlen(const char* text) {
    const char* cursor = text;
    while (*cursor != '\0') ++cursor;
    return static_cast<unsigned>(cursor - text);
}

// Observed: strlen16__8VIStringPCUs at 0x0101a998, 52 bytes. Takes a pointer to 16-bit units and returns the number of units
// before the first zero unit (the zero is not counted); reads one unit at a time from the start.
// Inferred: a static member of VIString (no object argument) measuring a 16-bit string; the result is an int count.
// Unknown: the maximum length, and behaviour on an unterminated string (it reads on until a zero unit).
extern "C" {
int strlen16__8VIStringPCUs(const unsigned short* text) {
    int length = 0;
    while (text[length] != 0) ++length;
    return length;
}
}

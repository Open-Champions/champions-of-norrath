// Observed: T__10VIUIWindowPCUs at 0x0103f2a8 is 8 bytes. With (this, text) it returns text unchanged and
// touches no memory; the object pointer is not read.
// Inferred: the 16-bit overload of the text helper needs no conversion, so it passes the string through
// (the narrow overload at 0x0103f270 converts into a shared buffer).
// Unknown: what the name "T" abbreviates (a translate/text helper is a guess from the two overloads only).
extern "C" const unsigned short* T__10VIUIWindowPCUs(void*, const unsigned short* text) {
    return text;
}

// Observed: SetPassword__20VIWndGenericRendererPCc at 0x010be240 is 36 bytes. With (this, text): when text is
// null it does nothing; otherwise it calls strncpy(this + 1156, text, 63) and returns nothing. The
// result of strncpy is dropped and no terminator is written by this routine.
// Inferred: the 64-byte field at +1156 holds the password text of the renderer window; 63 is the
// copy limit, leaving the last byte for the terminator the field's zero fill supplies (not verified here).
// Unknown: the field's name and its true size, and the class layout.
extern "C" {
char* strncpy(char* dest, const char* source, unsigned long count);

void SetPassword__20VIWndGenericRendererPCc(void* self, const char* text) {
    if (text != nullptr) {
        strncpy(static_cast<char*>(self) + 1156, text, 63);
    }
}
}

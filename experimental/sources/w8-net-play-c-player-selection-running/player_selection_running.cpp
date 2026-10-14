// Observed playerSelectionRunning__Fv at 0x00280508, 8 bytes: returns the word
// at playerSelRunning (defined by the session-roster state unit) with no other
// effect. The caller-visible type is a signed 32-bit value.
extern "C" {
extern int playerSelRunning;

int playerSelectionRunning__Fv() {
    return playerSelRunning;
}
}

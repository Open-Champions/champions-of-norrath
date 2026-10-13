// Observed setState__9Generatori at 0x001f20d8, 112 bytes.
// The new state is held in a stack word whose address goes to the Creature state processing call
// (setStateProcessing__8CreaturePi), which may change it; a nonzero result returns at once without
// storing. Otherwise the table of eleven cases at 0x46df00 maps states 5, 8, 9 and 10 to "keep":
// the halfword at +572 is left as it was. Every other state, including values of 11 or more
// (compared unsigned), stores the low 16 bits of the state into the halfword at +572.
// Unknown: what the states mean. Nothing here ran on the EE.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
}

extern "C" void setState__9Generatori(void* self, int state) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (setStateProcessing__8CreaturePi(self, &state)) return;
    if (static_cast<unsigned>(state) < 11u && (state == 5 || state >= 8)) return;
    *reinterpret_cast<unsigned short*>(object + 572) = static_cast<unsigned short>(state);
}

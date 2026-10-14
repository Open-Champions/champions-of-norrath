// Observed FindIopFunction__FUi at 0x011270d8, 80 bytes, a local function of the sound/IOP interface code
// loaded above the main image. Argument: a function code. It searches the pending-command buffer sSend
// (0x011edb00, 128 words) that SetIopFunction__FUii (0x01127040) appends to: word 0 is the number of queued
// commands, and from word 1 each command is a function code, a word count n and n argument words. The
// commands are walked in order (the stepping is 2 + n words); the first whose code equals the argument
// ends the search and the routine returns the address of its first argument word, that is, the address
// after the count word. A count of zero or less, or no match, returns null.
// The count word at the head is read once, before the walk.
// Inferred: this lets a caller merge into a command that is still waiting to be sent (see
// StopChannels__13VISoundDeviceUl). No bound is placed on the walk other than the command count; a
// corrupt count or word count would read past the buffer.
// Unknown: how the return type is declared in the original (the callers use an int pointer).
extern "C" {
extern int norrath_overlay_sSend_11edb00[];

int* FindIopFunction__FUi(unsigned function) {
    const int count = norrath_overlay_sSend_11edb00[0];
    if (count <= 0) return nullptr;
    int* entry = norrath_overlay_sSend_11edb00 + 1;
    int index = 0;
    do {
        const unsigned code = static_cast<unsigned>(entry[0]);
        const int words = entry[1];
        entry += 2;
        if (function == code) return entry;
        ++index;
        entry += words;
    } while (index < count);
    return nullptr;
}
}

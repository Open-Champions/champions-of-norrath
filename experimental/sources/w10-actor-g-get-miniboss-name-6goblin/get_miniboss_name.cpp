// Observed getMinibossName__6Goblin at 0x0022f530, 60 bytes.
// Formats the signed byte at object+76 into a 16-byte file-local buffer (the image's buffer.1100
// at 0x007d7b40, up to the next symbol at 0x007d7b50) with the template text "MBGoblin%d" through
// sprintf and returns the buffer. Inferred: +76 is a miniboss number (see initMiniboss__6Goblin).
// Nothing here ran on the EE.
extern "C" int sprintf(char* destination, const char* format, ...);

namespace {
char buffer[16];
}

extern "C" char* getMinibossName__6Goblin(void* self) {
    sprintf(buffer, "MBGoblin%d", static_cast<int>(*(static_cast<signed char*>(self) + 76)));
    return buffer;
}

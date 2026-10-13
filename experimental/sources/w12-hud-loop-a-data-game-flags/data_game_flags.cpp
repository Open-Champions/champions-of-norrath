// Observed gameFlags at 0x0034b898, 16 bytes, section .data (D).
// An initialised object of 16 bytes, all zero in the file. msg_hurt__6PlayerfP10GameObjecti and runHUD__Fi take its address and index it.
// Inferred: Four int flag words, zero at start (16 bytes is the ELF symbol size).
// Unknown: The meaning of each word.
extern "C" {
int gameFlags[4] = {0, 0, 0, 0};
}

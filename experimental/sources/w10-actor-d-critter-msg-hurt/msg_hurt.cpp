// Observed msg_hurt__7CritterfP10GameObjecti at 0x001f1908, 8 bytes.
// Returns at once: no call, no read, no write. The arguments (a float, a GameObject pointer and
// an int) are ignored. Unknown: why the slot is filled with an empty body. Nothing here ran on
// the EE.
extern "C" void msg_hurt__7CritterfP10GameObjecti(void* self, float amount, void* attacker, int flags) {
    (void)self; (void)amount; (void)attacker; (void)flags;
}

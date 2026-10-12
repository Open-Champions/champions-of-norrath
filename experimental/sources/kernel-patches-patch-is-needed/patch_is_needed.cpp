/* Observed PatchIsNeeded at 0x002dd9a8, 104 bytes. Four calls on two stack
 * words. GetOsdConfigParam fills the first word. The second word is that
 * value with bits 13..15 replaced by 1 (mask 0xffff1fff, then 0x2000), and
 * SetOsdConfigParam receives it. GetOsdConfigParam then overwrites the second
 * word, and SetOsdConfigParam receives the first word back. The result is 1
 * when bits 13..15 of the second word, as read back, are all zero, and 0
 * otherwise.
 * Inferred: the sequence writes a probe value, reads it back, and restores
 * the original value. A zero read-back means the field did not accept the
 * probe. InitExecPS2 patches the kernel only when this returns nonzero.
 * Unknown: what the OSD configuration word holds, and what the kernel does
 * with it.
 */
extern "C" {
void GetOsdConfigParam(unsigned* destination);
void SetOsdConfigParam(const unsigned* source);
int PatchIsNeeded() {
    unsigned words[2];
    GetOsdConfigParam(&words[0]);
    words[1] = (words[0] & 0xffff1fffu) | 0x2000u;
    SetOsdConfigParam(&words[1]);
    GetOsdConfigParam(&words[1]);
    SetOsdConfigParam(&words[0]);
    return ((words[1] >> 13) & 7u) == 0u;
}
}

// Observed msg_draw__15FrostStormSpell at 0x001e6b08, 8 bytes: `jr ra` and a nop. It does not read its argument, touch memory or
// call anything. The argument is the object (inferred from the name and the sibling draw handlers). Nothing here ran on the EE.
extern "C" void msg_draw__15FrostStormSpell(void*) {}

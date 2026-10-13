// Observed __ti5Spell at 0x00486dd8, 8 bytes, pinned SLUS_205.65: a zero-initialised small-data
// object between __ti5Skill (0x00486dd0) and __ti9type_info (0x00486de0). It is the run-time type
// descriptor node of class Spell: derived classes' __tf functions test its first word and, when
// zero, call __rtti_user(node, "5Spell") on it before passing it on as a base (see
// w10-actor-a-tf-15spellconeoffire). Both words stay zero until that call fills them.
// Unknown: which function first sets it up; no __tf5Spell body is in the imports.
extern "C" {
unsigned char __ti5Spell[8] __attribute__((aligned(4)));
}

// norrath_w13_spell_msg_a_view_8d6870: zeroed storage for an unresolved import of the w15-m-data-b package.
// Observed (source level):
// - The reference image has a .bss symbol at 0x008d6870; the callers declare norrath_w13_spell_msg_a_view_8d6870 as unsigned char[80] and pass its address to the particle-add call.
// - Callers in published units that name it:
//     w13-spell-msg-a-msg-run-11diseasebolt/
//     w14-projectile-b-msg-run-diseasebolt/
// Inferred:
// - 80 bytes and 16-byte alignment, from the particle-definition record contract (w8-spell-static-init-a-common/particle_def.hpp) and the 0x50 spacing of neighbouring views.
// Unknown:
// - Field contents. This is zeroed storage; the owner of the particle-definition initialisers has not supplied values.
// This unit provides the link name and extent only. It does not establish any field value.
extern "C" {
alignas(16) unsigned char norrath_w13_spell_msg_a_view_8d6870[80];
}
static_assert(sizeof norrath_w13_spell_msg_a_view_8d6870 == 80u, "norrath_w13_spell_msg_a_view_8d6870 extent");

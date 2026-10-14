// norrath_w13_spell_msg_a_view_7d6fc0: zeroed storage for an unresolved import. Closes the link name and extent only.
// Observed (source and reference-image level):
// - Reference image: an uninitialised (bss) symbol at address 0x007d6fc0 (reviewed EE nm; the table gives no size).
// - Callers declare norrath_w13_spell_msg_a_view_7d6fc0 as unsigned char[80] and pass its address to the particle-add call.
// - Callers in published units that name it: w13-spell-msg-a-msg-run-14hateprojectile/msg_run.cpp and w14-projectile-b-msg-run-hateprojectile/msg_run.cpp
// Inferred:
// - 80 bytes and 16-byte alignment, from the callers' declared extent and the particle-definition record contract
//   used by the sibling views in w15-m-data-b.
// Unknown:
// - Field contents. This is zeroed storage; the owner of the particle-definition initialisers has not supplied values.
// This unit does not establish any field value.
extern "C" {
alignas(16) unsigned char norrath_w13_spell_msg_a_view_7d6fc0[80];
}
static_assert(sizeof norrath_w13_spell_msg_a_view_7d6fc0 == 80u, "norrath_w13_spell_msg_a_view_7d6fc0 extent");

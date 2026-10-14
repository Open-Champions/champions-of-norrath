// skeletonResourceMap: zeroed storage for an unresolved import of the w15-m-data-b package.
// Observed (source level):
// - Reference image: symbol size 0x100 (256 bytes), initialised .data, at 0x0042d7d0 (symbol-table size read with the reviewed EE nm).
// - Callers in published units that name it:
//     w12-actor-spell-a-ctor-skeleton/ctor.cpp (passes skeletonResourceMap + (kind << 6) to the Creature constructor)
// Inferred:
// - 64-byte records: the caller forms each record address as the base plus (kind << 6).
// Unknown:
// - Field contents, and which entry each caller reads. The sibling w9 view norrath_ctors_lava_wraith_view_42d7d0 defines the same image bytes under another name; only one storage for that address may survive integration (see notes).
// This unit provides the link name and extent only. It does not establish any field value.
extern "C" {
alignas(16) unsigned char skeletonResourceMap[256];
}
static_assert(sizeof skeletonResourceMap == 256u, "skeletonResourceMap extent");

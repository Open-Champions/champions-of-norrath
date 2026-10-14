// skelightResourceMap: zeroed storage for an unresolved import of the w15-m-data-b package.
// Observed (source level):
// - Reference image: symbol size 0x50 (80 bytes), initialised .data, at 0x0042dbf0 (reviewed EE nm).
// - Callers in published units that name it:
//     w14-projectile-a-ctor-skelight/ctor.cpp (passes skelightResourceMap as the resource argument to the Creature constructor)
// Inferred:
// - Ten 8-byte slots, the layout the w9 sibling view observes for its 80-byte table at the same address.
// Unknown:
// - Field contents. The w9 sibling view norrath_ctors_lava_wraith_view_42dbf0 defines the same bytes under another name; see notes.
// This unit provides the link name and extent only. It does not establish any field value.
extern "C" {
alignas(16) unsigned char skelightResourceMap[80];
}
static_assert(sizeof skelightResourceMap == 80u, "skelightResourceMap extent");

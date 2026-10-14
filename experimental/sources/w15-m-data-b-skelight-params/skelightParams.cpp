// skelightParams: zeroed storage for an unresolved import of the w15-m-data-b package.
// Observed (source level):
// - Reference image: symbol size 0x20 (32 bytes), initialised .data, at 0x00438ed8 (reviewed EE nm).
// - Callers in published units that name it:
//     w13-skill-cast-b-cast-summon-skeleton/cast.cpp (takes skelightParams + 8 * index as a parameter block for index < 4)
// Inferred:
// - Four 8-byte records, from the caller's 8 * index stride with a guard of index < 4.
// Unknown:
// - Record contents and their meanings. Zeroed records give the callee zero parameters until the owner supplies the initial values.
// This unit provides the link name and extent only. It does not establish any field value.
extern "C" {
alignas(8) unsigned char skelightParams[32];
}
static_assert(sizeof skelightParams == 32u, "skelightParams extent");

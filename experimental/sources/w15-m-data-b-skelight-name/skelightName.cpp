// skelightName: zeroed storage for an unresolved import of the w15-m-data-b package.
// Observed (source level):
// - Reference image: symbol size 0x10 (16 bytes), initialised .data, at 0x00438ef8 (reviewed EE nm).
// - Callers in published units that name it:
//     w13-skill-cast-b-can-cast-summon-skeleton/can_cast.cpp (passes skelightName[index] to objectFindObject__FPc)
// Inferred:
// - Four pointer entries; 16 bytes means 4-byte pointers on this target, which the EE static_assert checks.
// Unknown:
// - The string each entry names, and the bound behind the index < 4 guard in the caller. Zeroed entries are null pointers, so the caller would pass null until the owner supplies real contents.
// This unit provides the link name and extent only. It does not establish any field value.
extern "C" {
char* skelightName[4];
}
static_assert(sizeof skelightName == 4u * sizeof(char*), "skelightName extent");

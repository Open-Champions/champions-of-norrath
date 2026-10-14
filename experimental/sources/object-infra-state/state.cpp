// Observed constructor and object-table storage.
// Extents are the bytes from each symbol to the next symbol. Initial bytes are
// the file-backed values; BSS objects are zero there. GP-relative users were
// resolved with the retained _gp value 0x4879f0.
//
// stringHash (0x5182c0, 2048) and objectHash (0x518ac0, 2048) are the tables
// both objectAddString and objectFindObject address. The constructor string
// views and the object hash-slot views are those two objects, so the second
// names are aliases rather than extra tables.

extern "C" {
// offTheGrid at 0x486890, four bytes, then userCreateByNameFunction.
unsigned char norrath_constructor_default_head_view[4];
// userCreateByNameFunction at 0x486894, four bytes.
unsigned char norrath_create_by_name_function_word[4];
// objectStrings at 0x486898, four bytes. objectAddString increments this word.
unsigned char norrath_constructor_string_count_view[4];
// freeNodes at 0x486a6c, four bytes. listOnewayLink pops this pointer.
unsigned char norrath_constructor_list_pool_view[4];
// syncRandState at 0x483e5c, four file-backed zero bytes.
unsigned char norrath_constructor_random_state_view[4];
// resourceFrame at 0x483f88. The file bytes are 01 00 00 00.
unsigned char norrath_constructor_resource_frame_view[4] = {1, 0, 0, 0};
// nmLumps at 0x483f8c, four file-backed zero bytes.
unsigned char norrath_constructor_lump_count_view[4];
// objectGrid at 0x4fc0c0. ghostObjectGrid follows 57600 bytes later.
unsigned char norrath_constructor_grid_a_view[120 * 120 * 4];
// ghostObjectGrid at 0x50a1c0. stringHash follows 57600 bytes later.
unsigned char norrath_constructor_grid_b_view[120 * 120 * 4];
// lumps at 0x6cb280. The next symbol is 5120 bytes later, 128 records of 40.
unsigned char norrath_constructor_lump_cache_view[128 * 40];
// stringHash, 512 words.
unsigned char norrath_constructor_string_names_view[512 * 4];
// objectHash, 512 words.
unsigned char norrath_constructor_string_objects_view[512 * 4];
// Bytes at 0x483ae0 through the first NUL. Following alignment bytes and the
// later literals are not part of this object.
unsigned char norrath_constructor_tag_a_view[] = {'h', 'i', 'd', 'e', 0};
// Bytes at 0x483ae8 through the first NUL.
unsigned char norrath_constructor_tag_b_view[] = {'n', 'a', 'm', 'e', 0};
// lightStart at 0x37f960, 36 file-backed zero bytes in .data.
unsigned char norrath_factory_marker_data_view[36];
}

// One object, two authored names. The reviewed EE assembler and the host
// assembler both accept .set; Mach-O adds the leading underscore.
#if defined(__APPLE__)
asm(
    ".globl _norrath_object_name_hash_slots_view\n"
    ".set _norrath_object_name_hash_slots_view, _norrath_constructor_string_names_view\n"
    ".globl _norrath_object_pointer_hash_slots_view\n"
    ".set _norrath_object_pointer_hash_slots_view, _norrath_constructor_string_objects_view\n"
);
#else
asm(
    ".globl norrath_object_name_hash_slots_view\n"
    ".set norrath_object_name_hash_slots_view, norrath_constructor_string_names_view\n"
    ".globl norrath_object_pointer_hash_slots_view\n"
    ".set norrath_object_pointer_hash_slots_view, norrath_constructor_string_objects_view\n"
);
#endif

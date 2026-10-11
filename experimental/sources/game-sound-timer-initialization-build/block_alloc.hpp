#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4, "target32 scalar representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target little endian byte view");
extern "C" {
extern unsigned char norrath_block_region_view[];
extern unsigned char norrath_list_copy_object_word[];
unsigned char* norrath_block_alloc(int requested_size);
void* norrath_block_memory_set(void* destination, int value, unsigned byte_count);
void norrath_list_set_copy_object(void* object);
}

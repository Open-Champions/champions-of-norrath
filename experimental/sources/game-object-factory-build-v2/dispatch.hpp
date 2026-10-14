#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(short) == 2, "target scalar representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4, "target32 address representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
struct NorrathPoint3 { float x, y, z; };
static_assert(sizeof(float) == 4 && sizeof(NorrathPoint3) == 12 && alignof(NorrathPoint3) == 4, "point word transport");
using NorrathCreateByName = unsigned char* (*)(char*, NorrathPoint3, short, char**);
extern "C" {
extern unsigned char norrath_create_by_name_function_word[];
void norrath_object_set_create_by_name_function(NorrathCreateByName function);
// The original target ABI transports Point3 through a target32 pointer to12 bytes.
unsigned char* norrath_object_create_by_name(char* name, const unsigned char* point, int type_word, char** arguments);
}

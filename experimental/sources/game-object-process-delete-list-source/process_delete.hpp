#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "Norrath word/address model");
extern "C" {
extern unsigned char norrath_restore_broadcast_head_view[];
extern unsigned char norrath_restore_broadcast_mask_word[];
void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask);
void norrath_list_safe_iterator_next(unsigned char* iterator);
void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags);
void norrath_object_process_delete_list();
}
namespace norrath_process_delete_detail {
using Address = unsigned;
inline Address address(const void* p) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
inline unsigned char* pointer(Address value) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
}
inline unsigned read_word(Address value) {
    const volatile unsigned char* p = pointer(value);
    const unsigned a = p[0], b = p[1], c = p[2], d = p[3];
    return a | (b << 8) | (c << 16) | (d << 24);
}
inline int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value)
                              : -1 - static_cast<int>(0xffffffffu - value);
}
inline int signed_adjustment(Address entry) {
    const volatile unsigned char* p = pointer(entry);
    const unsigned low = p[0], high = p[1];
    const unsigned value = low | (high << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
using Callback = void (*)(unsigned char*, int);
}

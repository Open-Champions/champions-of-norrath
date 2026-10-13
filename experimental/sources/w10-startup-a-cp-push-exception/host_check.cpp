// Host comparison only. Not an EE build input. The two callees are scripted; the record words
// and the chain head are checked at their 32-bit offsets.
#include <cstddef>
#include <cstdio>
#include <cstring>
#include "cp_push_exception.cpp"
alignas(16) static unsigned char block[64];
static unsigned head_word;
static unsigned alloc_size;
extern "C" void* __eh_alloc(unsigned size) { alloc_size = size; return block; }
extern "C" unsigned* __get_eh_info() { return &head_word; }
extern "C" void __cplus_type_matcher() {}
static void dtor() {}
int main() {
    int failures = 0;
    static_assert(sizeof(CpException) == 48 || sizeof(void*) == 8, "record size");
    static_assert(offsetof(CpException, previous) == 24 && offsetof(CpException, zero32) == 32 && offsetof(CpException, object_again) == 40, "layout");
    std::memset(block, 0xcc, sizeof block);
    head_word = 0x77665544u;
    int obj, typ;
    __cp_push_exception(&obj, &typ, dtor);
    const CpException* r = reinterpret_cast<const CpException*>(block);
    if (alloc_size != 48) { std::printf("allocation size\n"); ++failures; }
    if (r->matcher != address_word(reinterpret_cast<void*>(__cplus_type_matcher)) || r->first != 4 || r->second != 1) { std::printf("header words\n"); ++failures; }
    if (r->object != address_word(&obj) || r->object_again != address_word(&obj) || r->type != address_word(&typ) || r->destructor != address_word(reinterpret_cast<void*>(dtor))) { std::printf("object words\n"); ++failures; }
    if (r->zero20 != 0 || r->zero32 != 0) { std::printf("zero words\n"); ++failures; }
    if (r->previous != 0x77665544u || head_word != address_word(block)) { std::printf("chain\n"); ++failures; }
    if (static_cast<unsigned char>(block[28]) != 0xcc || static_cast<unsigned char>(block[44]) != 0xcc) { std::printf("unwritten words were touched\n"); ++failures; }
    __cp_push_exception(&obj, &typ, dtor);
    if (r->previous != address_word(block)) { std::printf("second push chains to itself only here\n"); ++failures; }
    if (failures) return 2;
    std::printf("cp_push_exception cases passed\n");
    return 0;
}

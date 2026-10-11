#pragma once
static_assert(sizeof(void*) == 4 && sizeof(unsigned) == 4 && sizeof(int) == 4, "EE word/pointer width");
extern "C" {
extern unsigned char norrath_heap_ring_word_view[];
extern unsigned char norrath_heap_fragment_threshold_view[];
extern unsigned char norrath_heap_next_fit_view[];
extern unsigned char norrath_heap_free_bytes_view[];
extern unsigned char norrath_heap_allocation_tag_view[];
extern unsigned char norrath_heap_total_bytes_view[];
extern unsigned char norrath_heap_dump_count_view[];
extern const unsigned char norrath_heap_check_file_view[];
extern const unsigned char norrath_heap_failure_format_view[];
extern const unsigned char norrath_heap_dump_border_view[];
extern const unsigned char norrath_heap_dump_count_format_view[];
extern const unsigned char norrath_heap_dump_free_format_view[];
extern const unsigned char norrath_heap_dump_used_format_view[];
extern const unsigned char norrath_heap_dump_totals_format_view[];
extern const unsigned char norrath_heap_dump_serial_format_view[];
// Same genuine absolute-address0 view as the loader correction. No ELF owner,
// payload or synthetic memory permission follows from this declaration.
extern volatile unsigned norrath_loader_failure_word_view;
void norrath_mem_check_heap_real(const char* file, int line);
void norrath_mem_dump_heap();
int norrath_sce_printf(const char* format, ...);
void* norrath_mem_alloc(int requested_bytes, char* allocation_name);
}
namespace norrath_heap_detail {
using Address = unsigned;
inline Address address(const void* p) { return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
inline const char* text(const unsigned char* p) { return reinterpret_cast<const char*>(p); }
inline int signed_word(unsigned v) { return v < 0x80000000u ? static_cast<int>(v) : -1 - static_cast<int>(~v); }
inline unsigned read_word(Address a) { const unsigned char* p=bytes(a); return unsigned(p[0]) | (unsigned(p[1])<<8) | (unsigned(p[2])<<16) | (unsigned(p[3])<<24); }
inline unsigned read_half(Address a) { return unsigned(bytes(a)[0]) | (unsigned(bytes(a)[1])<<8); }
inline void write_word(Address a, unsigned v) { unsigned char* p=bytes(a); p[0]=static_cast<unsigned char>(v);p[1]=static_cast<unsigned char>(v>>8);p[2]=static_cast<unsigned char>(v>>16);p[3]=static_cast<unsigned char>(v>>24); }
inline void write_half(Address a, unsigned v) { bytes(a)[0]=static_cast<unsigned char>(v);bytes(a)[1]=static_cast<unsigned char>(v>>8); }
}

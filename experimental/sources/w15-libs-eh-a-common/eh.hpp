// Shared layout for the w15-libs-eh-a exception-runtime sources. Nothing here is an EE input by itself.
// Observed from the unwinder routines at 0x002fa920..0x002fd0f0: the frame-state record they pass around is
// 1144 bytes. Words that hold addresses are 32-bit. Offsets (bytes):
//    0  word      read by get_reg_addr as the base for saved-at-offset registers, and rewritten by next_stack_level
//    4  word      not read by the routines of this package
//    8  doubleword  low half is added to the base word by next_stack_level (a signed offset)
//   16  doubleword  low half is the offset added before one extra load when the indirect byte is set
//   24  doubleword  written by the execute_cfa_insn record of a single unsigned value
//   32  123 doublewords, indexed by register number; the low half is a signed register number or offset
// 1016  halfword    register number that the base word is taken from
// 1018  halfword    return-address column (copied in from the common-information record by __frame_state_for)
// 1020  123 signed bytes, indexed by register number: 0 not saved, 1 saved at base + offset, 2 saved in
//                   another register
// 1143  signed byte the indirect flag
// Names are inferred from how the bytes are used; nothing in the image names these fields.
#ifndef W15_LIBS_EH_A_EH_HPP
#define W15_LIBS_EH_A_EH_HPP

namespace w15eh {

struct FrameState {
    unsigned base;
    unsigned word4;
    long long offset_from_base;
    long long indirect_offset;
    long long extra_value;
    long long reg_or_offset[123];
    unsigned short base_reg;
    unsigned short return_column;
    signed char saved[123];
    signed char indirect;
};
static_assert(sizeof(FrameState) == 1144, "frame-state record size");

enum { kNotSaved = 0, kSavedAtOffset = 1, kSavedInRegister = 2 };

}  // namespace w15eh

#if defined(W15_EH_HOST)
extern unsigned char* w15_eh_host_base;
#endif

namespace w15eh {
// Address word <-> pointer. On the target the word is the address; a host test keeps words as offsets from an
// arena base and routes every dereference through here.
inline unsigned char* from_word(unsigned word) {
#if defined(W15_EH_HOST)
    return w15_eh_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline unsigned to_word(const void* pointer) {
#if defined(W15_EH_HOST)
    return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - w15_eh_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}
}  // namespace w15eh


// Exception tables read by the handler-search routines (observed at 0x002fa920 and 0x002fa9f0).
#if defined(W15_EH_HOST)
unsigned w15_eh_host_call_matcher(unsigned function_word, unsigned char* info, unsigned match_info, unsigned char* table);
#endif
namespace w15eh {

// Old-style table: 12-byte entries {start, end, handler} ended by a start word of 0xffffffff.
struct OldHandlerEntry {
    unsigned start;
    unsigned end;
    unsigned handler;
};

// New-style table: an 8-byte header, then 16-byte entries {start, end, handler, match info} ended by a start word
// of 0xffffffff. The first word of the info record is a matcher function word.
struct HandlerEntry {
    unsigned start;
    unsigned end;
    unsigned handler;
    unsigned match_info;
};
struct HandlerTable {
    unsigned header[2];
    HandlerEntry entries[1];
};

inline unsigned call_matcher(unsigned function_word, unsigned char* info, unsigned match_info, HandlerTable* table) {
#if defined(W15_EH_HOST)
    return w15_eh_host_call_matcher(function_word, info, match_info, reinterpret_cast<unsigned char*>(table));
#else
    return reinterpret_cast<unsigned (*)(unsigned char*, unsigned, HandlerTable*)>(static_cast<__UINTPTR_TYPE__>(function_word))(info, match_info, table);
#endif
}

}  // namespace w15eh

#endif

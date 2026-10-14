// Observed execute_cfa_insn at 0x002fcca0, 1100 bytes (a local routine of the C++ exception runtime), pinned
// SLUS_205.65. It executes one call-frame instruction of the unwind tables. Arguments: the instruction cursor, the
// frame-state record (layout below, extended by one chain word at byte 1144), the five-word
// common-information record that extract_cie_info fills (code alignment at word 2, data alignment at word 3) and a
// pointer to the current code location. The advanced cursor is returned. The first byte decides:
//   * bit 0x40 set (this includes 0xc0): the location advances by (low six bits) * code alignment;
//   * else bit 0x80 set: an unsigned LEB128 operand times the data alignment (signed 32-bit product, sign-extended)
//     becomes the offset of register (low six bits), marked saved-at-offset, unless that register is the base register;
//   * else a test for 0xc0 that cannot succeed here, since neither 0x40 nor 0x80 is set: the arm that would clear
//     the saved marker of register (low six bits) is unreachable;
//   * else by opcode 0..47 (any other value calls abort and then returns the cursor):
//      0 7 8       nothing consumed beyond the opcode
//      1           location = the next four bytes (unaligned); cursor + 4
//      2 3 4       location += the next one, two (little-endian) or four bytes, unscaled
//      5           register, offset operands (both unsigned LEB128): as the 0x80 case
//      6           register operand: marked not saved
//      9           register, register2: marked saved in a register, value register2
//      10          remember state: allocate 1152 bytes, copy the record, chain word = the copy
//      11          restore state: copy the chained record back over the record and free it
//      12          register, offset: offset from base = offset (signed 32), base register = register
//      13          register: base register = register
//      14          offset: offset from base = offset (signed 32)
//      15          clears base register, both offsets and the indirect flag, then reads a length and runs
//                  decode_stack_op (0x002fcb80) on the following bytes until the cursor reaches the end
//      45          registers 16..31 become saved-at-offset with values 0, 4, ..., 60
//      46          operand stored at byte 24 (signed 32)
//      47          register, offset: marked saved-at-offset with the value -(offset * data alignment)
// Inferred: the DWARF opcode names implied by these actions (the numeric dispatch is what is observed). Unknown:
// decode_stack_op, __wrap_malloc and __wrap_free bodies; nothing was executed on the EE.
// Frame-state record layout (observed from this routine and the unwinder routines that share it; field names are
// inferred from use). Offsets in bytes:
//    0 word  base         4 word  (unused here)     8 doubleword  offset from base     16 doubleword  indirect offset
//   24 doubleword  extra value     32 123 doublewords indexed by register: a register number or an offset
// 1016 halfword  base register    1018 halfword  return-address column
// 1020 123 signed bytes indexed by register: 0 not saved, 1 saved at offset, 2 saved in a register
// 1143 signed byte  indirect flag; 1144 word  chain to the copy made by the remember-state instruction
// (the record is 1144 bytes to its callers and 1152 bytes as this routine allocates and copies it).
#if defined(W16_HOST_CHECK)
extern unsigned char* w16_host_arena;
#endif
namespace w16cfa {
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
    unsigned saved_state;
};
static_assert(sizeof(FrameState) == 1152, "extended frame-state record size");
enum { kNotSaved = 0, kSavedAtOffset = 1, kSavedInRegister = 2 };

// Address word <-> pointer. A host check keeps words as offsets from an arena base.
#if defined(W16_HOST_CHECK)
inline unsigned char* from_word(unsigned word) { return w16_host_arena + word; }
inline unsigned to_word(const void* pointer) { return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - w16_host_arena); }
#else
inline unsigned char* from_word(unsigned word) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word)); }
inline unsigned to_word(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
#endif
}  // namespace w16cfa

extern "C" {
struct CieInfo {
    const char* text;
    unsigned eh_pointer;
    unsigned code_align;
    int data_align;
    unsigned return_column;
};

const unsigned char* decode_uleb128(const unsigned char* cursor, unsigned* value);
const unsigned char* decode_stack_op(const unsigned char* cursor, w16cfa::FrameState* state);
void* __wrap_malloc(unsigned size);
void __wrap_free(void* pointer);
void abort();

static void copy_record(w16cfa::FrameState* to, const w16cfa::FrameState* from) {
    long long* d = reinterpret_cast<long long*>(to);
    const long long* s = reinterpret_cast<const long long*>(from);
    for (int i = 0; i < 144; ++i) {
        d[i] = s[i];
        __asm__ volatile("" ::: "memory");
    }
}

static void save_at_offset(w16cfa::FrameState* state, unsigned reg, long long value) {
    state->saved[reg] = w16cfa::kSavedAtOffset;
    state->reg_or_offset[reg] = value;
}

struct UnalignedWord {
    unsigned value;
} __attribute__((packed));

const unsigned char* execute_cfa_insn(const unsigned char* p, w16cfa::FrameState* state, CieInfo* info,
                                      unsigned* pc) {
    const unsigned insn = *p++;
    unsigned reg, reg2, operand;
    if (insn & 0x40) {
        *pc += (insn & 0x3f) * info->code_align;
    } else if (insn & 0x80) {
        reg = insn & 0x3f;
        p = decode_uleb128(p, &operand);
        if (reg != state->base_reg) save_at_offset(state, reg, static_cast<int>(operand) * info->data_align);
    } else if (insn & 0xc0) {
        state->saved[insn & 0x3f] = w16cfa::kNotSaved;
    } else if (insn < 48) {
        switch (insn) {
        case 0: case 7: case 8:
            break;
        case 1:
            *pc = reinterpret_cast<const UnalignedWord*>(p)->value;
            p += 4;
            break;
        case 2:
            *pc += *p++;
            break;
        case 3:
            *pc += static_cast<unsigned short>((p[1] << 8) | p[0]);
            p += 2;
            break;
        case 4:
            *pc += reinterpret_cast<const UnalignedWord*>(p)->value;
            p += 4;
            break;
        case 5:
            p = decode_uleb128(p, &reg);
            p = decode_uleb128(p, &operand);
            if (reg != state->base_reg) save_at_offset(state, reg, static_cast<int>(operand) * info->data_align);
            break;
        case 6:
            p = decode_uleb128(p, &reg);
            state->saved[reg] = w16cfa::kNotSaved;
            break;
        case 9:
            p = decode_uleb128(p, &reg);
            p = decode_uleb128(p, &reg2);
            state->saved[reg] = w16cfa::kSavedInRegister;
            state->reg_or_offset[reg] = reg2;
            break;
        case 10: {
            w16cfa::FrameState* copy = static_cast<w16cfa::FrameState*>(__wrap_malloc(1152));
            copy_record(copy, state);
            state->saved_state = w16cfa::to_word(copy);
            break;
        }
        case 11: {
            w16cfa::FrameState* saved = reinterpret_cast<w16cfa::FrameState*>(w16cfa::from_word(state->saved_state));
            copy_record(state, saved);
            __wrap_free(saved);
            break;
        }
        case 12:
            p = decode_uleb128(p, &reg);
            p = decode_uleb128(p, &operand);
            state->offset_from_base = static_cast<int>(operand);
            state->base_reg = static_cast<unsigned short>(reg);
            break;
        case 13:
            p = decode_uleb128(p, &reg);
            state->base_reg = static_cast<unsigned short>(reg);
            break;
        case 14:
            p = decode_uleb128(p, &operand);
            state->offset_from_base = static_cast<int>(operand);
            break;
        case 15: {
            state->base_reg = 0;
            state->offset_from_base = 0;
            state->indirect_offset = 0;
            state->indirect = 0;
            p = decode_uleb128(p, &operand);
            const unsigned char* end = p + operand;
            while (p < end) p = decode_stack_op(p, state);
            break;
        }
        case 45:
            for (unsigned i = 16; i < 32; ++i) {
                state->saved[i] = w16cfa::kSavedAtOffset;
                state->reg_or_offset[i] = (i - 16) * 4;
            }
            break;
        case 46:
            p = decode_uleb128(p, &operand);
            state->extra_value = static_cast<int>(operand);
            break;
        case 47:
            p = decode_uleb128(p, &reg);
            p = decode_uleb128(p, &operand);
            state->saved[reg] = w16cfa::kSavedAtOffset;
            state->reg_or_offset[reg] = -(static_cast<int>(operand) * info->data_align);
            break;
        default:
            abort();
        }
    } else {
        abort();
    }
    return p;
}
}

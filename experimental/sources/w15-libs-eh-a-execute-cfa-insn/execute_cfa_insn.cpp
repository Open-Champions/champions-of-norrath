// Observed execute_cfa_insn at 0x002fcca0, 1100 bytes (a local routine of the C++ exception runtime), pinned
// SLUS_205.65. It executes one call-frame instruction of the unwind tables. Arguments: the instruction cursor, the
// frame-state record (../w15-libs-eh-a-common/eh.hpp, extended by one chain word at byte 1144), the five-word
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
#include "../w15-libs-eh-a-common/eh.hpp"

extern "C" {
struct w15_cie_info {
    const char* augmentation;
    unsigned eh_pointer;
    unsigned code_align;
    int data_align;
    unsigned return_column;
};

const unsigned char* decode_uleb128(const unsigned char* cursor, unsigned* value);
const unsigned char* decode_stack_op(const unsigned char* cursor, w15eh::FrameStateInternal* state);
void* __wrap_malloc(unsigned size);
void __wrap_free(void* pointer);
void abort();

static void copy_record(w15eh::FrameStateInternal* to, const w15eh::FrameStateInternal* from) {
    long long* d = reinterpret_cast<long long*>(to);
    const long long* s = reinterpret_cast<const long long*>(from);
    for (int i = 0; i < 144; ++i) {
        d[i] = s[i];
        __asm__ volatile("" ::: "memory");
    }
}

static void save_at_offset(w15eh::FrameStateInternal* state, unsigned reg, long long value) {
    state->saved[reg] = w15eh::kSavedAtOffset;
    state->reg_or_offset[reg] = value;
}

struct w15_unaligned_word {
    unsigned value;
} __attribute__((packed));

const unsigned char* execute_cfa_insn(const unsigned char* p, w15eh::FrameStateInternal* state, w15_cie_info* info,
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
        state->saved[insn & 0x3f] = w15eh::kNotSaved;
    } else if (insn < 48) {
        switch (insn) {
        case 0: case 7: case 8:
            break;
        case 1:
            *pc = reinterpret_cast<const w15_unaligned_word*>(p)->value;
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
            *pc += reinterpret_cast<const w15_unaligned_word*>(p)->value;
            p += 4;
            break;
        case 5:
            p = decode_uleb128(p, &reg);
            p = decode_uleb128(p, &operand);
            if (reg != state->base_reg) save_at_offset(state, reg, static_cast<int>(operand) * info->data_align);
            break;
        case 6:
            p = decode_uleb128(p, &reg);
            state->saved[reg] = w15eh::kNotSaved;
            break;
        case 9:
            p = decode_uleb128(p, &reg);
            p = decode_uleb128(p, &reg2);
            state->saved[reg] = w15eh::kSavedInRegister;
            state->reg_or_offset[reg] = reg2;
            break;
        case 10: {
            w15eh::FrameStateInternal* copy = static_cast<w15eh::FrameStateInternal*>(__wrap_malloc(1152));
            copy_record(copy, state);
            state->saved_state = w15eh::to_word(copy);
            break;
        }
        case 11: {
            w15eh::FrameStateInternal* saved = reinterpret_cast<w15eh::FrameStateInternal*>(w15eh::from_word(state->saved_state));
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
                state->saved[i] = w15eh::kSavedAtOffset;
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
            state->saved[reg] = w15eh::kSavedAtOffset;
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

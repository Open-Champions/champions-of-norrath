// Cell executor for the observed amx_Exec entry and 131 dispatch slots.
// Handler-table entries are the slot numbers 0..130, which is what the null
// AMX query publishes. This does not reproduce original code addresses,
// physical frame saves, HI/LO banks, or interrupts.
extern "C" {
void* norrath_copy_bytes(void* destination, const void* source, __SIZE_TYPE__ count);
int norrath_memory_compare(const void* left, const void* right, __SIZE_TYPE__ count);
extern unsigned char norrath_amx_little_endian_word_view[4];
#if defined(NORRATH_HOST_CONTRACT)
extern unsigned* norrath_amx_exec_host_ids;
extern unsigned char* norrath_amx_exec_host_base;
extern unsigned long long (*norrath_amx_exec_host_callback)(void*, int, int*, void*);
#endif
}

namespace {
unsigned read32(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write32(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int signed32(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value)
                                : -1 - static_cast<int>(0xffffffffu - value);
}
unsigned add32(unsigned left, unsigned right) { return left + right; }
unsigned sub32(unsigned left, unsigned right) { return left - right; }

unsigned char* pointer_from(unsigned value) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_amx_exec_host_base + value;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}
unsigned address_of(const unsigned char* pointer) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned>(pointer - norrath_amx_exec_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}

unsigned handler_ids[131];
bool handler_ids_ready = false;
void ensure_handler_ids() {
    if (handler_ids_ready) return;
    for (unsigned index = 0; index < 131u; ++index) handler_ids[index] = index;
    handler_ids_ready = true;
}

void signed_divide(unsigned dividend, unsigned divisor, unsigned& quotient, unsigned& remainder) {
    const int left = signed32(dividend);
    const int right = signed32(divisor);
    // The original HI/LO result of this pair was not recovered. This keeps the
    // C++ evaluation defined; it is not an observed hardware result.
    if (left == static_cast<int>(0x80000000u) && right == -1) {
        quotient = 0x80000000u;
        remainder = 0;
        return;
    }
    quotient = static_cast<unsigned>(left / right);
    remainder = static_cast<unsigned>(left % right);
}

struct Machine {
    unsigned char* amx;
    unsigned char* image;
    unsigned char* code;
    unsigned char* data;
    unsigned char* cursor;
    unsigned pri;
    unsigned alt;
    unsigned hea;
    unsigned stk;
    unsigned frm;
    unsigned entry_stk;
    int* result;
    unsigned status;
    bool stop;

    unsigned cell() {
        const unsigned value = read32(cursor);
        cursor += 4;
        return value;
    }
    void restore_stack() { write32(amx + 24, entry_stk); }
    bool rejected(unsigned offset) {
        if (signed32(offset) < signed32(hea)) return false;
        if (signed32(offset) < signed32(stk) || signed32(offset) >= signed32(read32(amx + 28))) {
            restore_stack();
            status = 5;
            stop = true;
            return true;
        }
        return false;
    }
    unsigned endian() const { return read32(norrath_amx_little_endian_word_view); }
    void align_xor(unsigned& value, unsigned width) {
        if (endian() != 0 && width < 4u) value ^= (4u - width);
    }
    void finish(unsigned code_status) {
        status = code_status;
        stop = true;
    }
};

void execute(Machine& machine) {
    while (!machine.stop) {
        const unsigned operation = machine.cell();
        if (operation > 130u) {
            machine.restore_stack();
            machine.finish(6);
            break;
        }
        switch (operation) {
        case 0:
            machine.restore_stack();
            machine.finish(6);
            break;
        case 1: machine.pri = read32(machine.data + machine.cell()); break;
        case 2: machine.alt = read32(machine.data + machine.cell()); break;
        case 3: machine.pri = read32(machine.data + machine.frm + machine.cell()); break;
        case 4: machine.alt = read32(machine.data + machine.frm + machine.cell()); break;
        case 5: {
            const unsigned slot = read32(machine.data + machine.cell());
            machine.pri = read32(machine.data + slot);
            break;
        }
        case 6: {
            const unsigned slot = read32(machine.data + machine.cell());
            machine.alt = read32(machine.data + slot);
            break;
        }
        case 7: {
            const unsigned slot = read32(machine.data + machine.frm + machine.cell());
            machine.pri = read32(machine.data + slot);
            break;
        }
        case 8: {
            const unsigned slot = read32(machine.data + machine.frm + machine.cell());
            machine.alt = read32(machine.data + slot);
            break;
        }
        case 9:
            if (!machine.rejected(machine.pri)) machine.pri = read32(machine.data + machine.pri);
            break;
        case 10: {
            const unsigned width = machine.cell();
            if (machine.rejected(machine.pri)) break;
            unsigned char* address = machine.data + machine.pri;
            if (width == 1u) machine.pri = address[0];
            else if (width == 2u) machine.pri = unsigned(address[0]) | (unsigned(address[1]) << 8);
            else if (width == 4u) machine.pri = read32(address);
            break;
        }
        case 11: machine.pri = machine.cell(); break;
        case 12: machine.alt = machine.cell(); break;
        case 13: machine.pri = add32(machine.frm, machine.cell()); break;
        case 14: machine.alt = add32(machine.frm, machine.cell()); break;
        case 15: write32(machine.data + machine.cell(), machine.pri); break;
        case 16: write32(machine.data + machine.cell(), machine.alt); break;
        case 17: write32(machine.data + machine.frm + machine.cell(), machine.pri); break;
        case 18: write32(machine.data + machine.frm + machine.cell(), machine.alt); break;
        case 19: {
            const unsigned slot = read32(machine.data + machine.cell());
            write32(machine.data + slot, machine.pri);
            break;
        }
        case 20: {
            const unsigned slot = read32(machine.data + machine.cell());
            write32(machine.data + slot, machine.alt);
            break;
        }
        case 21: {
            const unsigned slot = read32(machine.data + machine.frm + machine.cell());
            write32(machine.data + slot, machine.pri);
            break;
        }
        case 22: {
            const unsigned slot = read32(machine.data + machine.frm + machine.cell());
            write32(machine.data + slot, machine.alt);
            break;
        }
        case 23:
            if (!machine.rejected(machine.alt)) write32(machine.data + machine.alt, machine.pri);
            break;
        case 24: {
            const unsigned width = machine.cell();
            if (machine.rejected(machine.alt)) break;
            unsigned char* address = machine.data + machine.alt;
            if (width == 1u) address[0] = static_cast<unsigned char>(machine.pri);
            else if (width == 2u) {
                address[0] = static_cast<unsigned char>(machine.pri);
                address[1] = static_cast<unsigned char>(machine.pri >> 8);
            } else if (width == 4u) write32(address, machine.pri);
            break;
        }
        case 25: {
            const unsigned offset = add32(machine.alt, machine.pri << 2);
            if (!machine.rejected(offset)) machine.pri = read32(machine.data + offset);
            break;
        }
        case 26: {
            const unsigned shift = machine.cell() & 31u;
            const unsigned offset = add32(machine.alt, machine.pri << shift);
            if (!machine.rejected(offset)) machine.pri = read32(machine.data + offset);
            break;
        }
        case 27: machine.pri = add32(machine.alt, machine.pri << 2); break;
        case 28: {
            const unsigned shift = machine.cell() & 31u;
            machine.pri = add32(machine.alt, machine.pri << shift);
            break;
        }
        case 29: machine.align_xor(machine.pri, machine.cell()); break;
        case 30: machine.align_xor(machine.alt, machine.cell()); break;
        case 31: {
            const unsigned selector = machine.cell();
            if (selector >= 7u) break;
            if (selector == 0u) machine.pri = read32(machine.image + 12);
            else if (selector == 1u) machine.pri = read32(machine.image + 16);
            else if (selector == 2u) machine.pri = machine.hea;
            else if (selector == 3u) machine.pri = read32(machine.amx + 28);
            else if (selector == 4u) machine.pri = machine.stk;
            else if (selector == 5u) machine.pri = machine.frm;
            else machine.pri = static_cast<unsigned>(machine.cursor - machine.code);
            break;
        }
        case 32: {
            const unsigned selector = machine.cell();
            if (selector == 2u) machine.hea = machine.pri;
            else if (selector == 4u) machine.stk = machine.pri;
            else if (selector == 5u) machine.frm = machine.pri;
            else if (selector == 6u) machine.cursor = machine.code + machine.pri;
            break;
        }
        case 33: machine.pri = machine.alt; break;
        case 34: machine.alt = machine.pri; break;
        case 35: {
            const unsigned saved = machine.pri;
            machine.pri = machine.alt;
            machine.alt = saved;
            break;
        }
        case 36:
            machine.stk = sub32(machine.stk, 4u);
            write32(machine.data + machine.stk, machine.pri);
            break;
        case 37:
            machine.stk = sub32(machine.stk, 4u);
            write32(machine.data + machine.stk, machine.alt);
            break;
        case 38: {
            unsigned count = machine.cell();
            while (signed32(count) != -1) {
                machine.stk = sub32(machine.stk, 4u);
                write32(machine.data + machine.stk, machine.pri);
                count = sub32(count, 1u);
            }
            break;
        }
        case 39: {
            const unsigned value = machine.cell();
            machine.stk = sub32(machine.stk, 4u);
            write32(machine.data + machine.stk, value);
            break;
        }
        case 40: {
            const unsigned value = read32(machine.data + machine.cell());
            machine.stk = sub32(machine.stk, 4u);
            write32(machine.data + machine.stk, value);
            break;
        }
        case 41: {
            const unsigned value = read32(machine.data + machine.frm + machine.cell());
            machine.stk = sub32(machine.stk, 4u);
            write32(machine.data + machine.stk, value);
            break;
        }
        case 42:
            machine.pri = read32(machine.data + machine.stk);
            machine.stk = add32(machine.stk, 4u);
            break;
        case 43:
            machine.alt = read32(machine.data + machine.stk);
            machine.stk = add32(machine.stk, 4u);
            break;
        case 44: {
            const unsigned adjustment = machine.cell();
            machine.alt = machine.stk;
            machine.stk = add32(machine.stk, adjustment);
            if (signed32(machine.stk) < signed32(add32(machine.hea, 64u))) machine.finish(3);
            else if (signed32(read32(machine.image + 24)) < signed32(machine.stk)) machine.finish(7);
            break;
        }
        case 45: {
            const unsigned adjustment = machine.cell();
            machine.alt = machine.hea;
            machine.hea = add32(machine.hea, adjustment);
            if (signed32(machine.stk) < signed32(add32(machine.hea, 64u))) machine.finish(3);
            else if (signed32(machine.hea) < 0) machine.finish(8);
            break;
        }
        case 46:
            machine.stk = sub32(machine.stk, 4u);
            write32(machine.data + machine.stk, machine.frm);
            machine.frm = machine.stk;
            break;
        case 47: {
            const unsigned saved_frame = read32(machine.data + machine.stk);
            machine.stk = add32(machine.stk, 4u);
            const unsigned return_offset = read32(machine.data + machine.stk);
            machine.stk = add32(machine.stk, 4u);
            machine.frm = saved_frame;
            if (return_offset == 0u) {
                if (machine.result != nullptr) *machine.result = signed32(machine.pri);
                machine.restore_stack();
                machine.finish(0);
            } else machine.cursor = machine.code + return_offset;
            break;
        }
        case 48: {
            const unsigned saved_frame = read32(machine.data + machine.stk);
            machine.stk = add32(machine.stk, 4u);
            const unsigned return_offset = read32(machine.data + machine.stk);
            machine.stk = add32(machine.stk, 4u);
            const unsigned argument_bytes = read32(machine.data + machine.stk);
            machine.stk = add32(add32(machine.stk, argument_bytes), 4u);
            machine.frm = saved_frame;
            write32(machine.amx + 24, machine.stk);
            if (return_offset == 0u) {
                if (machine.result != nullptr) *machine.result = signed32(machine.pri);
                machine.finish(0);
            } else machine.cursor = machine.code + return_offset;
            break;
        }
        case 49: {
            const unsigned return_offset = static_cast<unsigned>(machine.cursor + 4 - machine.code);
            const unsigned target = machine.cell();
            machine.stk = sub32(machine.stk, 4u);
            write32(machine.data + machine.stk, return_offset);
            machine.cursor = pointer_from(target);
            break;
        }
        case 50: {
            const unsigned return_offset = static_cast<unsigned>(machine.cursor - machine.code);
            machine.stk = sub32(machine.stk, 4u);
            write32(machine.data + machine.stk, return_offset);
            machine.cursor = machine.code + machine.pri;
            break;
        }
        case 51: machine.cursor = pointer_from(machine.cell()); break;
        case 52: {
            unsigned char* operand = machine.cursor;
            const unsigned immediate = machine.cell();
            machine.cursor = operand + signed32(immediate) + 4;
            break;
        }
        case 53: case 54: case 55: case 56: case 57: case 58: case 59: case 60:
        case 61: case 62: case 63: case 64: {
            const unsigned target = machine.cell();
            bool taken = false;
            const int pri = signed32(machine.pri);
            const int alt = signed32(machine.alt);
            if (operation == 53u) taken = machine.pri == 0u;
            else if (operation == 54u) taken = machine.pri != 0u;
            else if (operation == 55u) taken = machine.pri == machine.alt;
            else if (operation == 56u) taken = machine.pri != machine.alt;
            else if (operation == 57u) taken = machine.pri < machine.alt;
            else if (operation == 58u) taken = machine.pri <= machine.alt;
            else if (operation == 59u) taken = machine.pri > machine.alt;
            else if (operation == 60u) taken = machine.pri >= machine.alt;
            else if (operation == 61u) taken = pri < alt;
            else if (operation == 62u) taken = pri <= alt;
            else if (operation == 63u) taken = pri > alt;
            else taken = pri >= alt;
            if (taken) machine.cursor = pointer_from(target);
            break;
        }
        case 65: machine.pri = machine.pri << (machine.alt & 31u); break;
        case 66: machine.pri = machine.pri >> (machine.alt & 31u); break;
        case 67: machine.pri = static_cast<unsigned>(signed32(machine.pri) >> (machine.alt & 31u)); break;
        case 68: machine.pri = machine.pri << (machine.cell() & 31u); break;
        case 69: machine.alt = machine.alt << (machine.cell() & 31u); break;
        case 70: machine.pri = machine.pri >> (machine.cell() & 31u); break;
        case 71: machine.alt = machine.alt >> (machine.cell() & 31u); break;
        case 72: case 75: case 88: {
            const unsigned factor = operation == 88u ? machine.cell() : machine.alt;
            const long long product = static_cast<long long>(signed32(machine.pri)) * static_cast<long long>(signed32(factor));
            machine.pri = static_cast<unsigned>(product);
            break;
        }
        case 73: case 74: {
            const unsigned dividend = operation == 73u ? machine.pri : machine.alt;
            const unsigned divisor = operation == 73u ? machine.alt : machine.pri;
            if (divisor == 0u) {
                machine.restore_stack();
                machine.finish(11);
                break;
            }
            unsigned quotient = 0;
            unsigned remainder = 0;
            unsigned adjusted = 0;
            signed_divide(dividend, divisor, quotient, remainder);
            signed_divide(add32(remainder, divisor), divisor, quotient, adjusted);
            signed_divide(sub32(dividend, adjusted), divisor, quotient, remainder);
            machine.pri = quotient;
            machine.alt = adjusted;
            break;
        }
        case 76: case 77: {
            const unsigned dividend = operation == 76u ? machine.pri : machine.alt;
            const unsigned divisor = operation == 76u ? machine.alt : machine.pri;
            if (divisor == 0u) {
                machine.restore_stack();
                machine.finish(11);
                break;
            }
            machine.pri = dividend / divisor;
            machine.alt = dividend % divisor;
            break;
        }
        case 78: machine.pri = add32(machine.pri, machine.alt); break;
        case 79: machine.pri = sub32(machine.pri, machine.alt); break;
        case 80: machine.pri = sub32(machine.alt, machine.pri); break;
        case 81: machine.pri &= machine.alt; break;
        case 82: machine.pri |= machine.alt; break;
        case 83: machine.pri ^= machine.alt; break;
        case 84: machine.pri = machine.pri < 1u ? 1u : 0u; break;
        case 85: machine.pri = sub32(0u, machine.pri); break;
        case 86: machine.pri = ~machine.pri; break;
        case 87: machine.pri = add32(machine.pri, machine.cell()); break;
        case 89: machine.pri = 0; break;
        case 90: machine.alt = 0; break;
        case 91: write32(machine.data + machine.cell(), 0); break;
        case 92: write32(machine.data + machine.frm + machine.cell(), 0); break;
        case 93:
            if ((machine.pri & 0xffu) >= 128u) machine.pri |= 0xffffff00u;
            break;
        case 94:
            if ((machine.alt & 0xffu) >= 128u) machine.alt |= 0xffffff00u;
            break;
        case 95: machine.pri = machine.pri == machine.alt ? 1u : 0u; break;
        case 96: machine.pri = machine.pri != machine.alt ? 1u : 0u; break;
        case 97: machine.pri = machine.pri < machine.alt ? 1u : 0u; break;
        case 98: machine.pri = machine.pri <= machine.alt ? 1u : 0u; break;
        case 99: machine.pri = machine.pri > machine.alt ? 1u : 0u; break;
        case 100: machine.pri = machine.pri >= machine.alt ? 1u : 0u; break;
        case 101: machine.pri = signed32(machine.pri) < signed32(machine.alt) ? 1u : 0u; break;
        case 102: machine.pri = signed32(machine.pri) <= signed32(machine.alt) ? 1u : 0u; break;
        case 103: machine.pri = signed32(machine.pri) > signed32(machine.alt) ? 1u : 0u; break;
        case 104: machine.pri = signed32(machine.pri) >= signed32(machine.alt) ? 1u : 0u; break;
        case 105: machine.pri = machine.pri == machine.cell() ? 1u : 0u; break;
        case 106: machine.pri = machine.alt == machine.cell() ? 1u : 0u; break;
        case 107: machine.pri = add32(machine.pri, 1u); break;
        case 108: machine.alt = add32(machine.alt, 1u); break;
        case 109: {
            unsigned char* address = machine.data + machine.cell();
            write32(address, add32(read32(address), 1u));
            break;
        }
        case 110: {
            unsigned char* address = machine.data + machine.frm + machine.cell();
            write32(address, add32(read32(address), 1u));
            break;
        }
        case 111: write32(machine.data + machine.pri, add32(read32(machine.data + machine.pri), 1u)); break;
        case 112: machine.pri = sub32(machine.pri, 1u); break;
        case 113: machine.alt = sub32(machine.alt, 1u); break;
        case 114: {
            unsigned char* address = machine.data + machine.cell();
            write32(address, sub32(read32(address), 1u));
            break;
        }
        case 115: {
            unsigned char* address = machine.data + machine.frm + machine.cell();
            write32(address, sub32(read32(address), 1u));
            break;
        }
        case 116: write32(machine.data + machine.pri, sub32(read32(machine.data + machine.pri), 1u)); break;
        case 117: {
            const unsigned count = machine.cell();
            norrath_copy_bytes(machine.data + machine.alt, machine.data + machine.pri, count);
            break;
        }
        case 118: {
            const unsigned count = machine.cell();
            machine.pri = static_cast<unsigned>(norrath_memory_compare(
                machine.data + machine.alt, machine.data + machine.pri, count));
            break;
        }
        case 119: {
            unsigned count = machine.cell();
            unsigned offset = machine.alt;
            while (count >= 4u) {
                write32(machine.data + offset, machine.pri);
                offset = add32(offset, 4u);
                count = sub32(count, 4u);
            }
            break;
        }
        case 120: {
            const unsigned code_status = machine.cell();
            if (machine.result != nullptr) *machine.result = signed32(machine.pri);
            machine.restore_stack();
            machine.finish(code_status);
            break;
        }
        case 121: {
            const unsigned limit = machine.cell();
            if (signed32(machine.pri) < 0 || signed32(limit) < signed32(machine.pri)) {
                machine.restore_stack();
                machine.finish(4);
            }
            break;
        }
        case 122: case 123: {
            const unsigned index = operation == 122u ? machine.pri : machine.cell();
            write32(machine.amx + 12, static_cast<unsigned>(machine.cursor - machine.code));
            write32(machine.amx + 16, machine.frm);
            write32(machine.amx + 20, machine.hea);
            write32(machine.amx + 24, machine.stk);
            int cell = signed32(machine.pri);
#if defined(NORRATH_HOST_CONTRACT)
            const unsigned long long returned = norrath_amx_exec_host_callback(
                machine.amx, static_cast<int>(index), &cell, machine.data + machine.stk);
#else
            using Callback = unsigned long long (*)(void*, int, int*, void*);
            const auto callback = reinterpret_cast<Callback>(
                static_cast<__UINTPTR_TYPE__>(read32(machine.amx + 4)));
            const unsigned long long returned = callback(
                machine.amx, static_cast<int>(index), &cell, machine.data + machine.stk);
#endif
            machine.pri = static_cast<unsigned>(cell);
            if (returned != 0ull) {
                machine.restore_stack();
                machine.finish(static_cast<unsigned>(returned));
            }
            break;
        }
        case 124: machine.cursor += machine.cell(); break;
        case 125:
            write32(machine.amx + 36, machine.cell());
            write32(machine.amx + 40, machine.cell());
            break;
        case 126: {
            const unsigned length = machine.cell();
            const unsigned second = machine.cell();
            const unsigned third = machine.cell();
            write32(machine.amx + 48, second);
            write32(machine.amx + 56, address_of(machine.cursor));
            write32(machine.amx + 52, third);
            machine.cursor = machine.cursor + length - 8;
            break;
        }
        case 127:
            write32(machine.amx + 48, machine.cell());
            write32(machine.amx + 52, machine.cell());
            break;
        case 128: machine.cursor = machine.code + machine.pri; break;
        case 129: {
            const unsigned char* table = pointer_from(read32(machine.cursor));
            const unsigned count = read32(table + 4);
            machine.cursor = pointer_from(read32(table + 8));
            if (signed32(count) <= 0) break;
            const unsigned char* pair = table + 12;
            unsigned remaining = count;
            for (;;) {
                if (read32(pair) == machine.pri && signed32(remaining) > 0) {
                    machine.cursor = pointer_from(read32(pair + 4));
                    break;
                }
                remaining = sub32(remaining, 1u);
                if (signed32(remaining) <= 0) break;
                pair += 8;
            }
            break;
        }
        case 130:
            break;
        }
    }
}
}

extern "C" unsigned long long norrath_amx_exec(void* amx, int* result,
                                                long long public_index,
                                                long long parameter_count,
                                                const void* parameter_data) {
    ensure_handler_ids();
#if defined(NORRATH_HOST_CONTRACT)
    norrath_amx_exec_host_ids = handler_ids;
#endif
    if (amx == nullptr) {
        *result = static_cast<int>(static_cast<unsigned>(
            reinterpret_cast<__UINTPTR_TYPE__>(handler_ids)));
        return 0ull;
    }
    auto* state = static_cast<unsigned char*>(amx);
    if (read32(state + 4) == 0u) return 9ull;
    const unsigned flags = read32(state + 32);
    write32(state + 32, flags & ~0x4000u);
    if ((flags & 0x8000u) == 0u) return 22ull;

    unsigned char* image = pointer_from(read32(state));
    const unsigned entry_stk = read32(state + 24);
    const unsigned hea = read32(state + 20);
    if (signed32(read32(image + 24)) < signed32(entry_stk)) return 7ull;
    if (signed32(hea) < 0) return 8ull;

    unsigned char* code = image + read32(image + 12);
    unsigned char* data = image + read32(image + 16);
    unsigned char* cursor = nullptr;
    if (public_index == -1) {
        const unsigned cip = read32(image + 28);
        if (signed32(cip) < 0) return 20ull;
        cursor = code + cip;
    } else if (public_index < 0 && public_index != -2) {
        return 20ull;
    } else {
        const unsigned half = unsigned(image[32]) | (unsigned(image[33]) << 8);
        const int count = half < 0x8000u ? static_cast<int>(half) : static_cast<int>(half) - 65536;
        if (!(public_index < count)) return 20ull;
        const unsigned stride = static_cast<unsigned>(public_index) * 24u;
        const unsigned entry = read32(image + read32(image + 36) + stride);
        cursor = code + entry;
    }

    const unsigned byte_count = static_cast<unsigned>(parameter_count) << 2;
    unsigned stack = sub32(entry_stk, byte_count);
    if (parameter_count > 0) {
        const auto* source = static_cast<const unsigned char*>(parameter_data);
        unsigned char* destination = data + stack;
        for (long long remaining = parameter_count; remaining > 0; --remaining) {
            write32(destination, read32(source));
            source += 8;
            destination += 4;
        }
    }
    stack = sub32(stack, 4u);
    write32(data + stack, byte_count);
    stack = sub32(stack, 4u);
    write32(data + stack, 0u);
    if (signed32(stack) < signed32(add32(hea, 64u))) return 3ull;

    Machine machine{state, image, code, data, cursor, 0u, 0u, hea, stack, 0u,
                    entry_stk, result, 0u, false};
    execute(machine);
    return machine.status;
}

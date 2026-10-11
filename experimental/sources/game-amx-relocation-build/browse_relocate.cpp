#include "init.hpp"
#include "script_start.hpp"
extern "C" unsigned long long norrath_amx_browse_relocate(void* amx) {
    using namespace norrath_constructor_detail;
    const Address state = address(amx);
    const Address header = read_word(state);
    const unsigned code_offset = read_word(header + 12u);
    const unsigned data_offset = read_word(header + 16u);
    const Address code = header + code_offset;
    const unsigned code_size = data_offset - code_offset;
    int label_pointer_word;
    // True null-AMX Exec API writes its genuine runtime handler-table address.
    (void)norrath_amx_exec(nullptr, &label_pointer_word, 0, 0, nullptr);
    unsigned offset = 0u;
    while (signed_word(offset) < signed_word(code_size)) {
        const Address instruction = code + offset;
        const unsigned opcode = read_word(instruction);
        if (signed_word(opcode) >= 256) return 6ull;
        const Address labels = static_cast<unsigned>(label_pointer_word);
        const unsigned handler = read_word(labels + (opcode << 2));
        offset += 4u;
        // Original installs the handler before its opcode1..130 case guard.
        write_word(instruction, handler);
        if (opcode - 1u >= 130u) return 6ull;
        switch (opcode) {
        case 1u:
        case 2u:
        case 3u:
        case 4u:
        case 5u:
        case 6u:
        case 7u:
        case 8u:
        case 10u:
        case 11u:
        case 12u:
        case 13u:
        case 14u:
        case 15u:
        case 16u:
        case 17u:
        case 18u:
        case 19u:
        case 20u:
        case 21u:
        case 22u:
        case 24u:
        case 26u:
        case 28u:
        case 29u:
        case 30u:
        case 31u:
        case 32u:
        case 38u:
        case 39u:
        case 40u:
        case 41u:
        case 44u:
        case 45u:
        case 52u:
        case 68u:
        case 69u:
        case 70u:
        case 71u:
        case 87u:
        case 88u:
        case 91u:
        case 92u:
        case 105u:
        case 106u:
        case 109u:
        case 110u:
        case 114u:
        case 115u:
        case 117u:
        case 118u:
        case 119u:
        case 120u:
        case 121u:
        case 123u:
            offset += 4u;
            break;
        case 9u:
        case 23u:
        case 25u:
        case 27u:
        case 33u:
        case 34u:
        case 35u:
        case 36u:
        case 37u:
        case 42u:
        case 43u:
        case 46u:
        case 47u:
        case 48u:
        case 50u:
        case 65u:
        case 66u:
        case 67u:
        case 72u:
        case 73u:
        case 74u:
        case 75u:
        case 76u:
        case 77u:
        case 78u:
        case 79u:
        case 80u:
        case 81u:
        case 82u:
        case 83u:
        case 84u:
        case 85u:
        case 86u:
        case 89u:
        case 90u:
        case 93u:
        case 94u:
        case 95u:
        case 96u:
        case 97u:
        case 98u:
        case 99u:
        case 100u:
        case 101u:
        case 102u:
        case 103u:
        case 104u:
        case 107u:
        case 108u:
        case 111u:
        case 112u:
        case 113u:
        case 116u:
        case 122u:
        case 128u:
            break;
        case 49u:
        case 51u:
        case 53u:
        case 54u:
        case 55u:
        case 56u:
        case 57u:
        case 58u:
        case 59u:
        case 60u:
        case 61u:
        case 62u:
        case 63u:
        case 64u:
        case 129u:
        {
            const Address operand = code + offset;
            offset += 4u;
            write_word(operand, read_word(operand) + code);
            break;
        }
        case 124u:
        {
            const unsigned length = read_word(code + offset);
            offset += 4u;
            const unsigned second = read_word(code + offset);
            offset += 4u;
            write_word(state + 44u, 1u);
            const Address name = code + offset;
            write_word(state + 40u, second);
            offset = offset + length - 4u;
            write_word(state + 56u, name);
            break;
        }
        case 125u:
        {
            const Address first = code + offset;
            write_word(state + 44u, 2u);
            offset += 4u;
            const Address second = code + offset;
            offset += 4u;
            write_word(state + 36u, read_word(first));
            write_word(state + 40u, read_word(second));
            break;
        }
        case 126u:
        {
            const Address first = code + offset;
            offset += 4u;
            const unsigned second = read_word(code + offset);
            offset += 4u;
            const Address third = code + offset;
            const unsigned length = read_word(first);
            offset += 4u;
            write_word(state + 48u, second);
            const Address name = code + offset;
            const unsigned after_length = offset + length;
            const unsigned third_value = read_word(third);
            offset = after_length - 8u;
            write_word(state + 44u, 3u);
            write_word(state + 52u, third_value);
            write_word(state + 56u, name);
            break;
        }
        case 127u:
        {
            const Address first = code + offset;
            offset += 4u;
            const unsigned first_value = read_word(first);
            const Address second = code + offset;
            offset += 4u;
            write_word(state + 48u, first_value);
            const unsigned second_value = read_word(second);
            write_word(state + 44u, 8u);
            write_word(state + 52u, second_value);
            break;
        }
        case 130u:
        {
            const unsigned count = read_word(code + offset);
            offset += 4u;
            if (signed_word(count) >= 0) {
                Address target = code + offset;
                unsigned remaining = count + 1u;
                do {
                    const unsigned value = read_word(target);
                    remaining -= 1u;
                    write_word(target, value + code);
                    target += 8u;
                } while (remaining != 0u);
            }
            offset += (count << 3) + 4u;
            break;
        }
        }
    }
    write_word(state + 32u, read_word(state + 32u) | 0x8000u);
    return 0ull;
}

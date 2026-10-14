#include "item_create.hpp"
using namespace norrath_item_detail;
extern "C" void norrath_player_create_item(char* name, void* description,
                                           int level, int named_table) {
    alignas(16) unsigned char workspace[304];
    const Address local = address(workspace), output = address(description);
    (void)norrath_factory_strcpy(text(local), name);

    // Only these native template bytes are initialized. All other72-byte
    // template contents remain unspecified incoming stack state, never zeros.
    Address option = local + 268u;
    for (unsigned remaining = 4u; remaining != 0u; --remaining, option -= 8u)
        put_half(option, 0xffffu);
    workspace[241] = 0xffu;
    workspace[240] = 0xffu;
    for (unsigned offset = 16u; offset != 24u; offset += 2u) {
        put_half(local + 208u + offset, 0u);
        put_half(local + 216u + offset, 0u);
    }
    copy_template(output, local + 208u);

    char* next = norrath_factory_strchr(text(local), 43);
    if (next != nullptr) { *next = 0; ++next; }
    const Address selected = named_table == 0
        ? lookup(address(norrath_item_table_view), text(local))
        : lookup(address(norrath_named_item_table_view), name);
    const unsigned selected_half = half(selected + 12u);
    const unsigned long long selected_word = wide(selected + 40u);
    put_half(output + 4u, selected_half);
    put_wide(output + 8u, selected_word);
    write_word(output, selected);

    const Address values = local + 288u;
    int* first = reinterpret_cast<int*>(pointer(values));
    int* second = reinterpret_cast<int*>(pointer(values + 4u));
    int* third = reinterpret_cast<int*>(pointer(values + 8u));
    while (next != nullptr) {
        char* segment = next;
        next = norrath_factory_strchr(segment, 43);
        if (next != nullptr) { *next = 0; ++next; }
        (void)norrath_block_memory_set(pointer(values), 0, 12u);
        // Any nonzero scanf result (including -1) takes the native match branch.
        // Failed calls may have written earlier conversions; do not reset here.
        if (norrath_player_sscanf(segment, text(address(norrath_item_format_454428)), first) != 0)
            set_option(output, 35u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454440)), first) != 0)
            set_option(output, 30u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454458)), first) != 0)
            set_option(output, 29u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454468)), first) != 0)
            set_option(output, 28u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454478)), first) != 0)
            set_option(output, 27u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454488)), first) != 0)
            set_option(output, 26u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454498)), first, second) != 0)
            set_option(output, 25u, read_word(values), read_word(values + 4u), 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4544a8)), first, second) != 0)
            set_option(output, 21u, read_word(values), read_word(values + 4u), 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4544b8)), first, second) != 0)
            set_option(output, 22u, read_word(values), read_word(values + 4u), 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4544c8)), first, second) != 0)
            set_option(output, 24u, read_word(values), read_word(values + 4u), 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4544e0)), first, second) != 0)
            set_option(output, 23u, read_word(values), read_word(values + 4u), 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4544f8)), first) != 0)
            set_option(output, 20u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454508)), first) != 0)
            set_option(output, 14u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454518)), first) != 0)
            set_option(output, 15u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454530)), first) != 0)
            set_option(output, 16u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454548)), first) != 0)
            set_option(output, 17u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454560)), first) != 0)
            set_option(output, 18u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454578)), first) != 0)
            set_option(output, 19u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454588)), first) != 0)
            set_option(output, 13u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454598)), first) != 0)
            set_option(output, 11u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4545b0)), first) != 0)
            set_option(output, 12u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4545c8)), first) != 0)
            set_option(output, 5u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4545d8)), first) != 0)
            set_option(output, 6u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4545e8)), first) != 0)
            set_option(output, 7u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4545f8)), first) != 0)
            set_option(output, 8u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454610)), first) != 0)
            set_option(output, 9u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454628)), first) != 0)
            set_option(output, 10u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454638)), first) != 0)
            set_option(output, 0u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454648)), first) != 0)
            set_option(output, 1u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454658)), first) != 0)
            set_option(output, 2u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454668)), first) != 0)
            set_option(output, 3u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454678)), first) != 0)
            set_option(output, 4u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454688)), first, second) != 0)
            set_option(output, 33u, read_word(values), read_word(values + 4u), 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4546a0)), first, second, third) != 0)
            set_option(output, 31u, read_word(values), read_word(values + 4u), read_word(values + 8u));
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4546b8)), first, second, third) != 0)
            set_option(output, 32u, read_word(values), read_word(values + 4u), read_word(values + 8u));
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4546d0)), first, second, third) != 0)
            set_option(output, 34u, read_word(values), read_word(values + 4u), read_word(values + 8u));
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_4546e8)), first) != 0)
            set_option(output, 36u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454700)), first) != 0)
            set_option(output, 37u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454710)), first) != 0)
            set_option(output, 39u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454720)), first) != 0)
            set_option(output, 40u, read_word(values), 0u, 0u);
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_483260)), first) != 0)
            level = signed_word(read_word(values));
        else if (norrath_player_sscanf(segment, text(address(norrath_item_format_454738)), first, second) != 0)
            set_option(output, 38u, read_word(values), read_word(values + 4u), 0u);
    }

    unsigned choice = unsigned(level);
    if (level == -1) {
        const Address current_row = read_word(output);
        choice = unsigned((wide(current_row + 40u) >> 11u) & 31ull);
        if (choice != 0u) {
            const Address seed_address = address(norrath_constructor_random_state_view);
            const unsigned seed = read_word(seed_address) * 0x0019660du + 0x3c6ef35fu;
            write_word(seed_address, seed);
            choice = (seed >> 16u) % choice;
        }
    }
    const unsigned shifted = choice << 11u;
    const unsigned long long old_word = wide(output + 8u);
    put_wide(output + 8u, (old_word & 0xffffffffffff07ffull) | signed_low_word(shifted));
}

#include "archive_services.hpp"
namespace {
using Address = unsigned;
using namespace norrath_constructor_dependency_detail;
unsigned half(Address p) { return unsigned(bytes(p)[0]) | (unsigned(bytes(p)[1]) << 8); }
int character(Address p) { const unsigned c = *bytes(p); return c < 128u ? int(c) : int(c) - 256; }
void put_half(Address p, unsigned v) {
    bytes(p)[0] = static_cast<unsigned char>(v); bytes(p)[1] = static_cast<unsigned char>(v >> 8);
}
unsigned signed_shift_right32(unsigned v, unsigned shift) {
    return (v >> shift) | ((v & 0x80000000u) != 0u ? (0xffffffffu << (32u - shift)) : 0u);
}
void copy_twelve(Address destination, Address source) {
    const unsigned x = read_word(source), y = read_word(source + 4u), z = read_word(source + 8u);
    write_word(destination, x); write_word(destination + 4u, y); write_word(destination + 8u, z);
}
// These three complete original LOCAL functions remain private source helpers.
// Their original LOCAL strict-public admission remains rejected, never aliased.
static unsigned initialize_lump(void* archive);
static void load_directory(Address directory, int first_sector, int sectors);
static void issue_command();
static void permanent_halt() {
    // The actual debug-halt branch never polls its flag again or returns.
    // A native no-op keeps this intentional endless target loop observable to
    // the compiler without inventing a callback, memory response or exit cap.
    for (;;) { __asm__ volatile ("nop"); }
}
static void issue_command() {
    const Address command = address(norrath_archive_command_view);
    const unsigned mode = read_word(command);
    if (mode != 1u && mode != 2u) return;
    norrath_archive_engine_push_border(0x00ffffffu);
    for (;;) {
        const unsigned sector = read_word(command + 4u);
        const unsigned count = read_word(command + 8u);
        void* destination = pointer(read_word(command + 12u));
        const int accepted = mode == 2u
            ? norrath_archive_cd_read_iop(sector, count, destination, norrath_archive_read_mode_view)
            : norrath_archive_cd_read(sector, count, destination, norrath_archive_read_mode_view);
        if (accepted != 0) break;
        norrath_archive_engine_sync_vblank();
        if (character(address(norrath_archive_permanent_halt_flag_view)) != 0) permanent_halt();
    }
    norrath_archive_engine_pop_border();
}
static void load_directory(Address directory, int first_sector, int sectors) {
    // Original non-save frame0..2399. Keep name2304 and live sector slots2368/2372
    // in one object; no sector/name zero fill or parser capacity is introduced.
    alignas(16) unsigned char workspace[2400];
    const Address frame = address(workspace);
    const Address sector_buffer = (frame + 255u) & 0xffffff00u;
    const Address name = frame + 2304u;
    write_word(frame + 2368u, static_cast<unsigned>(first_sector));
    write_word(frame + 2372u, static_cast<unsigned>(sectors));
    write_word(directory + 24576u, 0u);
    unsigned sector_index = 0u;
    if (signed_word(read_word(frame + 2372u)) <= 0) return;
    do {
        const unsigned sector = read_word(frame + 2368u) + sector_index;
        (void)norrath_archive_cd_read(sector, 1u, pointer(sector_buffer), norrath_archive_read_mode_view);
        (void)norrath_archive_cd_sync(0);
        unsigned offset = 0u;
        Address record = sector_buffer;
        while (*bytes(record) != 0u) {
            const unsigned name_length = *bytes(record + 32u);
            unsigned input = offset + 33u;
            const unsigned last = offset + name_length + 32u;
            unsigned copied = 0u;
            if (signed_word(last) >= signed_word(input)) {
                do {
                    const unsigned char c = *bytes(sector_buffer + input);
                    input += 1u;
                    *bytes(name + copied) = c;
                    copied += 1u;
                } while (!(signed_word(last) < signed_word(input)));
            }
            *bytes(name + copied) = 0u;
            const unsigned logical_sector = read_word(sector_buffer + offset + 2u);
            const unsigned file_size = read_word(sector_buffer + offset + 10u);
            const unsigned before = read_word(directory + 24576u);
            norrath_factory_strcpy(text(directory + (before << 5)), text(name));
            const unsigned after_name = read_word(directory + 24576u);
            write_word(directory + (after_name << 5) + 16u, logical_sector);
            const unsigned after_sector = read_word(directory + 24576u);
            write_word(directory + (after_sector << 5) + 20u, file_size);
            const unsigned before_increment = read_word(directory + 24576u);
            write_word(directory + 24576u, before_increment + 1u);
            offset += *bytes(record);
            record = sector_buffer + offset;
        }
        sector_index += 1u;
    } while (signed_word(sector_index) < signed_word(read_word(frame + 2372u)));
}
}
extern "C" int norrath_cd_directory_lookup(char* name, int* sector_out, int* bytes_out, int) {
    using namespace norrath_constructor_dependency_detail;
    // Full observed non-save SP0..175, including name0 and search results80/128.
    alignas(16) unsigned char workspace[176];
    const Address frame = address(workspace);
    norrath_factory_sprintf(text(frame), text(address(norrath_archive_lookup_format_view)), name);
    Address cursor = frame;
    unsigned raw = *bytes(cursor);
    while (raw != 0u) {
        const int c = raw < 128u ? int(raw) : int(raw) - 256;
        const unsigned flags = norrath_factory_ctype_minimum_view[unsigned(c + 128)];
        *bytes(cursor) = static_cast<unsigned char>((flags & 2u) != 0u ? c - 32 : c);
        cursor += 1u; raw = *bytes(cursor);
    }
    unsigned index = 0u;
    Address directory;
    unsigned switch_enabled;
    for (;;) {
        directory = read_word(address(norrath_archive_directory_pointer_view));
        const int count = signed_word(read_word(directory + 24576u));
        switch_enabled = read_word(address(norrath_archive_directory_switch_view));
        if (!(signed_word(index) < count)) break;
        if (norrath_string_compare(text(frame), text(directory + (index << 5))) == 0) {
            directory = read_word(address(norrath_archive_directory_pointer_view));
            switch_enabled = read_word(address(norrath_archive_directory_switch_view));
            break;
        }
        index += 1u;
    }
    if (switch_enabled != 0u && index == read_word(directory + 24576u)) {
        const Address first = address(norrath_archive_directory_a_view);
        Address candidate;
        if (directory == first) {
            norrath_cd_wait();
            const int found = norrath_archive_cd_layer_search(bytes(frame + 80u),
                                      text(address(norrath_archive_directory_name_view)), 1);
            candidate = address(norrath_archive_directory_b_view);
            if (read_word(candidate + 24576u) == 0u && found != 0) {
                const unsigned length = read_word(frame + 84u);
                const unsigned blocks = (length + 2047u) >> 11;
                load_directory(candidate, signed_word(read_word(frame + 80u)), signed_word(blocks));
            }
        } else {
            candidate = first;
            norrath_cd_wait();
            (void)norrath_archive_cd_layer_search(bytes(frame + 128u),
                                      text(address(norrath_archive_directory_name_view)), 0);
            // The actual alternate path does NOT load_directory(first).
        }
        index = 0u;
        Address row = candidate;
        for (;;) {
            const unsigned candidate_count = read_word(candidate + 24576u);
            directory = read_word(address(norrath_archive_directory_pointer_view));
            if (!(signed_word(index) < signed_word(candidate_count))) break;
            const Address current_row = row;
            row += 32u;
            if (norrath_string_compare(text(frame), text(current_row)) == 0) {
                const unsigned fresh_count = read_word(candidate + 24576u);
                directory = read_word(address(norrath_archive_directory_pointer_view));
                if (signed_word(index) < signed_word(fresh_count)) {
                    directory = candidate;
                    write_word(address(norrath_archive_directory_pointer_view), directory);
                    bytes(address(norrath_archive_read_mode_view))[1] = 0u;
                }
                break;
            }
            index += 1u;
        }
    }
    // Preserve actual equality test, including odd negative/count/fallback states.
    if (index == read_word(directory + 24576u)) return 0;
    if (sector_out != nullptr) {
        unsigned sector = read_word(directory + (index << 5) + 16u);
        if (directory == address(norrath_archive_directory_b_view))
            sector += read_word(address(norrath_archive_layer_bias_view));
        write_word(address(sector_out), sector);
    }
    if (bytes_out != nullptr) {
        const Address fresh_directory = read_word(address(norrath_archive_directory_pointer_view));
        write_word(address(bytes_out), read_word(fresh_directory + (index << 5) + 20u));
    }
    return 1;
}
extern "C" void norrath_cd_wait() {
    using namespace norrath_constructor_dependency_detail;
    norrath_archive_engine_push_border(0x00ffffffu);
    while (character(address(norrath_archive_read_active_view)) != 0) {
        norrath_cd_process(); norrath_archive_engine_sync_vblank();
    }
    norrath_archive_engine_pop_border();
    if (character(address(norrath_archive_stream_active_view)) != 0) {
        (void)norrath_archive_cd_stream_pause();
        *norrath_archive_stream_active_view = 0u;
        *norrath_archive_stream_restart_view = 1u;
    }
}
extern "C" void norrath_cd_load(char* name, void* destination, int requested, int byte_offset) {
    using namespace norrath_constructor_dependency_detail;
    alignas(8) unsigned char local[16];
    const Address frame = address(local);
    norrath_cd_wait();
    const int found = norrath_cd_directory_lookup(name,
        reinterpret_cast<int*>(static_cast<__UINTPTR_TYPE__>(frame + 8u)),
        reinterpret_cast<int*>(static_cast<__UINTPTR_TYPE__>(frame + 4u)), 0);
    if (found == 0) {
        norrath_sce_printf(text(address(norrath_archive_load_failure_format_view)), name);
        norrath_loader_failure_word_view = 2u;
    } else {
        const unsigned length = requested == 0 ? read_word(frame + 4u) : static_cast<unsigned>(requested);
        const unsigned sector = read_word(frame + 8u) + signed_shift_right32(static_cast<unsigned>(byte_offset), 11u);
        const unsigned blocks = signed_shift_right32(length + 2047u, 11u);
        const Address command = address(norrath_archive_command_view);
        write_word(command + 12u, address(destination)); write_word(command + 4u, sector);
        write_word(command + 8u, blocks);
        write_word(address(norrath_archive_completion_pointer_view), frame);
        *bytes(frame) = 0u; write_word(command, 1u);
        issue_command();
        *norrath_archive_read_active_view = 1u;
    }
    norrath_archive_engine_push_border(0x00ffffffu);
    while (character(address(norrath_archive_read_active_view)) != 0) {
        norrath_cd_process(); norrath_archive_engine_sync_vblank();
    }
    norrath_archive_engine_pop_border();
}
extern "C" void norrath_cd_process() {
    using namespace norrath_constructor_dependency_detail;
    const int active = character(address(norrath_archive_read_active_view));
    if (active == 1) {
        bool complete = false;
        if (character(address(norrath_archive_stream_active_view)) == 0) {
            if (norrath_archive_cd_sync(1) == 0) {
                if (norrath_archive_cd_error() != 0) issue_command();
                else complete = true;
            }
        } else {
            const unsigned remaining = read_word(address(norrath_archive_stream_remaining_view));
            if (signed_word(remaining) > 0) {
                int error = 0;
                const unsigned blocks = signed_shift_right32(remaining, 11u);
                void* destination = pointer(read_word(address(norrath_archive_stream_destination_view)));
                const int returned = norrath_archive_cd_stream_read(blocks, destination, 0, &error);
                const unsigned bytes_read = static_cast<unsigned>(returned) << 11;
                const unsigned fresh_remaining = read_word(address(norrath_archive_stream_remaining_view)) - bytes_read;
                const unsigned fresh_destination = read_word(address(norrath_archive_stream_destination_view)) + bytes_read;
                write_word(address(norrath_archive_stream_remaining_view), fresh_remaining);
                write_word(address(norrath_archive_stream_destination_view), fresh_destination);
                if (fresh_remaining == 0u) complete = true;
            }
        }
        if (complete) {
            const Address completion = read_word(address(norrath_archive_completion_pointer_view));
            *bytes(completion) = static_cast<unsigned char>(active);
            *norrath_archive_read_active_view = 0u;
        }
    }
    const Address archive = read_word(address(norrath_loader_active_buffer_view));
    if (archive != 0u && character(address(norrath_loader_active_marker_view)) != 0) {
        const unsigned original_size = read_word(address(norrath_loader_active_size_view));
        const unsigned sound_before = norrath_archive_sound_free_ram();
        const unsigned discarded = initialize_lump(pointer(archive));
        const unsigned resized = original_size - discarded + 256u;
        (void)norrath_mem_realloc(pointer(archive), signed_word(resized));
        const Address cache = address(norrath_constructor_lump_cache_view);
        unsigned index = read_word(address(norrath_constructor_lump_count_view));
        write_word(cache + index * 40u + 24u, resized);
        const unsigned sound_after = norrath_archive_sound_free_ram();
        index = read_word(address(norrath_constructor_lump_count_view));
        const Address record = cache + index * 40u;
        write_word(record + 28u, sound_before - sound_after);
        write_word(record + 20u, 0u); write_word(record + 32u, 0u);
        norrath_factory_strcpy(text(record), text(address(norrath_loader_active_name_view)));
        index = read_word(address(norrath_constructor_lump_count_view));
        write_word(address(norrath_loader_active_buffer_view), 0u);
        write_word(address(norrath_constructor_lump_count_view), index + 1u);
        *norrath_loader_active_marker_view = 0u;
        write_word(cache + index * 40u + 16u, archive);
        write_word(cache + index * 40u + 36u, 1u);
    }
}
namespace {
static unsigned initialize_lump(void* archive) {
    const Address base = address(archive), records = base + 4u;
    unsigned total = half(base);
    norrath_mem_check_heap_real(text(address(norrath_archive_check_file_view)), 298);
    unsigned dropped = 0u;
    Address latest_data = 0u;
    unsigned index = 0u;
    while (signed_word(index) < signed_word(total)) {
        const Address record = records + index * 12u;
        const Address data = read_word(record + 4u) + base;
        const Address name = read_word(record) + base;
        write_word(record + 4u, data); write_word(record, name);
        unsigned next_index = index + 1u;
        // Alias entries are rebased but otherwise skipped exactly as observed.
        if (latest_data < data) {
            const unsigned original_size = read_word(record + 8u);
            if (original_size == 0u) {
                norrath_sce_printf(text(address(norrath_archive_zero_record_format_view)));
                norrath_loader_failure_word_view = 0u;
            }
            latest_data = read_word(record + 4u);
            if (signed_word(dropped) > 0) {
                for (unsigned future = index + 1u; signed_word(future) < signed_word(total); ++future) {
                    const Address other = records + future * 12u;
                    const unsigned offset = read_word(other + 4u);
                    const unsigned new_offset = offset - dropped;
                    if (offset + base == read_word(record + 4u)) write_word(other + 4u, new_offset);
                }
                const Address destination = read_word(record + 4u) - dropped;
                const unsigned size = read_word(record + 8u);
                write_word(record + 4u, destination);
                // Real original memcpy forward capture semantics, not host memmove.
                (void)norrath_copy_bytes(pointer(destination), pointer(latest_data), size);
            }
            Address current_name = read_word(record);
            if (norrath_factory_strncmp(text(current_name), text(address(norrath_archive_prefix_five_view)), 5u) == 0) {
                const Address suffix = current_name + 5u;
                const unsigned length = norrath_factory_strlen(text(current_name));
                (void)norrath_archive_move_bytes(pointer(current_name), pointer(suffix), length - 4u);
            }
            current_name = read_word(record);
            bool remove = false;
            if (norrath_factory_strncmp(text(current_name), text(address(norrath_archive_prefix_four_view)), 4u) == 0) {
                const Address suffix = current_name + 4u;
                const unsigned length = norrath_factory_strlen(text(current_name));
                (void)norrath_archive_move_bytes(pointer(current_name), pointer(suffix), length - 3u);
                remove = true;
            }
            unsigned duplicates = 0u;
            if (signed_word(index) > 0) {
                const Address current_data = read_word(record + 4u);
                for (unsigned previous = 0u; previous < index; ++previous)
                    if (read_word(records + previous * 12u + 4u) == current_data) duplicates += 1u;
            }
            if (signed_word(index + 1u) < signed_word(total)) {
                const Address current_data = read_word(record + 4u);
                for (unsigned future = index + 1u; signed_word(future) < signed_word(total); ++future)
                    if (read_word(records + future * 12u + 4u) + base == current_data) duplicates += 1u;
            }
            if (signed_word(duplicates) > 0) remove = false;
            if (remove) {
                dropped = (dropped + read_word(record + 8u)) & 0xffffff80u;
                if (signed_word(index) < signed_word(total)) {
                    unsigned copies = total - index;
                    Address cursor = record;
                    do {
                        // The final iteration really reads one trailing12-byte
                        // entry past the current logical record count. No cap.
                        copy_twelve(cursor, cursor + 12u);
                        cursor += 12u;
                        copies -= 1u;
                    } while (copies != 0u);
                }
                const unsigned fresh_header_count = half(base);
                next_index = index;
                total -= 1u;
                put_half(base, fresh_header_count - 1u);
            } else {
                const unsigned size = read_word(record + 8u);
                char* filename = text(read_word(record));
                void* payload = pointer(read_word(record + 4u));
                const unsigned discard = norrath_archive_initialize_file(filename, payload, signed_word(size));
                dropped = (dropped + discard) & 0xffffff80u;
            }
        }
        index = next_index;
    }
    if (signed_word(total) > 0) {
        Address record = records;
        unsigned remaining = total;
        do {
            char* extension = norrath_archive_last_character(text(read_word(record)), 46);
            if (extension != nullptr && norrath_string_compare(extension + 1,
                                      text(address(norrath_archive_animation_extension_view))) == 0)
                norrath_archive_map_animation_sounds(pointer(read_word(record + 4u)));
            remaining -= 1u;
            record += 12u;
        } while (remaining != 0u);
    }
    norrath_mem_check_heap_real(text(address(norrath_archive_check_file_view)), 400);
    return dropped;
}
}

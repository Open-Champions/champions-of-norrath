// Host contract for the constructor and object-table state. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" {
extern unsigned char norrath_constructor_default_head_view[4];
extern unsigned char norrath_create_by_name_function_word[4];
extern unsigned char norrath_constructor_string_count_view[4];
extern unsigned char norrath_constructor_list_pool_view[4];
extern unsigned char norrath_constructor_random_state_view[4];
extern unsigned char norrath_constructor_resource_frame_view[4];
extern unsigned char norrath_constructor_lump_count_view[4];
extern unsigned char norrath_constructor_grid_a_view[120 * 120 * 4];
extern unsigned char norrath_constructor_grid_b_view[120 * 120 * 4];
extern unsigned char norrath_constructor_lump_cache_view[128 * 40];
extern unsigned char norrath_constructor_string_names_view[512 * 4];
extern unsigned char norrath_constructor_string_objects_view[512 * 4];
extern unsigned char norrath_object_name_hash_slots_view[];
extern unsigned char norrath_object_pointer_hash_slots_view[];
extern unsigned char norrath_constructor_tag_a_view[];
extern unsigned char norrath_constructor_tag_b_view[];
extern unsigned char norrath_factory_marker_data_view[36];
}

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) |
           (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static bool all_zero(const unsigned char* p, unsigned size) {
    for (unsigned i = 0; i < size; ++i) if (p[i] != 0) return false;
    return true;
}

int main() {
    expect(sizeof norrath_constructor_default_head_view == 4, "default head size");
    expect(sizeof norrath_create_by_name_function_word == 4, "create-by-name word size");
    expect(sizeof norrath_constructor_string_count_view == 4, "string count size");
    expect(sizeof norrath_constructor_list_pool_view == 4, "list pool size");
    expect(sizeof norrath_constructor_random_state_view == 4, "random state size");
    expect(sizeof norrath_constructor_resource_frame_view == 4, "resource frame size");
    expect(sizeof norrath_constructor_lump_count_view == 4, "lump count size");
    expect(sizeof norrath_constructor_grid_a_view == 57600u, "grid a is 120 by 120 words");
    expect(sizeof norrath_constructor_grid_b_view == 57600u, "grid b is 120 by 120 words");
    expect(sizeof norrath_constructor_lump_cache_view == 5120u, "lump cache is 128 records of 40");
    expect(sizeof norrath_constructor_string_names_view == 2048u, "name table is 512 words");
    expect(sizeof norrath_constructor_string_objects_view == 2048u, "object table is 512 words");
    expect(sizeof norrath_factory_marker_data_view == 36u, "marker is 36 bytes");
    expect(all_zero(norrath_constructor_default_head_view, 4), "default head starts clear");
    expect(all_zero(norrath_create_by_name_function_word, 4), "create-by-name word starts clear");
    expect(all_zero(norrath_constructor_string_count_view, 4), "string count starts clear");
    expect(all_zero(norrath_constructor_list_pool_view, 4), "list pool starts clear");
    expect(all_zero(norrath_constructor_random_state_view, 4), "random state starts clear");
    expect(all_zero(norrath_constructor_lump_count_view, 4), "lump count starts clear");
    expect(all_zero(norrath_constructor_grid_a_view, 57600u), "grid a starts clear");
    expect(all_zero(norrath_constructor_grid_b_view, 57600u), "grid b starts clear");
    expect(all_zero(norrath_constructor_lump_cache_view, 5120u), "lump cache starts clear");
    expect(all_zero(norrath_constructor_string_names_view, 2048u), "name table starts clear");
    expect(all_zero(norrath_constructor_string_objects_view, 2048u), "object table starts clear");
    expect(all_zero(norrath_factory_marker_data_view, 36), "marker starts clear");
    expect(read_word(norrath_constructor_resource_frame_view) == 1u, "resource frame starts at one");
    expect(std::strcmp(reinterpret_cast<char*>(norrath_constructor_tag_a_view), "hide") == 0, "tag a");
    expect(std::strcmp(reinterpret_cast<char*>(norrath_constructor_tag_b_view), "name") == 0, "tag b");
    expect(+norrath_object_name_hash_slots_view == +norrath_constructor_string_names_view,
           "name hash slots are the constructor name table");
    expect(+norrath_object_pointer_hash_slots_view == +norrath_constructor_string_objects_view,
           "pointer hash slots are the constructor object table");
    norrath_object_name_hash_slots_view[2047] = 0x5a;
    expect(norrath_constructor_string_names_view[2047] == 0x5a, "name alias is the same storage");
    norrath_constructor_string_names_view[2047] = 0;
    norrath_object_pointer_hash_slots_view[0] = 0xa5;
    expect(norrath_constructor_string_objects_view[0] == 0xa5, "pointer alias is the same storage");
    norrath_constructor_string_objects_view[0] = 0;
    if (failures != 0) return 1;
    std::printf("object state contract passed\n");
    return 0;
}

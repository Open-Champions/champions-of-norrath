#include "actors.hpp"
#include "list_remove.hpp"
#include "load_entities.hpp"
namespace {
using Address = unsigned;
using Wide = unsigned long long;
struct PointBytes { unsigned x, y, z; };
static_assert(sizeof(PointBytes) == 12 && alignof(PointBytes) == 4, "raw point storage");
Address address(const void* p) { return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
void* pointer(Address a) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(a)); }
char* text(unsigned char* a) { return reinterpret_cast<char*>(a); }
unsigned word(Address a) {
    return unsigned(bytes(a)[0]) | (unsigned(bytes(a)[1]) << 8) |
           (unsigned(bytes(a)[2]) << 16) | (unsigned(bytes(a)[3]) << 24);
}
unsigned half(Address a) { return unsigned(bytes(a)[0]) | (unsigned(bytes(a)[1]) << 8); }
Wide wide(Address a) { return Wide(word(a)) | (Wide(word(a + 4u)) << 32); }
void put_word(Address a, unsigned v) {
    bytes(a)[0] = static_cast<unsigned char>(v); bytes(a)[1] = static_cast<unsigned char>(v >> 8);
    bytes(a)[2] = static_cast<unsigned char>(v >> 16); bytes(a)[3] = static_cast<unsigned char>(v >> 24);
}
void put_half(Address a, unsigned v) {
    bytes(a)[0] = static_cast<unsigned char>(v); bytes(a)[1] = static_cast<unsigned char>(v >> 8);
}
void put_wide(Address a, Wide v) { put_word(a, unsigned(v)); put_word(a + 4u, unsigned(v >> 32)); }
int signed_word(unsigned v) { return v <= 0x7fffffffu ? int(v) : -1 - int(0xffffffffu - v); }
PointBytes point_copy(const void* p) {
    const Address a = address(p);
    return PointBytes{word(a), word(a + 4u), word(a + 8u)};
}
float float_bits(unsigned bits) { float v; put_word(address(&v), bits); return v; }
float real_float(unsigned char* view) { return float_bits(word(address(view))); }
void put_float(Address a, float v) { put_word(a, word(address(&v))); }
void* object_resource(Address object, unsigned offset) {
    return pointer(word(word(object + 36u) + offset));
}
unsigned creature_parent(void* object, const void* point, short type, char** arguments,
                         unsigned char* map, float scale) {
    PointBytes local = point_copy(point);
    return norrath_creature_construct(object, &local, type, map, scale, arguments);
}
}
// eRand__10GameObject at153fa8/40; real object68 state, low32 LCG then logical16 shift.
extern "C" unsigned norrath_actor_random(void* object) {
    const Address a = address(object) + 68u;
    const unsigned next = word(a) * 0x19660du + 0x3c6ef35fu;
    put_word(a, next);
    return next >> 16;
}
// animInitState__FP14AnimationState at1637a8/8 changes ONLY byte4.
extern "C" void norrath_actor_animation_initialize(void* state) { bytes(address(state))[4] = 0u; }
// modelGetActiveChunks at16c970/88: full64-bit bitmap complement and selected modifier rows.
extern "C" Wide norrath_actor_model_active_chunks(void* model, int requested) {
    const Address a = address(model);
    const unsigned flags = half(a + 16u);
    Wide result = ~wide(a + 8u);
    unsigned selection = static_cast<unsigned>(requested);
    if ((flags & 2u) != 0u && selection != 0u) {
        Address row = a + unsigned(bytes(a)[33]) * 16u;
        do {
            const bool selected = (selection & 1u) != 0u;
            selection >>= 1;
            if (selected) {
                const Wide enable = wide(row), disable = wide(row + 8u);
                result = (result | enable) & ~disable;
            }
            row += 16u;
        } while (selection != 0u);
    }
    return result;
}
// Whole original run-list wrappers; raw low V0 is the real list link pointer.
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    const Address a = address(object);
    const unsigned existing = word(a + 4u);
    if (existing != 0u) (void)norrath_list_remove(bytes(existing));
    return address(norrath_list_oneway_link(norrath_actor_view_486878, pointer(a + 4u), object));
}
extern "C" unsigned norrath_actor_add_slow_run_list(void* object) {
    const Address a = address(object);
    const unsigned existing = word(a + 4u);
    if (existing != 0u) (void)norrath_list_remove(bytes(existing));
    unsigned next = word(address(norrath_actor_view_483ad0)) + 1u;
    if (signed_word(next) >= 60) next = 0u;
    // Original signed comparison has no lower bound. No fabricated clamp repairs it.
    void* head = pointer(address(norrath_actor_view_50bfd0) + (next << 2));
    put_word(address(norrath_actor_view_483ad0), next);
    return address(norrath_list_oneway_link(head, pointer(a + 4u), object));
}
extern "C" unsigned norrath_construct_CdEyeball(void* object, const void* point, short type, char** arguments) {
    (void)creature_parent(object, point, type, arguments, norrath_actor_view_42c710, 1.0f);
    const Address a = address(object);
    put_half(a + 962u, unsigned(type));
    put_word(a + 72u, address(norrath_actor_view_44b170));
    put_word(a + 168u, 0x3f800000u);
    const int first = norrath_game_object_find_tag_int(object, text(norrath_actor_view_485410), 0);
    put_word(a + 224u, 0x3f800000u);
    put_half(a + 960u, static_cast<unsigned>(first));
    const int scale = norrath_game_object_find_tag_int(object, text(norrath_actor_view_485418), 0);
    if (scale != 0) {
        const float converted = static_cast<float>(scale);
        const float factor = real_float(norrath_actor_view_4816d8);
        const float product = converted * factor;
        put_float(a + 224u, product);
    }
    put_word(a + 12u, word(a + 12u) | 9u);
    return norrath_actor_add_slow_run_list(object);
}
extern "C" unsigned norrath_construct_Cat(void* object, const void* point, short type, char** arguments) {
    (void)creature_parent(object, point, type, arguments, norrath_actor_view_42b118, 1000.0f);
    const Address a = address(object), animation = a + 288u;
    const unsigned global = word(address(norrath_actor_view_4844a4));
    put_word(a + 176u, 0x41a00000u); put_word(a + 28u, 0x41200000u);
    put_word(a + 72u, address(norrath_actor_view_44c170)); put_word(a + 248u, global);
    put_word(a + 168u, 0x3f000000u); put_word(a + 192u, 0u); put_word(a + 236u, 0u);
    (void)norrath_actor_set_parameters(object, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 1);
    const unsigned first = norrath_actor_random(object);
    put_word(a + 968u, 0u); put_word(a + 972u, 0u); put_word(a + 960u, first % 9u);
    const unsigned second = norrath_actor_random(object);
    const float converted = static_cast<float>(signed_word(second));
    const float scaled = converted * float_bits(0x37800000u);
    const float halved = scaled * 0.5f;
    const float shifted = halved + 1.5f;
    put_float(a + 224u, shifted);
    const int variant = norrath_game_object_find_tag_int(object, text(norrath_actor_view_485150), 0);
    put_word(a + 964u, static_cast<unsigned>(variant));
    norrath_actor_animation_initialize(pointer(animation));
    if (word(a + 964u) == 0u) {
        void* resource = object_resource(a, 116u);
        put_half(a + 572u, 0u);
        norrath_actor_animation_add(pointer(animation), resource, 1);
    } else {
        const Address resource_map = word(a + 36u);
        put_half(a + 572u, 13u);
        norrath_actor_animation_add(pointer(animation), pointer(word(resource_map + 132u)), 1);
    }
    return norrath_actor_add_slow_run_list(object);
}
extern "C" unsigned norrath_construct_Mermaid(void* object, const void* point, short type, char** arguments) {
    (void)creature_parent(object, point, type, arguments, norrath_actor_view_42b768, 10.0f);
    const Address a = address(object), animation = a + 288u;
    const Wide flags = wide(a + 160u) | 0x200040000ull;
    bytes(a)[41] = 50u; bytes(a)[40] = 50u;
    const float parameter = real_float(norrath_actor_view_480eec);
    const unsigned global = word(address(norrath_actor_view_4844a4));
    put_wide(a + 160u, flags); put_word(a + 28u, 0x41c80000u);
    put_float(a + 168u, parameter); put_word(a + 224u, 0x3f800000u);
    const float first_parameter = real_float(norrath_actor_view_480ef0);
    put_word(a + 72u, address(norrath_actor_view_44e1c0)); put_word(a + 248u, global);
    put_word(a + 176u, 0u); put_word(a + 192u, 0u); put_word(a + 236u, 0u);
    put_half(a + 572u, 0u); put_word(a + 256u, 0u);
    (void)norrath_actor_set_parameters(object, first_parameter, parameter, 1.0f, 1.0f, 1.0f, 1);
    norrath_actor_animation_initialize(pointer(animation));
    norrath_actor_animation_add(pointer(animation), object_resource(a, 92u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    return norrath_actor_finish_construction(object);
}
extern "C" unsigned norrath_construct_SmallSpider(void* object, const void* point, short type, char** arguments) {
    (void)creature_parent(object, point, type, arguments, norrath_actor_view_42c130, 1.0f);
    const Address a = address(object), animation = a + 288u;
    const Wide flags = wide(a + 160u) | 0x8004001000ull;
    const unsigned global = word(address(norrath_actor_view_4844a4));
    const float parameter = real_float(norrath_actor_view_4813a4);
    const float scale = real_float(norrath_actor_view_4813a8);
    const float input = real_float(norrath_actor_view_4813ac);
    put_word(a + 72u, address(norrath_actor_view_44b540)); put_float(a + 168u, parameter);
    put_word(a + 248u, global); put_word(a + 28u, 0x40a00000u);
    put_float(a + 224u, scale); put_word(a + 176u, 0x42480000u);
    put_half(a + 56u, unsigned(type)); bytes(a)[172] = 18u; put_wide(a + 160u, flags);
    put_word(a + 96u, 0x3f800000u); put_word(a + 236u, 0u); put_half(a + 572u, 0u);
    (void)norrath_actor_set_parameters(object, input, input, input, input, input, 1);
    norrath_actor_animation_add(pointer(animation), object_resource(a, 60u), 1);
    norrath_actor_animation_randomize_time(pointer(animation));
    put_half(a + 572u, 0u);
    (void)norrath_actor_add_slow_run_list(object);
    return norrath_actor_finish_construction(object);
}
extern "C" unsigned norrath_construct_GnomeNavigator(void* object, const void* point, short type, char** arguments) {
    (void)creature_parent(object, point, type, arguments, norrath_actor_view_42baa8, 10.0f);
    const Address a = address(object), animation = a + 288u;
    const unsigned flags = word(a + 12u) | 0x1000000u;
    bytes(a)[40] = 15u;
    const Wide creature_flags = wide(a + 160u) | 0x400040000ull;
    const unsigned global = word(address(norrath_actor_view_4844a4));
    const float parameter = real_float(norrath_actor_view_48113c);
    put_word(a + 72u, address(norrath_actor_view_44e6a0)); put_word(a + 28u, 0x41700000u);
    put_word(a + 248u, global); put_half(a + 792u, 0xffffu); put_float(a + 168u, parameter);
    const float first = real_float(norrath_actor_view_481140);
    put_word(a + 12u, flags); bytes(a)[172] = 9u; put_wide(a + 160u, creature_flags);
    put_word(a + 176u, 0u); put_word(a + 192u, 0u); put_word(a + 236u, 0u); put_half(a + 572u, 0u);
    (void)norrath_actor_set_parameters(object, first, parameter, 1.0f, 1.0f, 1.0f, 1);
    put_word(a + 244u, 0u);
    const Address map = word(a + 36u);
    const Wide active = norrath_actor_model_active_chunks(pointer(word(map + 12u)), 0);
    put_wide(a + 552u, active);
    norrath_actor_animation_initialize(pointer(animation));
    norrath_actor_animation_add(pointer(animation), object_resource(a, 28u), 1);
    put_word(a + 236u, 1u);
    (void)norrath_actor_add_run_list(object);
    return norrath_actor_finish_construction(object);
}
extern "C" unsigned norrath_construct_Natasla(void* object, const void* point, short type, char** arguments) {
    (void)creature_parent(object, point, type, arguments, norrath_actor_view_42d0c0, 10.0f);
    const Address a = address(object), animation = a + 288u;
    const unsigned flags = word(a + 12u) | 0x1000008u;
    const Wide creature_flags = wide(a + 160u) | 0x200040000ull;
    const unsigned global = word(address(norrath_actor_view_4844a4));
    const float parameter = real_float(norrath_actor_view_481a08);
    put_float(a + 168u, parameter); put_word(a + 28u, 0x41700000u);
    put_word(a + 248u, global); put_word(a + 12u, flags); put_wide(a + 160u, creature_flags);
    put_word(a + 176u, 0u); put_word(a + 192u, 0u); put_word(a + 236u, 0u);
    put_half(a + 572u, 0u); put_word(a + 256u, 0u);
    put_word(a + 72u, address(norrath_actor_view_44f4c8));
    // Five stores at104,100,96,92,88; the original BGEZ loop includes its zero iteration.
    Address field = a + 104u;
    for (unsigned count = 0; count < 5u; ++count) { put_word(field, 0x3f800000u); field -= 4u; }
    const float first = real_float(norrath_actor_view_481a0c);
    const float second = real_float(norrath_actor_view_481a10);
    put_word(a + 224u, 0x3f800000u);
    (void)norrath_actor_set_parameters(object, first, second, 1.0f, 1.0f, 1.0f, 1);
    put_word(a + 560u, 0x447a0000u);
    norrath_actor_animation_initialize(pointer(animation));
    norrath_actor_animation_add(pointer(animation), object_resource(a, 28u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    return norrath_actor_finish_construction(object);
}
// Complete Creature852 parent, including fresh repeated tag queries and real script/grid effects.
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type,
                                               void* resource_map, float scale, char** arguments) {
    const Address a = address(object);
    PointBytes local = point_copy(point);
    (void)norrath_game_object_construct(object, &local, resource_map, arguments);
    bytes(a)[293] = 8u;
    put_word(a + 768u, 0u); put_word(a + 760u, 0u); put_word(a + 764u, 0u);
    put_word(a + 72u, address(norrath_actor_view_4488f0));
    put_word(a + 576u, 0u); put_word(a + 752u, 0u);
    // The original reloads the local point representation AFTER the base call.
    const PointBytes fresh = point_copy(&local);
    put_word(a + 776u, fresh.x); put_word(a + 780u, fresh.y); put_word(a + 784u, fresh.z);
    put_word(a + 224u, 0x3f800000u); put_word(a + 228u, 0u);
    const unsigned flags = word(a + 12u) | 0x20u;
    put_word(a + 192u, 0u); put_word(a + 12u, flags); put_half(a + 56u, unsigned(type));
    put_float(a + 564u, scale); put_wide(a + 552u, 0xffffffffffffffffull);
    put_word(a + 888u, 0u); put_float(a + 560u, scale);
    put_word(a + 580u, 0xffffffffu); put_word(a + 584u, 0xffffffffu);
    put_word(a + 708u, 0u); put_word(a + 712u, 0u); put_half(a + 284u, 0u);
    put_word(a + 744u, 0u); bytes(a)[570] = 0u; put_word(a + 256u, 0u);
    put_word(a + 716u, 0u); put_half(a + 792u, 0u); put_half(a + 796u, 0u);
    put_word(a + 808u, 0u); put_word(a + 812u, 0u); bytes(a)[800] = 0u;
    bytes(a)[76] = 0u; bytes(a)[85] = 255u; bytes(a)[156] = 0u;
    const unsigned position_z = word(a + 24u);
    const unsigned index = *norrath_actor_view_484f98;
    put_word(a + 60u, position_z); put_word(a + 788u, 0x447a0000u);
    put_word(a + 208u, 0u); put_word(a + 216u, 0u); put_word(a + 212u, 0u);
    *norrath_actor_view_484f98 = static_cast<unsigned char>(index + 1u);
    bytes(a)[569] = static_cast<unsigned char>(index);
    if (*norrath_actor_view_484318 == 0u) {
        char* script = norrath_actor_find_tag_string(object, text(norrath_actor_view_460710), 0);
        if (script != nullptr) (void)norrath_script_start(script);
    }
    if (norrath_game_object_find_tag_int(object, text(norrath_actor_view_4837a8), 0) != 0) {
        const int fresh_value = norrath_game_object_find_tag_int(object, text(norrath_actor_view_4837a8), 0);
        bytes(a)[570] = static_cast<unsigned char>(fresh_value);
    }
    if (norrath_game_object_find_tag_int(object, text(norrath_actor_view_4837b0), 0) != 0) {
        bytes(a)[570] = 3u;
        norrath_actor_remove_from_grid(object);
    }
    if (norrath_actor_find_tag_string(object, text(norrath_actor_view_460720), 0) != nullptr) {
        char* fresh_value = norrath_actor_find_tag_string(object, text(norrath_actor_view_460720), 0);
        const unsigned fresh_flags = word(a + 12u);
        put_word(a + 892u, address(fresh_value)); put_word(a + 12u, fresh_flags | 0x100000u);
    }
    if (norrath_game_object_find_tag_int(object, text(norrath_actor_view_4837b8), 0) != 0)
        put_word(a + 192u, 0x447a0000u);
    if (norrath_game_object_find_tag_int(object, text(norrath_actor_view_460730), 0) != 0) {
        const int fresh_value = norrath_game_object_find_tag_int(object, text(norrath_actor_view_460730), 0);
        const float converted = static_cast<float>(fresh_value);
        put_float(a + 788u, converted);
    }
    if (norrath_game_object_find_tag_int(object, text(norrath_actor_view_4837c0), 0) != 0) {
        const int fresh_value = norrath_game_object_find_tag_int(object, text(norrath_actor_view_4837c0), 0);
        const float factor = real_float(norrath_actor_view_47fb54);
        const float converted = static_cast<float>(fresh_value);
        const float scaled = converted * factor;
        put_float(a + 224u, scaled);
    }
    if (norrath_game_object_find_tag_int(object, text(norrath_actor_view_460740), 0) != 0)
        put_wide(a + 160u, wide(a + 160u) | 1ull);
    // The three genuine string calls short-circuit in original order.
    unsigned result;
    if (norrath_actor_find_tag_string(object, text(norrath_actor_view_4837c8), 0) != nullptr ||
        norrath_actor_find_tag_string(object, text(norrath_actor_view_4837d0), 0) != nullptr ||
        norrath_actor_find_tag_string(object, text(norrath_actor_view_4837d8), 0) != nullptr) {
        const Wide updated = wide(a + 160u) | 0x40000000ull;
        put_wide(a + 160u, updated);
        result = unsigned(updated);
    } else {
        result = 0u; // V0 is the final real null string answer on this path.
    }
    put_half(a + 798u, half(a + 56u));
    return result;
}

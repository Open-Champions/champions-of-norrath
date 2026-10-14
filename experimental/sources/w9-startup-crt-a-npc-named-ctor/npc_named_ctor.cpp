// Observed __3NPCG6Point3sPcN33ifPPc at 0x002313c8, 556 bytes: the NPC constructor that takes names. Arguments, in
// order: the object; the address of a Point3 (passed by value in the source); a signed short; four name strings (lump,
// model, texture, animation); an integer; a float; and, on the stack, the argument list. The 4-argument sibling is
// authored in ctors-lava-wraith-npc. In order:
//  1. The Point3 is copied to a local. The Creature constructor runs with it, the sign-extended short, a null resource
//     map, the single 8.0 and the argument list.
//  2. Word +72 receives the address of _vt$3NPC (0x0044adb0) and float +168 receives the single at 0x004819c8 (bit
//     pattern 0x3f333333, about 0.7). strcpy copies the lump name to +960, the model name to +992 and the texture
//     name to +1024, in that order. Halfword +1066 receives the integer (low 16 bits). Word +12 is ORed with
//     0x00100008. Five floats at +88..+107 (from +104 downwards) are set to 1.0. Byte +40 is 36, float +28 is 20.0 and
//     word +236 is zero.
//  3. lumpFindResource(lump, model) goes to word +1056 and lumpFindResource(lump, texture) to word +1060. Halfword
//     +572 is zero; word +12 is ORed with 0x00010000; float +168 is replaced by 0.5; float +176 receives the incoming
//     float; word +1136 is zero and that store happens before the animation state at +288 is initialised.
//  4. The animation resource lumpFindResource(lump, animation) is looked up; when it is non-null it is looked up again
//     and added to the animation state with flag 1, and the state's time is randomised.
//  5. The 64-bit modelGetActiveChunks(model at +1056, 0) is stored at +552 (this store happens before the run-list
//     call starts), and the object is added to the slow run list. When the tag string "showName" is not found on the
//     object, bit 0x00100000 of word +12 is cleared. The byte playedStinger (0x004857e0) is cleared before the final
//     call, finishConstruction__8Creature, whose result is returned.
// The unnamed integer and float arguments, and the meaning of the names beyond their use as resource keys, are not
// established here. The provider names are the ones other authored units already use; playedStinger is the
// executable's own symbol. Fresh compilation is not an original-byte match.
extern "C" {
unsigned norrath_creature_construct(void* object, const void* point, short type, void* resource_map, float scale,
                                    char** arguments);
char* strcpy(char* destination, const char* source);
void* norrath_lump_find_resource(char* lump_name, char* resource_name);
void norrath_actor_animation_initialize(void* state);
void norrath_actor_animation_add(void* state, void* header, int flags);
void norrath_actor_animation_randomize_time(void* state);
unsigned long long norrath_actor_model_active_chunks(void* model, int requested);
unsigned norrath_actor_add_slow_run_list(void* object);
char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
unsigned norrath_creature_finish_construction(void* object);
extern unsigned char vt_3NPC[] __asm__("_vt$3NPC");
extern unsigned char playedStinger;
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
void* pointer_from_word(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
}

extern "C" unsigned __3NPCG6Point3sPcN33ifPPc(void* object, const void* point, short type, char* lump, char* model,
                                              char* texture, char* animation, int number, float scale,
                                              char** arguments) {
    struct Point3Words {
        unsigned x, y, z;
    };
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned* words = static_cast<unsigned*>(object);
    float* floats = static_cast<float*>(object);
    const Point3Words local = *static_cast<const Point3Words*>(point);
    (void)norrath_creature_construct(object, &local, type, nullptr, 8.0f, arguments);
    words[18] = low32(vt_3NPC);
    words[42] = 0x3f333333u;
    strcpy(reinterpret_cast<char*>(bytes + 960), lump);
    strcpy(reinterpret_cast<char*>(bytes + 992), model);
    strcpy(reinterpret_cast<char*>(bytes + 1024), texture);
    *reinterpret_cast<unsigned short*>(bytes + 1066) = static_cast<unsigned short>(number);
    words[3] = words[3] | 0x00100008u;
    for (int index = 4; index >= 0; --index) floats[22 + index] = 1.0f;
    bytes[40] = 36;
    words[7] = 0x41a00000u;
    words[59] = 0u;
    words[264] = low32(norrath_lump_find_resource(reinterpret_cast<char*>(bytes + 960), reinterpret_cast<char*>(bytes + 992)));
    words[265] = low32(norrath_lump_find_resource(reinterpret_cast<char*>(bytes + 960), reinterpret_cast<char*>(bytes + 1024)));
    *reinterpret_cast<unsigned short*>(bytes + 572) = 0;
    words[3] = words[3] | 0x00010000u;
    words[42] = 0x3f000000u;
    floats[44] = scale;
    words[284] = 0u;
    norrath_actor_animation_initialize(bytes + 288);
    if (norrath_lump_find_resource(reinterpret_cast<char*>(bytes + 960), animation) != nullptr) {
        void* header = norrath_lump_find_resource(reinterpret_cast<char*>(bytes + 960), animation);
        norrath_actor_animation_add(bytes + 288, header, 1);
        norrath_actor_animation_randomize_time(bytes + 288);
    }
    const unsigned long long chunks = norrath_actor_model_active_chunks(pointer_from_word(words[264]), 0);
    words[138] = static_cast<unsigned>(chunks);
    words[139] = static_cast<unsigned>(chunks >> 32);
    (void)norrath_actor_add_slow_run_list(object);
    if (norrath_actor_find_tag_string(object, const_cast<char*>("showName"), 0) == nullptr)
        words[3] = words[3] & 0xffefffffu;
    playedStinger = 0;
    return norrath_creature_finish_construction(object);
}

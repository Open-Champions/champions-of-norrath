// Observed findTagFloat__10GameObjectPci at 0x1410a0, extent 324.
// Looks up a "name=value" tag on an object and returns the value as a float.
// The object's tag list is the word at object+44: a zero-terminated array of
// string pointers. In order:
//   1. A zero list word returns 0.0f before the name is touched.
//   2. The query name is lowercased in place: each byte whose ctype bit 0 is set
//      (the table norrath_factory_ctype_minimum_view, indexed by the signed byte
//      plus 128) has 32 added.
//   3. The name length is taken. A first list entry of zero returns 0.0f.
//   4. Each entry is compared with strncmp over that length; the list word is
//      re-read after the comparison. An entry that matches and whose byte at the
//      length is '=' counts as one hit; the caller's occurrence number is
//      decremented per hit and the hit that brings it to -1 is parsed with atof
//      and narrowed to float (dptofp). Other entries advance. Running off the end
//      of the list returns 0.0f.
// The same original behavior was already authored once for the player package as
// norrath_player_find_tag_float; this definition is under the original name and
// also supplies norrath_game_object_find_tag_float, the name the constructor
// sources import. It uses the same atof and double-narrowing providers.
// Unknown: the layout of the object beyond the list word.
extern "C" {
unsigned norrath_factory_strlen(const char* text);
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
extern const unsigned char norrath_factory_ctype_minimum_view[];
unsigned long long norrath_player_atof(const char* text);
float norrath_player_double_to_float(unsigned long long value);
}

struct TagObject {
    unsigned char before_tags[44];
    char** tags;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(TagObject, tags) == 44 && sizeof(char**) == 4, "observed tag list offset");
#endif

extern "C" float findTagFloat__10GameObjectPci(TagObject* object, char* name, int occurrence) {
    if (object->tags == nullptr) return 0.0f;
    for (unsigned char* cursor = reinterpret_cast<unsigned char*>(name); *cursor != 0; ++cursor) {
        const int character = static_cast<signed char>(*cursor);
        if ((norrath_factory_ctype_minimum_view[character + 128] & 1u) != 0) *cursor = static_cast<unsigned char>(character + 32);
    }
    const unsigned length = norrath_factory_strlen(name);
    if (object->tags[0] == nullptr) return 0.0f;
    unsigned index = 0;
    for (;;) {
        const char* entry = object->tags[index];
        const int compare = norrath_factory_strncmp(entry, name, length);
        entry = object->tags[index];
        ++index;
        if (compare == 0 && entry[length] == '=') {
            if (--occurrence == -1) return norrath_player_double_to_float(norrath_player_atof(entry + length + 1));
        }
        if (object->tags[index] == nullptr) return 0.0f;
    }
}

#if defined(__mips__)
extern "C" float norrath_game_object_find_tag_float(void* object, char* tag, int occurrence) __attribute__((alias("findTagFloat__10GameObjectPci")));
#endif

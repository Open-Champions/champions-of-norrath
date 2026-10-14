// Observed: ParseSpellEffectObj__10VIESFParse at 0x01179568 is 1,140 bytes. Argument: parse (a0,
// the ESF parser record: creation-argument word at +0, scene word at +24, dictionary word at +32,
// object file word at +36). The spell effect system is the sub-object at scene+7024, the particle
// system the one at scene+1352, and a flag word is read from the scene at +19356. In order it:
//   1. version = ObjectVersion__9VIObjFile(file) (a signed comparison below);
//      ReadBegin__9VIObjFileRUsT1(file, &tag, &second) (negative returns -1); reads a signed id;
//      ReadEnd__9VIObjFile (negative returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 21 returns -1, and type 21 calls ReadEnd
//      (ignored) and returns the found handle;
//   3. when version is positive it reads a header: a tag of 0xc230 means a nested-resource block
//      (when the scene flag word is non-zero: n = NumSubObjects__9VIObjFile(file) and n calls of
//      ParseObject__10VIESFParse(parse), negative returning -1) closed by ReadEnd (negative
//      returning -1); any other tag leaves that header as the one already consumed;
//   4. handle = CreateSpellEffect__19VISpellEffectSystemi(system, parse[0]) (exactly -1 returns -1),
//      effect = SpellEffect__19VISpellEffectSystemi(system, handle) (null returns -1),
//      Create__13VISpellEffectP7VIScene(effect, scene) (negative returns -1) and, for a non-zero id,
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, effect, id, 21, handle) (negative
//      returns -1);
//   5. unless step 3 consumed a header already, it reads the next header (negative returns -1);
//   6. three event lists at effect+12, +24 and +36 are read in that order: a signed count, then
//      Create__16VISpellEventListiP7VIScene(list, count, scene). Each list holds 44-byte records
//      (the array pointer is the list word at +8): an unsigned type, four signed ints (the
//      third of them at +8 is a resource reference), and six floats. For version below 2 a type-1
//      record gets 5000 in its +12 field. Types 3 and 8 refer to a particle definition, types 9 and
//      11 to a sprite; a reference is resolved only when the scene flag word is non-zero (else the
//      field becomes -1): for types 3/8 a field of -1 or 0 becomes -1, otherwise
//      Find(dict, field, &type, &handle) must hit (a miss returns -1), the handle replaces the
//      field and ShareParticleDefinition__16VIParticleSystemi(particle system, handle) runs; for
//      types 9/11 Find must hit, the handle replaces the field and ShareSprite__7VIScenei(scene,
//      handle) runs. The handle variable of step 4 is reused by those lookups, so after any
//      successful lookup the final return value is that lookup's handle, not the effect's;
//   7. finally ReadEnd (negative returns -1), a second ReadEnd (negative returns -1) and the handle.
// Inferred: resource type 21 means "spell effect"; 0xc230 is the optional nested resource block;
// the three lists are spell event lists (their purposes and record field names are unknown).
// Unknown: field names, record layout beyond the stored sizes, and the quirk that the final handle
// may come from an event lookup (reproduced as observed).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int NumSubObjects__9VIObjFile(void* file);
unsigned ObjectVersion__9VIObjFile(void* file);
int ParseObject__10VIESFParse(void* parse);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int CreateSpellEffect__19VISpellEffectSystemi(void* system, int argument);
void* SpellEffect__19VISpellEffectSystemi(void* system, int handle);
int Create__13VISpellEffectP7VIScene(void* effect, void* scene);
void Create__16VISpellEventListiP7VIScene(void* list, int count, void* scene);
void ShareParticleDefinition__16VIParticleSystemi(void* system, int definition);
void ShareSprite__7VIScenei(void* scene, int sprite);

int ParseSpellEffectObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    unsigned char* scene = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[6]));
    void* system = NORRATH_WORD_ADDRESS(words[6] + 7024);
    void* particles = NORRATH_WORD_ADDRESS(words[6] + 1352);
    unsigned short tag;
    unsigned short second;
    int id;
    int type;
    int handle;
    int version = static_cast<int>(ObjectVersion__9VIObjFile(file));
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    Read__9VIObjFileRi(file, &id);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, static_cast<unsigned>(id), &type, &handle) != 0) {
        if (type != 21) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    int header_consumed = 0;
    if (version > 0) {
        if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
        if (tag == 0xc230) {
            if (*reinterpret_cast<unsigned*>(scene + 19356) != 0) {
                int count = NumSubObjects__9VIObjFile(file);
                for (int i = 0; i < count; ++i) {
                    if (ParseObject__10VIESFParse(parse) < 0) return -1;
                }
            }
            if (ReadEnd__9VIObjFile(file) < 0) return -1;
        } else {
            header_consumed = 1;
        }
    }
    handle = CreateSpellEffect__19VISpellEffectSystemi(system, static_cast<int>(words[0]));
    if (handle == -1) return -1;
    unsigned char* effect = static_cast<unsigned char*>(SpellEffect__19VISpellEffectSystemi(system, handle));
    if (effect == 0) return -1;
    if (Create__13VISpellEffectP7VIScene(effect, scene) < 0) return -1;
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, effect, static_cast<unsigned>(id), 21, handle) < 0) return -1;
    if (!header_consumed) {
        if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    }
    for (int index = 0; index < 3; ++index) {
        unsigned char* list = effect + 12 + 12 * index;
        int count;
        Read__9VIObjFileRi(file, &count);
        Create__16VISpellEventListiP7VIScene(list, count, scene);
        for (int event = 0; event < count; ++event) {
            unsigned char* record = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(list + 8))) + 44 * event;
            Read__9VIObjFileRUi(file, reinterpret_cast<unsigned*>(record));
            Read__9VIObjFileRi(file, reinterpret_cast<int*>(record + 4));
            Read__9VIObjFileRi(file, reinterpret_cast<int*>(record + 8));
            Read__9VIObjFileRi(file, reinterpret_cast<int*>(record + 12));
            Read__9VIObjFileRi(file, reinterpret_cast<int*>(record + 16));
            for (int i = 0; i < 6; ++i) Read__9VIObjFileRf(file, reinterpret_cast<float*>(record + 20 + 4 * i));
            if (version < 2 && *reinterpret_cast<int*>(record) == 1) *reinterpret_cast<int*>(record + 12) = 5000;
            int kind = *reinterpret_cast<int*>(record);
            if (kind == 3 || kind == 8) {
                if (*reinterpret_cast<unsigned*>(scene + 19356) == 0) {
                    *reinterpret_cast<int*>(record + 8) = -1;
                } else {
                    int reference = *reinterpret_cast<int*>(record + 8);
                    if (static_cast<unsigned>(reference + 1) < 2) {
                        *reinterpret_cast<int*>(record + 8) = -1;
                    } else {
                        if (Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, static_cast<unsigned>(reference), &type, &handle) == 0) return -1;
                        *reinterpret_cast<int*>(record + 8) = handle;
                        ShareParticleDefinition__16VIParticleSystemi(particles, handle);
                    }
                }
            } else if (kind == 9 || kind == 11) {
                if (*reinterpret_cast<unsigned*>(scene + 19356) == 0) {
                    *reinterpret_cast<int*>(record + 8) = -1;
                } else {
                    if (Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, *reinterpret_cast<unsigned*>(record + 8), &type, &handle) == 0) return -1;
                    *reinterpret_cast<int*>(record + 8) = handle;
                    ShareSprite__7VIScenei(scene, handle);
                }
            }
        }
    }
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return handle;
}
}

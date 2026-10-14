// Observed: ParseParticleDefinitionObj__10VIESFParse at 0x01178f08 is 480 bytes. Argument: parse
// (a0, the ESF parser record: creation-argument word at +0, a flag word at +12, scene word at +24,
// dictionary word at +32, object file word at +36). The particle system is the sub-object at
// scene+1352; a second flag word is read from the scene at +19356. In order it:
//   1. returns -1 when the system address (scene word + 1352) is zero; when the scene word at
//      +19356 is zero it only calls ReadEnd__9VIObjFile and returns -1 if that is negative, else 0;
//   2. ReadBegin__9VIObjFileRUsT1(file, &tag, &second) (negative returns -1), reads a signed id,
//      ReadEnd (negative returns -1);
//   3. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 18 returns -1, and type 18 calls ReadEnd
//      (ignored) and returns the found handle;
//   4. otherwise it calls CreateParticleDefinition__16VIParticleSystemi(system, parse[0]) (negative
//      returns -1) and ParticleDefinition__16VIParticleSystemi(system, handle) for the definition
//      record; for a non-zero id it registers the definition (record + 764, or null when the
//      record is null) with Add__12VIDictionaryR10VIResourceUi14VIResourceTypei (dict, resource,
//      id, 18, handle), negative returning -1;
//   5. it reads the next header: for tag 0x1000 with the flag word at +12 non-zero it calls
//      ParseSurfaceObj__10VIESFParse(parse) (negative returns -1, the result is not used further);
//      with the flag zero it reads and closes one more header (ReadBegin then ReadEnd, results
//      ignored); any other tag goes straight to the next step;
//   6. for the surface cases it reads one more header (negative returns -1), then
//      ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition(parse, file,
//      record) (negative returns -1), ReadEnd, and the created handle (-1 when ReadEnd is
//      negative).
// Inferred: resource type 18 means particle definition; the optional surface sub-object is the
// definition's texture, skipped when the flag at +12 is zero; the record + 764 is the embedded
// resource header. Unknown: what the two flag words mean, and the size of the record.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateParticleDefinition__16VIParticleSystemi(void* system, int argument);
unsigned char* ParticleDefinition__16VIParticleSystemi(void* system, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int ParseSurfaceObj__10VIESFParse(void* parse);
int ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition(void* parse, void* file, void* definition);

int ParseParticleDefinitionObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    unsigned char* scene = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[6]));
    void* system = NORRATH_WORD_ADDRESS(words[6] + 1352);
    unsigned short tag;
    unsigned short second;
    int id;
    int type;
    int handle;
    if (words[6] + 1352 == 0) return -1;
    if (*reinterpret_cast<unsigned*>(scene + 19356) == 0) return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    Read__9VIObjFileRi(file, &id);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, static_cast<unsigned>(id), &type, &handle) != 0) {
        if (type != 18) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    int created = CreateParticleDefinition__16VIParticleSystemi(system, static_cast<int>(words[0]));
    if (created < 0) return -1;
    unsigned char* definition = ParticleDefinition__16VIParticleSystemi(system, created);
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, definition != 0 ? definition + 764 : 0, static_cast<unsigned>(id), 18, created) < 0) return -1;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag == 0x1000) {
        if (words[3] != 0) {
            if (ParseSurfaceObj__10VIESFParse(parse) < 0) return -1;
        } else {
            ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
            ReadEnd__9VIObjFile(file);
        }
        if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    }
    if (ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition(parse, file, definition) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : created;
}
}

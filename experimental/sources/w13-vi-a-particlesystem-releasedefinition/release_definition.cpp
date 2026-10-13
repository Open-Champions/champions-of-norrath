// Observed: ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary at 0x010b8020 is 308
// bytes. Arguments: system (a0), definition index id (a1) and dictionary (a2). The system holds a
// pool at +84 whose item array is the word at +92; items are 12 bytes and the first word of item
// id is the definition pointer def.
//   1. count = def[764 + 4] - 1 is stored back; if count > 0 it returns 0.
//   2. Otherwise: if the word def[764] is non-zero it calls Remove__12VIDictionaryR10VIResource(
//      dictionary, def + 764) (a null def would give a null resource).
//   3. If the int def[776] is not -1: raster = the word at system+0; it calls
//      SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(Material__8VIRasteri(raster,
//      def[776]), 0, -1, raster, dictionary) and then ReleaseMaterial__8VIRasteriR12VIDictionary(
//      raster, def[776], dictionary).
//   4. While FirstMotif__C22VIParticleDefinitionEx(def) is non-null it calls
//      DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif(def, motif).
//   5. If def is non-null it calls _$_22VIParticleDefinitionEx(def, 0) and __builtin_delete(def).
//   6. It calls Erase__t6VIPool1ZP20VIParticleDefinitioni(system + 84, id) and returns 0 when
//      that is non-negative, else -1.
// Inferred: a reference-counted particle definition (count at +768, a dictionary registration
// at +764, a material id at +776) owned by a pool; the last release unregisters it, frees its
// material layer 0 texture and material, deletes its motifs and itself and removes the pool slot.
// Unknown: the exact layout of the definition, what the word at def+764 is (a registered flag or
// a dictionary key), and why the destructor is called with flag 0 and then delete is called
// explicitly.
extern "C" {
int Remove__12VIDictionaryR10VIResource(void* dictionary, void* resource);
void* Material__8VIRasteri(void* raster, int material);
int SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void* material, int layer, int texture, void* raster, void* dictionary);
int ReleaseMaterial__8VIRasteriR12VIDictionary(void* raster, int material, void* dictionary);
void* FirstMotif__C22VIParticleDefinitionEx(const void* definition);
void DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif(void* definition, void* motif);
void _$_22VIParticleDefinitionEx(void* definition, int flags);
void __builtin_delete(void* pointer);
int Erase__t6VIPool1ZP20VIParticleDefinitioni(void* pool, int index);
}

// The words at system+0, system+92 and the first word of each pool item are 32-bit target
// addresses. A host check defines NORRATH_HOST_ARENA, so that they become arena offsets.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* pointerAt(const unsigned char* p) {
    unsigned word = *reinterpret_cast<const unsigned*>(p);
    return word == 0 ? nullptr : norrath_host_pointer(word);
}
#else
static inline unsigned char* pointerAt(const unsigned char* p) {
    return *reinterpret_cast<unsigned char* const*>(p);
}
#endif

extern "C" {
int ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(unsigned char* system, int id, void* dictionary) {
    unsigned char* pool = system + 84;
    unsigned char* def = pointerAt(pointerAt(pool + 8) + id * 12);
    unsigned char* refs = def + 764;
    int count = *reinterpret_cast<int*>(refs + 4) - 1;
    *reinterpret_cast<int*>(refs + 4) = count;
    if (count > 0) {
        return 0;
    }
    if (*reinterpret_cast<int*>(refs) != 0) {
        __asm__("" : "+r"(def));   // the image tests def for null after reading through it; keep the test
        Remove__12VIDictionaryR10VIResource(dictionary, def != 0 ? refs : 0);
    }
    if (*reinterpret_cast<int*>(def + 776) != -1) {
        void* raster = pointerAt(system);
        SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(Material__8VIRasteri(raster, *reinterpret_cast<int*>(def + 776)), 0, -1, raster, dictionary);
        ReleaseMaterial__8VIRasteriR12VIDictionary(raster, *reinterpret_cast<int*>(def + 776), dictionary);
    }
    void* motif;
    while ((motif = FirstMotif__C22VIParticleDefinitionEx(def)) != 0) {
        DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif(def, motif);
    }
    __asm__("" : "+r"(def));
    if (def != 0) {
        _$_22VIParticleDefinitionEx(def, 0);
        __builtin_delete(def);
    }
    return Erase__t6VIPool1ZP20VIParticleDefinitioni(pool, id) >= 0 ? 0 : -1;
}
}

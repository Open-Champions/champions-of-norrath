// Observed: Clear__12VIAtmosphereR7VIScene at 0x010672a0 is 380 bytes. It reads
// three words from the scene before anything else: the sound device at 19360, the
// raster at 19356 and the dictionary at 19352. If the word at offset 1228 of this
// is nonzero it then: for the words at 4496 and 4500 in turn, when nonzero calls
// Stop__13VISoundDevicei(device, word) and stores -1; calls
// ReleaseMaterial__8VIRasteriR12VIDictionary(raster, word, dictionary) for the
// words at 3920, 1244 and 56 and for the two words at 160 and 164. Then, whether
// or not 1228 was set: a nonzero word at 12 goes to __builtin_vec_delete and is
// zeroed; ClearAllFireballs__12VIAtmosphere(this) is called; a nonzero word at
// 4476 is an array of 2048-byte elements whose count is the word at base-16: each
// element from the last to the first has _$_17VIParticleEmitter called with
// flags 2 on element+1056 and then on element+64, and __builtin_vec_delete is
// called on base-16 before the word is zeroed; ClearAllSplashes__12VIAtmosphere(this)
// is called; a nonzero word at 48 goes to __builtin_vec_delete and is zeroed. The
// word at 1228 is zeroed last. No value is returned.
// Inferred: the count word before the array is the compiler's vector-new cookie.
extern "C" {
void Stop__13VISoundDevicei(void* device, int id);
void ReleaseMaterial__8VIRasteriR12VIDictionary(void* raster, int material, void* dictionary);
void ClearAllFireballs__12VIAtmosphere(void* object);
void ClearAllSplashes__12VIAtmosphere(void* object);
void _$_17VIParticleEmitter(void* emitter, int flags);
void __builtin_vec_delete(void* pointer);

static unsigned char* at(unsigned int word) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
static unsigned int& word_at(unsigned char* base, int offset) {
    return *reinterpret_cast<unsigned int*>(base + offset);
}

void Clear__12VIAtmosphereR7VIScene(void* object, void* scene) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned char* scene_bytes = static_cast<unsigned char*>(scene);
    unsigned char* device = at(word_at(scene_bytes, 19360));
    unsigned char* raster = at(word_at(scene_bytes, 19356));
    unsigned char* dictionary = at(word_at(scene_bytes, 19352));
    if (word_at(bytes, 1228) != 0) {
        if (word_at(bytes, 4496) != 0) {
            Stop__13VISoundDevicei(device, static_cast<int>(word_at(bytes, 4496)));
            word_at(bytes, 4496) = 0xffffffffu;
        }
        if (word_at(bytes, 4500) != 0) {
            Stop__13VISoundDevicei(device, static_cast<int>(word_at(bytes, 4500)));
            word_at(bytes, 4500) = 0xffffffffu;
        }
        ReleaseMaterial__8VIRasteriR12VIDictionary(raster, static_cast<int>(word_at(bytes, 3920)), dictionary);
        ReleaseMaterial__8VIRasteriR12VIDictionary(raster, static_cast<int>(word_at(bytes, 1244)), dictionary);
        ReleaseMaterial__8VIRasteriR12VIDictionary(raster, static_cast<int>(word_at(bytes, 56)), dictionary);
        ReleaseMaterial__8VIRasteriR12VIDictionary(raster, static_cast<int>(word_at(bytes, 160)), dictionary);
        ReleaseMaterial__8VIRasteriR12VIDictionary(raster, static_cast<int>(word_at(bytes, 164)), dictionary);
    }
    if (word_at(bytes, 12) != 0) {
        __builtin_vec_delete(at(word_at(bytes, 12)));
        word_at(bytes, 12) = 0;
    }
    ClearAllFireballs__12VIAtmosphere(object);
    if (word_at(bytes, 4476) != 0) {
        unsigned char* base = at(word_at(bytes, 4476));
        unsigned char* element = base + (word_at(base, -16) << 11);
        if (base != element) {
            do {
                element -= 2048;
                _$_17VIParticleEmitter(element + 1056, 2);
                _$_17VIParticleEmitter(element + 64, 2);
            } while (at(word_at(bytes, 4476)) != element);
        }
        __builtin_vec_delete(base - 16);
        word_at(bytes, 4476) = 0;
    }
    ClearAllSplashes__12VIAtmosphere(object);
    if (word_at(bytes, 48) != 0) {
        __builtin_vec_delete(at(word_at(bytes, 48)));
        word_at(bytes, 48) = 0;
    }
    word_at(bytes, 1228) = 0;
}
}

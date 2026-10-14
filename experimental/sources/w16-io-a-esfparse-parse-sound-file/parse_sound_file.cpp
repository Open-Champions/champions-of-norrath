// Observed: ParseSound__10VIESFParsePCcR7VIScene at 0x0116dc38, 272 bytes. Arguments: parse (the ESF parser record), a file name
// and the scene. It keeps a VIObjFile on its stack (the frame reserves 592 bytes for it below the saved registers) and in order:
//   1. constructs it with __9VIObjFile(object);
//   2. fills the parser: scene word at +24 = scene; +32, +12, +20 and +16 take the scene words at +19352, +19356, +19364 and
//      +19360; the word at +28 is cleared; the object file word at +36 is set to the address of the stack object;
//   3. Open__9VIObjFilePCcQ29VIObjFile8FileMode(object, name, 0);
//   4. when that is not negative, FileType__9VIObjFile(object) is 0xab4f and NumObjects__9VIObjFile(object) is 1, it calls
//      ParseSound__10VIESFParse(parse); a non-negative result is kept and Close__9VIObjFile(object) is called; when Close is not
//      negative the words at +36, +24, +32, +12, +16, +20 and +28 are cleared, the destructor _$_9VIObjFile(object, 2) runs and the
//      kept result is returned;
//   5. every other path (open, type, count, parse or close failure) runs the destructor with flags 2 and returns -1, leaving the
//      parser words set as in step 2 (including the +36 word pointing at the destroyed stack object).
// Inferred: this loads a one-object sound file (type 0xab4f) from disk and parses it as a sound resource, returning the resource
// handle; the parser words are a per-parse context that is reset after a successful parse.
// Unknown: the member names, the meaning of the scene words at +19352..+19364 (they are copied unchanged), the size of VIObjFile
// (taken from the stack space reserved) and the FileMode value 0.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
static inline unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
extern "C" {
void __9VIObjFile(void* object);
void _$_9VIObjFile(void* object, int flags);
int Open__9VIObjFilePCcQ29VIObjFile8FileMode(void* object, const char* name, int mode);
int FileType__9VIObjFile(void* object);
int NumObjects__9VIObjFile(void* object);
int Close__9VIObjFile(void* object);
int ParseSound__10VIESFParse(void* parse);

int ParseSound__10VIESFParsePCcR7VIScene(unsigned char* parse, const char* name, unsigned char* scene) {
    alignas(8) unsigned char object[592];
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    const unsigned* sceneWords = reinterpret_cast<const unsigned*>(scene);
    __9VIObjFile(object);
    words[6] = low32(scene);
    unsigned a = sceneWords[19352 / 4];
    unsigned b = sceneWords[19356 / 4];
    unsigned c = sceneWords[19364 / 4];
    unsigned d = sceneWords[19360 / 4];
    words[8] = a;
    words[3] = b;
    words[5] = c;
    words[4] = d;
    words[7] = 0;
    words[9] = low32(object);
    if (Open__9VIObjFilePCcQ29VIObjFile8FileMode(object, name, 0) >= 0 && FileType__9VIObjFile(object) == 0xab4f &&
        NumObjects__9VIObjFile(object) == 1) {
        int result = ParseSound__10VIESFParse(parse);
        if (result >= 0 && Close__9VIObjFile(object) >= 0) {
            words[9] = 0;
            words[6] = 0;
            words[8] = 0;
            words[3] = 0;
            words[4] = 0;
            words[5] = 0;
            words[7] = 0;
            _$_9VIObjFile(object, 2);
            return result;
        }
    }
    _$_9VIObjFile(object, 2);
    return -1;
}
}

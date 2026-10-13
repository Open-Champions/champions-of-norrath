// Observed: Parse__10VIESFParseR9VIObjFileR7VIScene at 0x0116d960 is 212 bytes.
// Arguments: the parser (a0), an object file (a1) and a scene (a2). In order it:
//   - stores the scene pointer at parser+24;
//   - reads four words of the scene at +19364, +19360, +19352, +19356 (in that order) and
//     stores them at parser+20, +32, +12 and +16 as follows: scene+19364 -> +20,
//     scene+19352 -> +32, scene+19356 -> +12, scene+19360 -> +16;
//   - stores 0 at parser+28 and the file pointer at parser+36, then calls
//     FileType__9VIObjFile(file);
//   - if that result is not 0xab4f it returns -1, leaving all of those fields set;
//   - otherwise it calls NumObjects__9VIObjFile(file) and, when the count is positive,
//     calls ParseObject__10VIESFParse(parser) that many times (at least once), returning
//     -1 immediately, fields still set, if a call returns a negative value;
//   - after the loop (or at once if the count is not positive) it clears parser+36, then
//     +24, +32, +12, +16, +20 and +28 to zero and returns 0.
// Inferred: the parser keeps a working view of the scene (four words at +12..+20 and +32
// taken from the scene) and its current file at +36, which are only valid during a parse.
// 0xab4f is the file-type value that identifies an ESF file. Unknown: the member names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned norrath_host_address(const unsigned char* pointer);
#define NORRATH_POINTER_TO_WORD(pointer) norrath_host_address(pointer)
#else
#define NORRATH_POINTER_TO_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif

extern "C" {
int FileType__9VIObjFile(void* file);
int NumObjects__9VIObjFile(void* file);
int ParseObject__10VIESFParse(void* parser);

int Parse__10VIESFParseR9VIObjFileR7VIScene(unsigned char* parser, void* file, unsigned char* scene) {
    unsigned* words = reinterpret_cast<unsigned*>(parser);
    const unsigned* scene_words = reinterpret_cast<const unsigned*>(scene);
    words[6] = NORRATH_POINTER_TO_WORD(scene);
    unsigned a = scene_words[19364 / 4];
    unsigned b = scene_words[19360 / 4];
    unsigned c = scene_words[19352 / 4];
    unsigned d = scene_words[19356 / 4];
    words[5] = a;
    words[8] = c;
    words[3] = d;
    words[4] = b;
    words[7] = 0;
    words[9] = NORRATH_POINTER_TO_WORD(static_cast<unsigned char*>(file));
    if (FileType__9VIObjFile(file) != 0xab4f) return -1;
    int count = NumObjects__9VIObjFile(file);
    if (count > 0) {
        int index = 0;
        do {
            if (ParseObject__10VIESFParse(parser) < 0) return -1;
            ++index;
        } while (index < count);
    }
    words[9] = 0;
    words[6] = 0;
    words[8] = 0;
    words[3] = 0;
    words[4] = 0;
    words[5] = 0;
    words[7] = 0;
    return 0;
}
}

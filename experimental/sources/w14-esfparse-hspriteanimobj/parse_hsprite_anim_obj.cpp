// Observed: ParseHSpriteAnimObj__10VIESFParse at 0x01172f60 is 1,856 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, progress-callback word at +4 and its argument
// word at +8, scene word at +24, dictionary word at +32, object file word at +36). In order it:
//   1. calls the callback word with the argument word when it is non-zero;
//   2. reads an unsigned id; with v = ObjectVersion__9VIObjFile(file): when v >= 2 a signed format
//      value (otherwise format 0); then a node count and a frame count; when v >= 3 a key count
//      (otherwise 0); a float (rate); when v is non-zero a second float (scale, otherwise 1.0) and
//      a signed playback value (otherwise 0);
//   3. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 6 returns -1, and type 6 calls ReadEnd__9VIObjFile
//      (ignored) and returns the found handle;
//   4. otherwise CreateAnimation__7VIScenei(scene, parse[0]) (negative returns -1),
//      Animation__7VIScenei(scene, handle) and, for a non-zero id,
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, animation, id, 6, handle)
//      (negative returns -1); then
//      Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType(animation, format, nodes,
//      frames, keys, rate, scale, playback) (negative returns -1);
//   5. the per-node data depends on the format and on the word at animation+8: format 0 requires
//      that word to be 0; format 1 accepts 0 or 1; anything else returns -1. For each node a signed
//      reference id is read and passed to SetNodeRefID__13VIHSpriteAnimii(animation, node, id)
//      (negative returns -1), then, for each frame, eight values are read and the frame is set
//      with SetNodeFrame__13VIHSpriteAnimii (animation, node, frame, record), negative returning -1:
//        - format 0: eight floats into a 32-byte record (R14VIHSpriteFrame);
//        - format 1 with word 1: eight signed 16-bit values into a 16-byte record
//          (R18VIHSpritePackFrame);
//        - format 1 with word 0: eight signed 16-bit values converted to floats, the first four
//          multiplied by the float with bit pattern 0x38000100 (2^-15 * (1 + 2^-15), written as a hex literal) and the last four by 1/512,
//          passed as a 32-byte float record (R14VIHSpriteFrame);
//      nothing is read for a node when the frame count is not positive, and no node is visited
//      when the node count is not positive;
//   6. for each of the key count keys: a signed key index and a float, then
//      SetKeyframe__13VIHSpriteAnimiif(animation, key number, key index, value) (result not
//      examined); finally ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: resource type 6 means "hierarchical sprite animation"; the 16-bit format is a
// compressed rotation (scaled to about +-1) plus translation (1/512 units); the key records
// attach a float to a frame index. Unknown: the member names, the meaning of the word at
// animation+8 (it selects between the two format-1 encodings), the enumerators of the format and
// playback arguments, and what SetKeyframe returns.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
typedef void (*ProgressCallback)(void* argument);

struct AnimFrame { float v[8]; };
struct AnimPackFrame { short v[8]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRs(void* file, short* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadEnd__9VIObjFile(void* file);
unsigned ObjectVersion__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateAnimation__7VIScenei(void* scene, int argument);
unsigned char* Animation__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType(void* animation, int format, int nodes, int frames, int keys, float rate, float scale, int playback);
int SetNodeRefID__13VIHSpriteAnimii(void* animation, int node, int id);
int SetNodeFrame__13VIHSpriteAnimiiR14VIHSpriteFrame(void* animation, int node, int frame, const AnimFrame* record);
int SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame(void* animation, int node, int frame, const AnimPackFrame* record);
void SetKeyframe__13VIHSpriteAnimiif(void* animation, int key, int index, float value);

int ParseHSpriteAnimObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    unsigned id;
    int format = 0;
    int nodes;
    int frames;
    int keys;
    float rate;
    float scale;
    int playback;
    int type;
    int handle;
    if (words[1] != 0) reinterpret_cast<ProgressCallback>(NORRATH_WORD_ADDRESS(words[1]))(NORRATH_WORD_ADDRESS(words[2]));
    Read__9VIObjFileRUi(file, &id);
    if (ObjectVersion__9VIObjFile(file) >= 2) Read__9VIObjFileRi(file, &format);
    Read__9VIObjFileRi(file, &nodes);
    Read__9VIObjFileRi(file, &frames);
    if (ObjectVersion__9VIObjFile(file) >= 3) Read__9VIObjFileRi(file, &keys);
    else keys = 0;
    Read__9VIObjFileRf(file, &rate);
    if (ObjectVersion__9VIObjFile(file) != 0) {
        Read__9VIObjFileRf(file, &scale);
        Read__9VIObjFileRi(file, &playback);
    } else {
        playback = 0;
        scale = 1.0f;
    }
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 6) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateAnimation__7VIScenei(scene, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* animation = Animation__7VIScenei(scene, handle);
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, animation, id, 6, handle) < 0) return -1;
    if (Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType(animation, format, nodes, frames, keys, rate, scale, playback) < 0) return -1;
    int encoding = *reinterpret_cast<int*>(animation + 8);
    if (format == 0) {
        if (encoding != 0) return -1;
    } else if (format == 1) {
        if (encoding != 0 && encoding != 1) return -1;
    } else {
        return -1;
    }
    for (int node = 0; node < nodes; ++node) {
        int ref;
        Read__9VIObjFileRi(file, &ref);
        if (SetNodeRefID__13VIHSpriteAnimii(animation, node, ref) < 0) return -1;
        for (int frame = 0; frame < frames; ++frame) {
            if (format == 0) {
                AnimFrame record;
                for (int i = 0; i < 8; ++i) Read__9VIObjFileRf(file, &record.v[i]);
                if (SetNodeFrame__13VIHSpriteAnimiiR14VIHSpriteFrame(animation, node, frame, &record) < 0) return -1;
            } else if (encoding == 1) {
                AnimPackFrame record;
                for (int i = 0; i < 8; ++i) Read__9VIObjFileRs(file, &record.v[i]);
                if (SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame(animation, node, frame, &record) < 0) return -1;
            } else {
                short packed[8];
                AnimFrame record;
                for (int i = 0; i < 8; ++i) Read__9VIObjFileRs(file, &packed[i]);
                for (int i = 0; i < 4; ++i) record.v[i] = static_cast<float>(packed[i]) * 0x1.0002p-15f;
                for (int i = 4; i < 8; ++i) record.v[i] = static_cast<float>(packed[i]) * 0.001953125f;
                if (SetNodeFrame__13VIHSpriteAnimiiR14VIHSpriteFrame(animation, node, frame, &record) < 0) return -1;
            }
        }
    }
    for (int key = 0; key < keys; ++key) {
        int index;
        float value;
        Read__9VIObjFileRi(file, &index);
        Read__9VIObjFileRf(file, &value);
        SetKeyframe__13VIHSpriteAnimiif(animation, key, index, value);
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}

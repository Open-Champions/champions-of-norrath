// Observed: ParseHSpriteAttachments__10VIESFParseR9VIHSprite at 0x01172ce8 is 364 bytes. Arguments:
// parse (a0; scene word at +24, dictionary word at +32, object file word at +36) and hsprite (a1,
// a hierarchical sprite). In order it:
//   1. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x2500, returns -1;
//   2. reads a signed count (Read__9VIObjFileRi); then for each index below the count (the count
//      local is read again after every record) reads a record of a signed kind (Read__9VIObjFileRi),
//      an unsigned id (Read__9VIObjFileRUi) and a signed node (Read__9VIObjFileRi), in that order;
//   3. a kind of 0 selects resource type value 4, a kind of 1 selects 7, any other kind returns -1
//      (after all three values were read);
//   4. a record with a zero id does nothing more; otherwise the id is looked up with
//      FindTyped__12VIDictionaryUi14VIResourceTypeRi(dict, id, type, &handle) (a zero result
//      returns -1), and then, when the node is -1, AttachSkin__9VIHSpriteiR7VIScene(hsprite,
//      handle, scene), else Attach__9VIHSpriteiiR7VIScene(hsprite, handle, node, scene); a
//      negative result returns -1;
//   5. returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: kind 0 attaches a resource of type 4 and kind 1 a resource of type 7 to a node of the
// hierarchy; the node value -1 means "the skin" rather than a numbered node (the callee names).
// Unknown: which resource types 4 and 7 are, the name of the kind field, and the second ReadBegin
// output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* handle);
int AttachSkin__9VIHSpriteiR7VIScene(void* hsprite, int handle, void* scene);
int Attach__9VIHSpriteiiR7VIScene(void* hsprite, int handle, int node, void* scene);

int ParseHSpriteAttachments__10VIESFParseR9VIHSprite(unsigned char* parse, void* hsprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    unsigned short tag;
    unsigned short second;
    int count;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2500) return -1;
    Read__9VIObjFileRi(file, &count);
    for (int i = 0; i < count; ++i) {
        int kind;
        unsigned id;
        int node;
        int handle;
        int type = 7;
        Read__9VIObjFileRi(file, &kind);
        Read__9VIObjFileRUi(file, &id);
        Read__9VIObjFileRi(file, &node);
        if (kind == 0) type = 4;
        else if (kind != 1) return -1;
        if (id != 0) {
            if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id, type, &handle) == 0) return -1;
            int result;
            if (node == -1) result = AttachSkin__9VIHSpriteiR7VIScene(hsprite, handle, scene);
            else result = Attach__9VIHSpriteiiR7VIScene(hsprite, handle, node, scene);
            if (result < 0) return -1;
        }
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}

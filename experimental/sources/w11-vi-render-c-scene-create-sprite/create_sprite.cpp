// Observed CreateSprite__7VIScene12VISpriteTypei at 0x0113ca98, 688 bytes. Arguments: scene, a sprite
// type, and an integer `addRef`. The type minus one is used as an unsigned index into a fourteen-entry
// jump table (so types 1 through 14 are handled; any other type leaves the sprite null). For a handled
// type, __builtin_new(size) is called and, if it returned nonzero, the matching constructor runs on it:
//   type  1: 76 bytes   __14VISimpleSprite        type  8: 80 bytes   __12VIPointLight
//   type  2: 68 bytes   __13VIPointSprite         type  9: 88 bytes   __13VISoundSprite
//   type  3: 76 bytes   __12VISkinSprite          type 10: 1088 bytes __16VIParticleSprite
//   type  4: 400 bytes  __9VIHSprite              type 11: 228 bytes  __13VIGroupSprite
//   type  5: 4624 bytes __9VICSprite              type 12: 200 bytes  __19VIStreamAudioSprite
//   type  6: 116 bytes  __12VINameSprite          type 13: 72 bytes   __13VIFloraSprite
//   type  7: 112 bytes  __11VILODSprite           type 14: 192 bytes  __20VIEffectVolumeSprite
// A null sprite (unhandled type or failed allocation) gives -1. Otherwise, when addRef is nonzero the
// word at sprite + 4 is incremented, and the result is that of
// Add__t6VIPool1ZP8VISpriteRCP8VISprite(scene + 996, &sprite), which appends the sprite pointer to the
// scene's sprite pool. The mapping from jump-table entries to constructors was read from the table
// itself (entries 12 and 13 are not in address order).
// Inferred: the word at sprite + 4 is a count (it is incremented). Unknown: the layout of every sprite.
extern "C" {
struct VIScene;
struct VISprite;

void* __builtin_new(unsigned size);
int Add__t6VIPool1ZP8VISpriteRCP8VISprite(void* pool, VISprite* const* sprite);
void __14VISimpleSprite(void* sprite);
void __13VIPointSprite(void* sprite);
void __12VISkinSprite(void* sprite);
void __9VIHSprite(void* sprite);
void __9VICSprite(void* sprite);
void __12VINameSprite(void* sprite);
void __11VILODSprite(void* sprite);
void __12VIPointLight(void* sprite);
void __13VISoundSprite(void* sprite);
void __16VIParticleSprite(void* sprite);
void __13VIGroupSprite(void* sprite);
void __19VIStreamAudioSprite(void* sprite);
void __13VIFloraSprite(void* sprite);
void __20VIEffectVolumeSprite(void* sprite);

static VISprite* make(unsigned size, void (*construct)(void*)) {
    void* object = __builtin_new(size);
    if (object) construct(object);
    return static_cast<VISprite*>(object);
}

int CreateSprite__7VIScene12VISpriteTypei(VIScene* scene, int type, int addRef) {
    VISprite* sprite = 0;
    switch (type) {
    case 1: sprite = make(76, __14VISimpleSprite); break;
    case 2: sprite = make(68, __13VIPointSprite); break;
    case 3: sprite = make(76, __12VISkinSprite); break;
    case 4: sprite = make(400, __9VIHSprite); break;
    case 5: sprite = make(4624, __9VICSprite); break;
    case 6: sprite = make(116, __12VINameSprite); break;
    case 7: sprite = make(112, __11VILODSprite); break;
    case 8: sprite = make(80, __12VIPointLight); break;
    case 9: sprite = make(88, __13VISoundSprite); break;
    case 10: sprite = make(1088, __16VIParticleSprite); break;
    case 11: sprite = make(228, __13VIGroupSprite); break;
    case 12: sprite = make(200, __19VIStreamAudioSprite); break;
    case 13: sprite = make(72, __13VIFloraSprite); break;
    case 14: sprite = make(192, __20VIEffectVolumeSprite); break;
    default: break;
    }
    if (!sprite) return -1;
    if (addRef) ++*(reinterpret_cast<int*>(sprite) + 1);
    return Add__t6VIPool1ZP8VISpriteRCP8VISprite(reinterpret_cast<char*>(scene) + 996, &sprite);
}
}

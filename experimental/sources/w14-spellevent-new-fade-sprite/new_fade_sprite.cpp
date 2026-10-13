// Observed: NewFadeSprite__18VISpellEventDriverP12VISpellEvent at 0x0117d998 is 692 bytes in the
// pinned SLUS-20565 image. Arguments: (this, record). Returns -1 on any failure path, otherwise 0.
// "Chain search" means: start at the node the word at this+76 points to; a node whose first word
// (its next node) is zero ends the chain and is not examined; a node's event is the word at node+116,
// an event's kind is the int at event+0 and its id the int at event+4. scene = the word at this+20,
// raster = the word at scene+19356, dictionary = the word at scene+19352.
//   1. node = the first node whose event has kind 9 and id equal to the int at record+4; none: -1.
//   2. a = __builtin_new(184); when non-null it calls __18VISpellActiveEventP12VISpellEventP7VIScene
//      (a, record, scene). Stores the 64-bit GetTime__7VIScene(scene) at a+176 (also when null).
//   3. s = GetActorSprite__C7VIScenei(scene, the int at node+140); sprite = Sprite__7VIScenei(scene,
//      s). When the int at sprite+52 is 1: c = CopySprite__7VISceneii(scene, s, 0);
//      SetActorSprite__7VISceneii(scene, node+140 int, c); sprite2 = GetActorSpritePtr__C7VIScenei
//      (scene, node+140 int); pal = CopyMaterialPal__8VIRasterii(raster, GetMaterialPal__C14VISimple
//      Sprite(sprite2), 0); SetMaterialPal__14VISimpleSpriteiR12VIDictionaryR8VIRaster(sprite2, pal,
//      dictionary, raster).
//      Otherwise: first = GetFirstAttach__C9VIHSprite(sprite), negative: -1; asprite =
//      GetAttachSprite__C9VIHSpritei(sprite, first) and anode = GetAttachNode__C9VIHSpritei(sprite,
//      first), either negative: -1; c = CopySprite(scene, asprite, 0); Detach__9VIHSpriteiR7VIScene
//      (sprite, first, scene); Attach__9VIHSpriteiiR7VIScene(sprite, c, anode, scene); sprite2 =
//      Sprite(scene, c); then the same palette copy and SetMaterialPal on sprite2 as above.
//   4. entry = GetMaterialPalEntry__8VIRasterii(raster, pal, 0); zero: -1. Otherwise colour =
//      GetLayerColor__C10VIMateriali(entry, 0), a four-byte colour returned through a hidden first
//      pointer argument; stores (float)colour byte 3 times the float constant the image carries
//      (bits 0x3b808080) at a+156, then AddTail__7VILListP7VILNode(this+76, a) and returns 0.
//      In both -1 cases after step 2 the active event a is neither freed nor linked.
// Inferred: record type 14 of the event table, a sprite fade: the sprite of an earlier record-9 event
// with the same id is cloned (directly, or the first attached child when the sprite itself is not of
// kind 1) so that a private copy of its palette can be modified, and the start alpha (byte 3 of the
// first layer colour, scaled to 0..1 by about 1/255) is recorded at a+156 for ProcessFadeSprite.
// Unknown: what the sprite kinds are besides 1, the attach node/sprite semantics, what palette entry
// and layer 0 stand for, and the exact reference-versus-pointer type of the dictionary and raster
// arguments. Nothing here ran on the EE.
struct Color32 {
    unsigned char r, g, b, a;
};
extern "C" {
void* __builtin_new(unsigned size);
void __18VISpellActiveEventP12VISpellEventP7VIScene(void* self, void* event, void* scene);
long long GetTime__7VIScene(void* scene);
int GetActorSprite__C7VIScenei(void* scene, int actor);
unsigned char* Sprite__7VIScenei(void* scene, int index);
int CopySprite__7VISceneii(void* scene, int index, int flag);
void SetActorSprite__7VISceneii(void* scene, int actor, int sprite);
void* GetActorSpritePtr__C7VIScenei(void* scene, int actor);
void* GetMaterialPal__C14VISimpleSprite(void* sprite);
int CopyMaterialPal__8VIRasterii(void* raster, void* palette, int flag);
void SetMaterialPal__14VISimpleSpriteiR12VIDictionaryR8VIRaster(void* sprite, int palette, void* dictionary, void* raster);
int GetFirstAttach__C9VIHSprite(void* sprite);
int GetAttachSprite__C9VIHSpritei(void* sprite, int attach);
int GetAttachNode__C9VIHSpritei(void* sprite, int attach);
void Detach__9VIHSpriteiR7VIScene(void* sprite, int attach, void* scene);
void Attach__9VIHSpriteiiR7VIScene(void* sprite, int child, int node, void* scene);
void* GetMaterialPalEntry__8VIRasterii(void* raster, int palette, int index);
void GetLayerColor__C10VIMateriali(Color32* result, void* material, int layer);
void AddTail__7VILListP7VILNode(void* list, void* node);

int NewFadeSprite__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    unsigned char* node = *reinterpret_cast<unsigned char**>(self + 76);
    unsigned char* next = *reinterpret_cast<unsigned char**>(node);
    void* raster = *reinterpret_cast<void**>(*reinterpret_cast<unsigned char**>(self + 20) + 19356);
    int id = *reinterpret_cast<int*>(record + 4);
    unsigned char* spriteNode = 0;
    while (next != 0) {
        unsigned char* event = *reinterpret_cast<unsigned char**>(node + 116);
        if (*reinterpret_cast<int*>(event + 4) == id && *reinterpret_cast<int*>(event) == 9) {
            spriteNode = node;
            break;
        }
        node = next;
        next = *reinterpret_cast<unsigned char**>(node);
    }
    if (spriteNode == 0) {
        return -1;
    }
    unsigned char* a = static_cast<unsigned char*>(__builtin_new(184));
    if (a != 0) {
        __18VISpellActiveEventP12VISpellEventP7VIScene(a, record, *reinterpret_cast<void**>(self + 20));
    }
    *reinterpret_cast<long long*>(a + 176) = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    int s = GetActorSprite__C7VIScenei(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(spriteNode + 140));
    unsigned char* sprite = Sprite__7VIScenei(*reinterpret_cast<void**>(self + 20), s);
    int palette;
    unsigned char* sprite2;
    if (*reinterpret_cast<int*>(sprite + 52) == 1) {
        int copy = CopySprite__7VISceneii(*reinterpret_cast<void**>(self + 20), s, 0);
        SetActorSprite__7VISceneii(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(spriteNode + 140), copy);
        sprite2 = static_cast<unsigned char*>(GetActorSpritePtr__C7VIScenei(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(spriteNode + 140)));
        palette = CopyMaterialPal__8VIRasterii(raster, GetMaterialPal__C14VISimpleSprite(sprite2), 0);
        SetMaterialPal__14VISimpleSpriteiR12VIDictionaryR8VIRaster(sprite2, palette, *reinterpret_cast<void**>(*reinterpret_cast<unsigned char**>(self + 20) + 19352), raster);
    } else {
        int first = GetFirstAttach__C9VIHSprite(sprite);
        if (first < 0) {
            return -1;
        }
        int attachSprite = GetAttachSprite__C9VIHSpritei(sprite, first);
        int attachNode = GetAttachNode__C9VIHSpritei(sprite, first);
        if (attachSprite < 0) {
            return -1;
        }
        if (attachNode < 0) {
            return -1;
        }
        int copy = CopySprite__7VISceneii(*reinterpret_cast<void**>(self + 20), attachSprite, 0);
        Detach__9VIHSpriteiR7VIScene(sprite, first, *reinterpret_cast<void**>(self + 20));
        Attach__9VIHSpriteiiR7VIScene(sprite, copy, attachNode, *reinterpret_cast<void**>(self + 20));
        sprite2 = Sprite__7VIScenei(*reinterpret_cast<void**>(self + 20), copy);
        palette = CopyMaterialPal__8VIRasterii(raster, GetMaterialPal__C14VISimpleSprite(sprite2), 0);
        SetMaterialPal__14VISimpleSpriteiR12VIDictionaryR8VIRaster(sprite2, palette, *reinterpret_cast<void**>(*reinterpret_cast<unsigned char**>(self + 20) + 19352), raster);
    }
    void* entry = GetMaterialPalEntry__8VIRasterii(*reinterpret_cast<void**>(*reinterpret_cast<unsigned char**>(self + 20) + 19356), palette, 0);
    if (entry == 0) {
        return -1;
    }
    Color32 color;
    GetLayerColor__C10VIMateriali(&color, entry, 0);
    *reinterpret_cast<float*>(a + 156) = static_cast<float>(color.a) * (1.0f / 255.0f);
    AddTail__7VILListP7VILNode(self + 76, a);
    return 0;
}
}

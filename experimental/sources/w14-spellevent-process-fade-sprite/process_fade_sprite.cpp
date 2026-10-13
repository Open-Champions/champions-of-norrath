// Observed: ProcessFadeSprite__18VISpellEventDriverP18VISpellActiveEvent at 0x0117f9c8 is 732 bytes in
// the pinned SLUS-20565 image. Arguments: (this, node). Always returns 0. ev = the word at node+116
// (the script record); scene = the word at this+20; raster = the word at scene+19356; start = the
// 64-bit value at node+176.
//   1. elapsed = the low 32 bits of (GetTime__7VIScene(scene) - start) as a signed int. When the int at
//      ev+8 is below elapsed, alpha = the float at ev+20. Otherwise alpha = the float at node+156 plus
//      (the float at ev+20 minus that start) times r, with r = (float)(a second GetTime minus start,
//      through __floatdisf) divided by (float)(the int at ev+8), then max.s with 0.0f and min.s with
//      1.0f.
//   2. sn = the first node of the chain starting at the node the word at this+76 points to (a node whose
//      first word is zero ends the chain and is not examined) whose event has id (int +4) equal to the
//      int at ev+4 and kind (int +0) 9. When sn exists: s = GetActorSprite__C7VIScenei(scene, the int
//      at sn+140); sprite = Sprite__7VIScenei(scene, s); when the int at sprite+52 is 1 the palette is
//      GetMaterialPal__C14VISimpleSprite(sprite); otherwise f = GetFirstAttach__C9VIHSprite(sprite),
//      a = GetAttachSprite__C9VIHSpritei(sprite, f) and the palette is GetMaterialPal(Sprite(scene,
//      a)). entry = GetMaterialPalEntry__8VIRasterii(raster, palette, 0). When it is non-zero:
//      GetLayerColor__C10VIMateriali(&colour, entry, 0) (hidden result pointer), the colour's alpha
//      byte (+3) is replaced by (unsigned)(alpha * 255.0f), and SetLayerColor__10VIMaterialiRC9VIColor32
//      (entry, 0, &colour) is called.
//   3. A third GetTime: when the int at ev+8 is below the new signed low-32-bit elapsed value, the node
//      is removed: Remove__7VILNode(node); ReleaseActor__7VIScenei(the word at node+16, the int at
//      node+140) when that int is not -1; the same for node+144; _$_17VIParticleEmitter(it, 3) for the
//      non-zero words at node+120 and node+124; _$_14VIMotifBlender(it, 3) for the one at node+128;
//      __builtin_delete(node). Returns 0.
// Inferred: the per-frame step of a sprite fade (record type 14): it moves the alpha of the first
// palette layer of the sprite created by the matching type-9 event from its start value (node+156,
// recorded by NewFadeSprite) to the target (ev+20) over ev+8 time units, then keeps it at the target
// and finally ends the event.
// Unknown: the time unit, the layer 0 meaning, why the duration test uses the truncated difference
// first and the full 64-bit one for the ramp, and the exact colour class. Nothing here ran on the EE.
struct Color32 {
    unsigned char r, g, b, a;
};
extern "C" {
long long GetTime__7VIScene(void* scene);
int GetActorSprite__C7VIScenei(void* scene, int actor);
unsigned char* Sprite__7VIScenei(void* scene, int index);
void* GetMaterialPal__C14VISimpleSprite(void* sprite);
int GetFirstAttach__C9VIHSprite(void* sprite);
int GetAttachSprite__C9VIHSpritei(void* sprite, int attach);
void* GetMaterialPalEntry__8VIRasterii(void* raster, void* palette, int index);
void GetLayerColor__C10VIMateriali(Color32* result, void* material, int layer);
void SetLayerColor__10VIMaterialiRC9VIColor32(void* material, int layer, const Color32* color);
void Remove__7VILNode(void* node);
void ReleaseActor__7VIScenei(void* scene, int actor);
void _$_17VIParticleEmitter(void* emitter, int flags);
void _$_14VIMotifBlender(void* blender, int flags);
void __builtin_delete(void* pointer);

static inline float floatMax(float a, float b) {
    float r;
    __asm__("max.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}
static inline float floatMin(float a, float b) {
    float r;
    __asm__("min.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}

int ProcessFadeSprite__18VISpellEventDriverP18VISpellActiveEvent(unsigned char* self, unsigned char* node) {
    unsigned char* ev = *reinterpret_cast<unsigned char**>(node + 116);
    void* raster = *reinterpret_cast<void**>(*reinterpret_cast<unsigned char**>(self + 20) + 19356);
    long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    float alpha;
    if (*reinterpret_cast<int*>(ev + 8) < static_cast<int>(now - *reinterpret_cast<long long*>(node + 176))) {
        alpha = *reinterpret_cast<float*>(ev + 20);
    } else {
        long long later = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
        float ratio = static_cast<float>(later - *reinterpret_cast<long long*>(node + 176)) / static_cast<float>(*reinterpret_cast<int*>(ev + 8));
        float start = *reinterpret_cast<float*>(node + 156);
        alpha = start + (*reinterpret_cast<float*>(ev + 20) - start) * ratio;
        alpha = floatMin(floatMax(alpha, 0.0f), 1.0f);
    }
    unsigned char* chain = *reinterpret_cast<unsigned char**>(self + 76);
    unsigned char* next = *reinterpret_cast<unsigned char**>(chain);
    unsigned char* spriteNode = 0;
    int id = *reinterpret_cast<int*>(ev + 4);
    while (next != 0) {
        unsigned char* event = *reinterpret_cast<unsigned char**>(chain + 116);
        if (*reinterpret_cast<int*>(event + 4) == id && *reinterpret_cast<int*>(event) == 9) {
            spriteNode = chain;
            break;
        }
        chain = next;
        next = *reinterpret_cast<unsigned char**>(chain);
    }
    if (spriteNode != 0) {
        int s = GetActorSprite__C7VIScenei(*reinterpret_cast<void**>(self + 20), *reinterpret_cast<int*>(spriteNode + 140));
        unsigned char* sprite = Sprite__7VIScenei(*reinterpret_cast<void**>(self + 20), s);
        void* palette;
        if (*reinterpret_cast<int*>(sprite + 52) == 1) {
            palette = GetMaterialPal__C14VISimpleSprite(sprite);
        } else {
            int first = GetFirstAttach__C9VIHSprite(sprite);
            int attached = GetAttachSprite__C9VIHSpritei(sprite, first);
            palette = GetMaterialPal__C14VISimpleSprite(Sprite__7VIScenei(*reinterpret_cast<void**>(self + 20), attached));
        }
        void* entry = GetMaterialPalEntry__8VIRasterii(raster, palette, 0);
        if (entry != 0) {
            Color32 color = {0, 0, 0, 255};
            GetLayerColor__C10VIMateriali(&color, entry, 0);
            unsigned scaled = static_cast<unsigned>(alpha * 255.0f);
            color.a = static_cast<unsigned char>(scaled);
            SetLayerColor__10VIMaterialiRC9VIColor32(entry, 0, &color);
        }
    }
    long long end = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    if (*reinterpret_cast<int*>(ev + 8) < static_cast<int>(end - *reinterpret_cast<long long*>(node + 176))) {
        Remove__7VILNode(node);
        if (*reinterpret_cast<int*>(node + 140) != -1) {
            ReleaseActor__7VIScenei(*reinterpret_cast<void**>(node + 16), *reinterpret_cast<int*>(node + 140));
        }
        if (*reinterpret_cast<int*>(node + 144) != -1) {
            ReleaseActor__7VIScenei(*reinterpret_cast<void**>(node + 16), *reinterpret_cast<int*>(node + 144));
        }
        if (*reinterpret_cast<void**>(node + 120) != 0) {
            _$_17VIParticleEmitter(*reinterpret_cast<void**>(node + 120), 3);
        }
        if (*reinterpret_cast<void**>(node + 124) != 0) {
            _$_17VIParticleEmitter(*reinterpret_cast<void**>(node + 124), 3);
        }
        if (*reinterpret_cast<void**>(node + 128) != 0) {
            _$_14VIMotifBlender(*reinterpret_cast<void**>(node + 128), 3);
        }
        __builtin_delete(node);
    }
    return 0;
}
}

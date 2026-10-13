// Observed: RenderSky__12VIAtmosphereR7VISceneR8VICamera at 0x01067f38 is 652
// bytes and returns nothing. It does nothing unless the word at offset 1228 of
// this is nonzero. It builds a zero local matrix M, takes the raster from the
// scene word at 19356, calls sceVu0CopyMatrix(M, camera+48), zeroes the three
// floats at M+48, calls SetModelView__8VIRasterRC10VIMatrix44(raster, M), then
// draws the sprite Sprite__7VIScenei(scene, this[1248]). Drawing a sprite is an
// indirect call: read the vtable word at sprite+60, add 24, take the signed
// halfword d at that address and the function word at +4, and call
// function(sprite + d, camera, scene, raster, 3). If the word at 1560 is zero
// and the word at 1476 is nonzero, a second local matrix N is built and, for
// each of (this+1280, sprite word 1260), (this+1344, 1264), (this+1408, 1268) in
// turn: Mul__10VIMatrix44RC10VIMatrix44T1(N, matrix, M), SetModelView(raster, N),
// then the sprite of that word is drawn as above; afterwards N is destroyed with
// _$_10VIMatrix44 flags 2. Whether or not that block ran, SetModelView(raster, M)
// is called again; if the word at 1560 is zero then
// RenderStars__12VIAtmosphereR7VISceneR8VICamera(this, scene, camera) is called.
// M is destroyed with _$_10VIMatrix44 flags 2.
// Inferred: the sprite call is a virtual draw method (slot 24 bytes into the vtable
// of the sprite object); 1228 is the active flag set by Init and cleared by Clear.
extern "C" {
void sceVu0CopyMatrix(void* destination, const void* source);
void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
void* Sprite__7VIScenei(void* scene, int index);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* result, const void* left, const void* right);
void RenderStars__12VIAtmosphereR7VISceneR8VICamera(void* object, void* scene, void* camera);
void _$_10VIMatrix44(void* matrix, int flags);

static unsigned int word_at(const unsigned char* base, int offset) {
    return *reinterpret_cast<const unsigned int*>(base + offset);
}

static void draw_sprite(void* scene, int index, void* camera, void* raster) {
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, index));
    const unsigned char* entry = reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(word_at(sprite, 60))) + 24;
    short delta = *reinterpret_cast<const short*>(entry);
    typedef void (*Draw)(void*, void*, void*, void*, int);
    Draw draw = reinterpret_cast<Draw>(static_cast<__UINTPTR_TYPE__>(word_at(entry, 4)));
    draw(sprite + delta, camera, scene, raster, 3);
}

void RenderSky__12VIAtmosphereR7VISceneR8VICamera(void* object, void* scene, void* camera) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (*reinterpret_cast<int*>(bytes + 1228) == 0) return;
    alignas(16) float view[16];
    for (int i = 0; i < 16; ++i) view[i] = 0.0f;
    void* raster = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word_at(static_cast<unsigned char*>(scene), 19356)));
    sceVu0CopyMatrix(view, static_cast<unsigned char*>(camera) + 48);
    view[12] = 0.0f;
    view[13] = 0.0f;
    view[14] = 0.0f;
    SetModelView__8VIRasterRC10VIMatrix44(raster, view);
    draw_sprite(scene, static_cast<int>(word_at(bytes, 1248)), camera, raster);
    if (word_at(bytes, 1560) == 0 && word_at(bytes, 1476) != 0) {
        alignas(16) float work[16];
        for (int i = 0; i < 16; ++i) work[i] = 0.0f;
        Mul__10VIMatrix44RC10VIMatrix44T1(work, bytes + 1280, view);
        SetModelView__8VIRasterRC10VIMatrix44(raster, work);
        draw_sprite(scene, static_cast<int>(word_at(bytes, 1260)), camera, raster);
        Mul__10VIMatrix44RC10VIMatrix44T1(work, bytes + 1344, view);
        SetModelView__8VIRasterRC10VIMatrix44(raster, work);
        draw_sprite(scene, static_cast<int>(word_at(bytes, 1264)), camera, raster);
        Mul__10VIMatrix44RC10VIMatrix44T1(work, bytes + 1408, view);
        SetModelView__8VIRasterRC10VIMatrix44(raster, work);
        draw_sprite(scene, static_cast<int>(word_at(bytes, 1268)), camera, raster);
        _$_10VIMatrix44(work, 2);
    }
    SetModelView__8VIRasterRC10VIMatrix44(raster, view);
    if (word_at(bytes, 1560) == 0) RenderStars__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    _$_10VIMatrix44(view, 2);
}
}

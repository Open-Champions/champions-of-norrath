// Observed: RasterDeferred__12VINameSpriteR8VIRaster at 0x010a0ca8 is 568 bytes
// and returns 0. It works on two static members of VINameSprite: the int
// _12VINameSprite$NumNameDefers (0x01190c40) and the array
// _12VINameSprite$NameDefers (0x01190c50, 3456 bytes, 24 records of 144 bytes).
// Phase 1: qsort(NameDefers, NumNameDefers, 144, VINameDeferSort). Phase 2, with
// kept = 0: for each record i in order, compare the 16 bytes at record+56 of
// record i against the same field of every kept record j < kept, using
// Intersects__C6VIRectRC6VIRect(&record[i].field, &record[j].field), stopping at
// the first true result. A record that intersects no kept record is copied into
// slot kept (4 bytes at 0, 4 bytes at 4, 32 bytes at 8, 16 bytes at 40, 16 bytes
// at 56, then sceVu0CopyMatrix(dest+80, src+80) for the 64 bytes at 80; bytes 72
// to 79 are not copied) and kept is incremented. The count is read again after
// each record. Phase 3: with the local vector (0.0f, bits 0x3f199999, 0.0f), for
// each kept record in order SetModelView__8VIRasterRC10VIMatrix44(raster, record+80)
// then DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc(raster, the int
// at record+4, vector, record+40, record+8). Finally NumNameDefers is set to 0.
// Inferred: records are deferred name labels; the field at 56 is a screen rectangle
// used to drop overlapping labels (closest-first order from the sort); record+40
// is a colour, record+8 a 32-byte name, record+80 a model-view matrix.
// Unknown: the record's remaining bytes at 72 to 79 and the comparison order.
extern "C" {
extern int _12VINameSprite$NumNameDefers;
extern unsigned char _12VINameSprite$NameDefers[];
void qsort(void* base, int count, int size, int (*compare)(const void*, const void*));
int VINameDeferSort__FPCvT0(const void* a, const void* b);
int Intersects__C6VIRectRC6VIRect(const void* self, const void* other);
void sceVu0CopyMatrix(void* destination, const void* source);
void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
void DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc(void* raster, int id, const void* position, const void* color, const char* text);

static void copy_bytes(unsigned char* destination, const unsigned char* source, int count) {
    for (int i = 0; i < count; ++i) destination[i] = source[i];
}

int RasterDeferred__12VINameSpriteR8VIRaster(void* raster) {
    float position[3] = {0.0f, 0.0f, 0.0f};
    unsigned char* records = _12VINameSprite$NameDefers;
    int kept = 0;
    qsort(records, _12VINameSprite$NumNameDefers, 144, VINameDeferSort__FPCvT0);
    for (int i = 0; i < _12VINameSprite$NumNameDefers; ++i) {
        unsigned char* source = records + 144 * i;
        int j = 0;
        while (j < kept && Intersects__C6VIRectRC6VIRect(source + 56, records + 144 * j + 56) == 0) ++j;
        if (j == kept) {
            unsigned char* destination = records + 144 * kept;
            copy_bytes(destination + 0, source + 0, 8);
            copy_bytes(destination + 8, source + 8, 32);
            copy_bytes(destination + 40, source + 40, 16);
            copy_bytes(destination + 56, source + 56, 16);
            sceVu0CopyMatrix(destination + 80, source + 80);
            ++kept;
        }
    }
    position[0] = 0.0f;
    position[1] = __builtin_bit_cast(float, 0x3f199999u);
    position[2] = 0.0f;
    for (int i = 0; i < kept; ++i) {
        unsigned char* record = records + 144 * i;
        SetModelView__8VIRasterRC10VIMatrix44(raster, record + 80);
        DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc(raster, *reinterpret_cast<int*>(record + 4), position, record + 40, reinterpret_cast<const char*>(record + 8));
    }
    _12VINameSprite$NumNameDefers = 0;
    return 0;
}
}

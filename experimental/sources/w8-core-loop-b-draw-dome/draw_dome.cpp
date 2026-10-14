// Observed drawDome__Fv at 0x001cd998, 412 bytes (a local symbol).
// Does nothing when domeLump (0x00486b00) is null. Otherwise builds a 48-byte matrix with matMakeTrans from
// worldCameraPos. When the string at levelName+81 equals "hate1", "hate3", "hatecut" or "intro" (tested in that
// order), the matrix is also rotated with matRot(matrix, 2, (short)((frameNm % 65536) * 3)), where the remainder
// is the C signed remainder. Then modelClippingOn = 1, modelSetNoZWrite, lightDisableLocalLights,
// lightSetAmbientColor with a function-local static short array {128, 128, 128, 0} (the local symbol c.1130 at
// 0x00484ec8),
// and lightSetAmbientOnly. For index 0, 1, ... the loop looks up the lump entries "vif" and "tex" at that index
// in domeLump with lumpFindByType; it stops at the first index where either is missing. For each pair it calls
// modelDraw with the word at +4 of the "vif" entry, the word at +4 of the "tex" entry, mode 0, the point
// (1000000.0f, 0.0f, 0.0f), the matrix, no animation, flags -1, no draw record and a final 0. After the loop,
// modelResetOptionalParams and lightResetOptionalParams run. The matrix and point locals are adjacent, as in the
// original frame.
// Inferred: the dome is a sky model. Only the order of lookups and the constants are observed.
// Unknown: the Matrix34 field layout, the lump entry layout beyond +4, what levels "hate1", "hate3", "hatecut"
// and "intro" share, and why the point's x is 1.0e6.
extern "C" void* domeLump;
extern "C" char levelName[];
extern "C" unsigned char worldCameraPos[12];
extern "C" int frameNm;
extern "C" unsigned char modelClippingOn;

struct DomeLumpEntry {
    int first;
    void* data;
};

extern "C" int strcmp(const char* a, const char* b);
extern "C" void matMakeTrans__FP8Matrix34G6Point3(void* matrix, const void* point);
extern "C" void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
extern "C" void modelSetNoZWrite__Fv();
extern "C" void lightDisableLocalLights__Fv();
extern "C" void lightSetAmbientColor__FPs(short* color);
extern "C" void lightSetAmbientOnly__Fv();
extern "C" DomeLumpEntry* lumpFindByType__FPvPci(void* lump, const char* type, int index);
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last);
extern "C" void modelResetOptionalParams__Fv();
extern "C" void lightResetOptionalParams__Fv();

extern "C" void drawDome__Fv() {
    if (!domeLump) return;
    struct Frame {
        unsigned char matrix[48];
        float point[3];
    } frame;
    matMakeTrans__FP8Matrix34G6Point3(frame.matrix, worldCameraPos);
    const char* stage = levelName + 81;
    if (strcmp(stage, "hate1") == 0 || strcmp(stage, "hate3") == 0 || strcmp(stage, "hatecut") == 0 ||
        strcmp(stage, "intro") == 0) {
        int turn = frameNm % 65536;
        matRot__FP8Matrix34is(frame.matrix, 2, (short)(turn * 3));
    }
    static short ambient[4] = {128, 128, 128, 0};
    modelClippingOn = 1;
    modelSetNoZWrite__Fv();
    lightDisableLocalLights__Fv();
    lightSetAmbientColor__FPs(ambient);
    lightSetAmbientOnly__Fv();
    for (int index = 0;; ++index) {
        DomeLumpEntry* model = lumpFindByType__FPvPci(domeLump, "vif", index);
        if (!model) break;
        DomeLumpEntry* texture = lumpFindByType__FPvPci(domeLump, "tex", index);
        if (!texture) break;
        frame.point[0] = 1000000.0f;
        frame.point[1] = 0.0f;
        frame.point[2] = 0.0f;
        modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
            model->data, texture->data, 0, frame.point, frame.matrix, 0, 0xffffffffu, 0, 0);
    }
    modelResetOptionalParams__Fv();
    lightResetOptionalParams__Fv();
}

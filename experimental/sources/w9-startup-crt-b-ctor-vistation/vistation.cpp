// Observed __9VIStation at 0x01017618, 640 bytes: the VIStation constructor, one
// argument (the object). It has the same shape as __7VISetup (0x0114f3b8) with
// different offsets and a different set of sub-objects. In the order of the image:
//  - sub-object constructors __8VIRaster at object+1552 and __9VICollide at
//    object+21104;
//  - floats are cleared to 0.0f at object+21672 (3 floats), object+21692 (3
//    floats), then the 448 bytes object+21712..+22159. As in VISetup, part of
//    that range is written twice, once with scalar float stores and once with
//    quad-word stores of a zeroed vector register; the bytes are the same;
//  - sub-object constructors in this order: __12VIDictionary at +22160,
//    __13VISoundDevice at +22184, __7VIScene at +26672, __6VIFont at +46048,
//    __4VIUI at +46064, __7VIFader at +46152, __5VIPad at +46192,
//    __10VIKeyboard at +46288, __15VIWindowManager at +47000,
//    __20VIWndGenericRenderer at +47136, __9VIWndEula at +87048;
//  - a tail call to printf (0x00302560) with one argument, the string at
//    0x011c3ae8; the result of printf is the result of the constructor and is
//    not used here.
// Inferred: class names are the image's own symbol names; each sub-constructor
// takes only the sub-object address. Unknown: the cleared regions' meaning, the
// contents of the first 1552 bytes (untouched here) and what the printed text is.
extern "C" {
void __8VIRaster(void* self);
void __9VICollide(void* self);
void __12VIDictionary(void* self);
void __13VISoundDevice(void* self);
void __7VIScene(void* self);
void __6VIFont(void* self);
void __4VIUI(void* self);
void __7VIFader(void* self);
void __5VIPad(void* self);
void __10VIKeyboard(void* self);
void __15VIWindowManager(void* self);
void __20VIWndGenericRenderer(void* self);
void __9VIWndEula(void* self);
int printf(const char* format, ...);
extern char norrath_w9_startup_crt_b_view_11c3ae8[];

void __9VIStation(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    __8VIRaster(object + 1552);
    __9VICollide(object + 21104);
    for (int i = 0; i < 3; ++i) {
        reinterpret_cast<float*>(object + 21672)[i] = 0.0f;
        reinterpret_cast<float*>(object + 21692)[i] = 0.0f;
    }
    for (int i = 0; i < 112; ++i) reinterpret_cast<float*>(object + 21712)[i] = 0.0f;
    __12VIDictionary(object + 22160);
    __13VISoundDevice(object + 22184);
    __7VIScene(object + 26672);
    __6VIFont(object + 46048);
    __4VIUI(object + 46064);
    __7VIFader(object + 46152);
    __5VIPad(object + 46192);
    __10VIKeyboard(object + 46288);
    __15VIWindowManager(object + 47000);
    __20VIWndGenericRenderer(object + 47136);
    __9VIWndEula(object + 87048);
    printf(norrath_w9_startup_crt_b_view_11c3ae8);
}
}

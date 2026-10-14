// Observed __7VISetup at 0x0114f3b8, 656 bytes: the VISetup constructor, one
// argument (the object). In the order of the image:
//  - sub-object constructors __8VIRaster at object+0 and __9VICollide at
//    object+19552;
//  - floats are cleared to 0.0f at object+20120 (3 floats), object+20140 (3
//    floats), then the 256 bytes object+20160..+20415 and the 192 bytes
//    object+20416..+20607. The image writes the first 256 bytes twice, once
//    with scalar float stores and once with four quad-word stores of a vector
//    register holding zero (vf0 - vf0); the result is the same zero bytes;
//  - sub-object constructors in this order: __12VIDictionary at +20608,
//    __13VISoundDevice at +20632, __7VIScene at +25120, __6VIFont at +44496,
//    __4VIUI at +44512, __7VIFader at +44600, __5VIPad at +44632,
//    __15VIWindowManager at +44900, __11VIWndSplash at +45032,
//    __12VIWndPatcher at +54704, __9VIWndDnas at +73824, __9VIWndEula at +77160;
//    the last one, __10VIWndLegal at +86928, is a tail call.
// Inferred: the class names are the image's own symbol names; each callee is a
// constructor taking only the sub-object address. Unknown: the meaning of the
// cleared regions (they may be several arrays or fields).
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
void __15VIWindowManager(void* self);
void __11VIWndSplash(void* self);
void __12VIWndPatcher(void* self);
void __9VIWndDnas(void* self);
void __9VIWndEula(void* self);
void __10VIWndLegal(void* self);

void __7VISetup(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    __8VIRaster(self);
    __9VICollide(object + 19552);
    for (int i = 0; i < 3; ++i) {
        reinterpret_cast<float*>(object + 20120)[i] = 0.0f;
        reinterpret_cast<float*>(object + 20140)[i] = 0.0f;
    }
    for (int i = 0; i < 112; ++i)
        reinterpret_cast<float*>(object + 20160)[i] = 0.0f;
    __12VIDictionary(object + 20608);
    __13VISoundDevice(object + 20632);
    __7VIScene(object + 25120);
    __6VIFont(object + 44496);
    __4VIUI(object + 44512);
    __7VIFader(object + 44600);
    __5VIPad(object + 44632);
    __15VIWindowManager(object + 44900);
    __11VIWndSplash(object + 45032);
    __12VIWndPatcher(object + 54704);
    __9VIWndDnas(object + 73824);
    __9VIWndEula(object + 77160);
    __10VIWndLegal(object + 86928);
}
}

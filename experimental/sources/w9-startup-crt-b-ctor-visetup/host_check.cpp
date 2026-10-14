// Host check only; not an EE build input. Sub-object constructors are recorders;
// the object is filled with a nonzero pattern so the cleared ranges show. The
// same call order and changed bytes were compared with the original in a private
// interpreter run (3 random fills, identical logs and memory).
#include "visetup.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static std::vector<unsigned char> storage(0x16000 + 16);
static unsigned char* object;
static std::string calls;
#define REC(name) extern "C" void name(void* self) { calls += std::string(#name) + "@" + std::to_string(static_cast<unsigned char*>(self) - object) + ";"; }
REC(__8VIRaster) REC(__9VICollide) REC(__12VIDictionary) REC(__13VISoundDevice) REC(__7VIScene)
REC(__6VIFont) REC(__4VIUI) REC(__7VIFader) REC(__5VIPad) REC(__15VIWindowManager)
REC(__11VIWndSplash) REC(__12VIWndPatcher) REC(__9VIWndDnas) REC(__9VIWndEula) REC(__10VIWndLegal)

int main() {
    object = storage.data();
    std::memset(object, 0x6b, 0x16000);
    __7VISetup(object);
    int failures = 0;
    const char* want =
        "__8VIRaster@0;__9VICollide@19552;__12VIDictionary@20608;__13VISoundDevice@20632;__7VIScene@25120;"
        "__6VIFont@44496;__4VIUI@44512;__7VIFader@44600;__5VIPad@44632;__15VIWindowManager@44900;"
        "__11VIWndSplash@45032;__12VIWndPatcher@54704;__9VIWndDnas@73824;__9VIWndEula@77160;__10VIWndLegal@86928;";
    if (calls != want) { std::fprintf(stderr, "fail: calls %s\n", calls.c_str()); ++failures; }
    for (int i = 0; i < 0x16000; ++i) {
        const bool zero = (i >= 20120 && i < 20132) || (i >= 20140 && i < 20152) || (i >= 20160 && i < 20608);
        const unsigned char expected = zero ? 0 : 0x6b;
        if (object[i] != expected) { std::fprintf(stderr, "fail: byte %d is %02x\n", i, object[i]); ++failures; break; }
    }
    std::printf("%s\n", failures ? "FAIL" : "ok");
    return failures != 0;
}

// Host check only; not an EE build input. Sub-object constructors and printf are
// recorders; the object is filled with a nonzero pattern so the cleared ranges
// show. The same call order, printf argument and changed bytes were compared
// with the original in a private interpreter run (3 random fills).
#include <cstdio>
#define printf recorded_printf  // the source calls printf; keep the host one out of the way
#include "vistation.cpp"
#undef printf
#include <cstring>
#include <string>
#include <vector>

static std::vector<unsigned char> storage(0x16000 + 16);
static unsigned char* object;
static std::string calls;
char norrath_w9_startup_crt_b_view_11c3ae8[4];
#define REC(name) extern "C" void name(void* self) { calls += std::string(#name) + "@" + std::to_string(static_cast<unsigned char*>(self) - object) + ";"; }
REC(__8VIRaster) REC(__9VICollide) REC(__12VIDictionary) REC(__13VISoundDevice) REC(__7VIScene)
REC(__6VIFont) REC(__4VIUI) REC(__7VIFader) REC(__5VIPad) REC(__10VIKeyboard)
REC(__15VIWindowManager) REC(__20VIWndGenericRenderer) REC(__9VIWndEula)
extern "C" int recorded_printf(const char* format, ...) {
    calls += format == norrath_w9_startup_crt_b_view_11c3ae8 ? "printf;" : "printf?;";
    return 0;
}

int main() {
    object = storage.data();
    std::memset(object, 0x6b, 0x16000);
    __9VIStation(object);
    int failures = 0;
    const char* want =
        "__8VIRaster@1552;__9VICollide@21104;__12VIDictionary@22160;__13VISoundDevice@22184;__7VIScene@26672;"
        "__6VIFont@46048;__4VIUI@46064;__7VIFader@46152;__5VIPad@46192;__10VIKeyboard@46288;"
        "__15VIWindowManager@47000;__20VIWndGenericRenderer@47136;__9VIWndEula@87048;printf;";
    if (calls != want) { std::fprintf(stderr, "fail: calls %s\n", calls.c_str()); ++failures; }
    for (int i = 0; i < 0x16000; ++i) {
        const bool zero = (i >= 21672 && i < 21684) || (i >= 21692 && i < 21704) || (i >= 21712 && i < 22160);
        if (object[i] != (zero ? 0 : 0x6b)) { std::fprintf(stderr, "fail: byte %d is %02x\n", i, object[i]); ++failures; break; }
    }
    std::printf("%s\n", failures ? "FAIL" : "ok");
    return failures != 0;
}

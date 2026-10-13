// Observed: __16VIRealmInterfaceP20VIWndGenericRendererPCc at 0x01102ec8 is 864 bytes
// and returns nothing. Arguments after the object: a renderer pointer and an optional
// server name. Byte offsets are from the object. In order it:
//  - calls the base constructor __Q212RealmService15RealmGatewayAPIUiPCcs(this, 2,
//    name, 0x9f61) where name is the argument, or "con.soegames.net" when it is null;
//  - stores the renderer at 12 and the address of the data symbol _vt$16VIRealmInterface
//    (0x01210298, 600 bytes) at 8;
//  - clears memory with memset: 256 bytes at 16; zero at 272; 1024 at 276; 1024 at 1300;
//    zero at 2324; 20 at 2328; 1024 at 2348; zero at 3372; 20 at 3376; then constructs a
//    VIUDPSocket at 3396 with argument -1 (__11VIUDPSocketi); zero words at 3468, 3472
//    and 3476; 400 bytes at 3480; zero at 3880; 400 bytes at 3884;
//  - fills two arrays of 100 words, at 4284 and at 4684, with the shared empty-string
//    representation of the library string class: each word receives
//    nilRep + 16 and the representation's reference word (offset 8) is incremented,
//    unless its flag word (offset 12) is nonzero, in which case the word receives the
//    result of the Rep clone routine (called with the nilRep address). Both arrays are
//    therefore 100 default-constructed strings; the destructor releases them;
//  - clears 4576 bytes at 5088, a 64-bit zero at 9664, 20 bytes at 9672 and 24 at 9692;
//  - when a name was given prints "Using SRSServer (%s)\n" with it;
//  - repeats the clearing of the regions 276, 1300, 2328, 2348, 3376, 16, 3480, 3884,
//    5088, 9672 and 9692 with the same sizes in that order (the original does this a
//    second time, as an inlined Reset-style step; its body is not established);
//  - stores GetTime__7VITimer() as a 64-bit value at 9728, clears 256 bytes at 9736,
//    25600 at 9992, 20 at 35592 and 2400 at 35612.
// Unknown: the field meanings, the object size beyond 38012 bytes, and the string
// class' own names (only the library symbols are imported).
extern "C" {
extern int norrath_string_nil_rep[] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
extern char norrath_realm_vt[] __asm__("_vt$16VIRealmInterface");

void __Q212RealmService15RealmGatewayAPIUiPCcs(void* object, unsigned mode, const char* name, short port);
void __11VIUDPSocketi(void* socket, int value);
char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(void* rep);
void* memset(void* destination, int value, unsigned count);
int printf(const char* format, ...);
long long GetTime__7VITimer();

static char* empty_string() {
    int* rep = norrath_string_nil_rep;
    if (rep[3] != 0)
        return clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(rep);
    rep[2] = rep[2] + 1;
    return reinterpret_cast<char*>(rep) + 16;
}

void __16VIRealmInterfaceP20VIWndGenericRendererPCc(void* object, void* renderer, const char* name) {
    char* base = static_cast<char*>(object);
    const char* use = name != nullptr ? name : "con.soegames.net";
    __Q212RealmService15RealmGatewayAPIUiPCcs(object, 2, use, static_cast<short>(0x9f61));
    *reinterpret_cast<void**>(base + 12) = renderer;
    *reinterpret_cast<char**>(base + 8) = norrath_realm_vt;
    memset(base + 16, 0, 256);
    *reinterpret_cast<unsigned*>(base + 272) = 0;
    memset(base + 276, 0, 1024);
    memset(base + 1300, 0, 1024);
    *reinterpret_cast<unsigned*>(base + 2324) = 0;
    memset(base + 2328, 0, 20);
    memset(base + 2348, 0, 1024);
    *reinterpret_cast<unsigned*>(base + 3372) = 0;
    memset(base + 3376, 0, 20);
    __11VIUDPSocketi(base + 3396, -1);
    *reinterpret_cast<unsigned*>(base + 3468) = 0;
    *reinterpret_cast<unsigned*>(base + 3472) = 0;
    *reinterpret_cast<unsigned*>(base + 3476) = 0;
    memset(base + 3480, 0, 400);
    *reinterpret_cast<unsigned*>(base + 3880) = 0;
    memset(base + 3884, 0, 400);
    char** strings = reinterpret_cast<char**>(base + 4284);
    for (int index = 0; index < 100; ++index) strings[index] = empty_string();
    strings = reinterpret_cast<char**>(base + 4684);
    for (int index = 0; index < 100; ++index) strings[index] = empty_string();
    memset(base + 5088, 0, 4576);
    *reinterpret_cast<unsigned long long*>(base + 9664) = 0;
    memset(base + 9672, 0, 20);
    memset(base + 9692, 0, 24);
    if (name != nullptr) printf("Using SRSServer (%s)\n", name);
    memset(base + 276, 0, 1024);
    memset(base + 1300, 0, 1024);
    memset(base + 2328, 0, 20);
    memset(base + 2348, 0, 1024);
    memset(base + 3376, 0, 20);
    memset(base + 16, 0, 256);
    memset(base + 3480, 0, 400);
    memset(base + 3884, 0, 400);
    memset(base + 5088, 0, 4576);
    memset(base + 9672, 0, 20);
    memset(base + 9692, 0, 24);
    *reinterpret_cast<unsigned long long*>(base + 9728) = static_cast<unsigned long long>(GetTime__7VITimer());
    memset(base + 9736, 0, 256);
    memset(base + 9992, 0, 25600);
    memset(base + 35592, 0, 20);
    memset(base + 35612, 0, 2400);
}
}

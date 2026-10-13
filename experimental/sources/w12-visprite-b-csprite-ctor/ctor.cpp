// Observed __9VICSprite at 0x010ada18, 772 bytes in the pinned SLUS-20565 image. With the object as
// this it does, in order:
//   1. __9VIHSprite(this); the address of _vt$9VICSprite is stored at this+60.
//   2. 66 records of 52 bytes from this+424 (to +3856): words -1, -1, float 1.0, words 0, 0, 0, -1,
//      -1, floats 1.0, 1.0, 0.0, 0.0 and word 0.
//   3. 19 records of 24 bytes from this+3896 (to +4352): words 0, -1, -1, -1, 0, 0.
//   4. Words 0, 2, 1 at this+4352, +4356, +4360; floats zero over +4424 up to +4448 and at +4448 and
//      +4452; words -1, 0, -1 at +4592, +4596, +4600; an 8-byte zero at +4608.
//   5. __t7VIArray1Z20VICSpritePartEmitter(this+4616).
//   6. Final stores: word 5 at +52; words 1, 0, 9, 0 at +400, +404, +408, +412; -1 at +416 and +420;
//      -1 over +3856 up to +3896; at +4364, +4376 and +4388 the triple (-1, 11, -1); -1 over +4400
//      up to +4424; 0 at +4456; the four-word group (0, -1, 0, -1) at +4460, +4476 and +4508; 0 at
//      +4492 and +4500 with -1 at +4496 and +4504; word 1 at +4544 and zero over +4528 up to +4544;
//      floats 0.25, 0.25, 0.25, 1.0 at +4548; zero over +4568 up to +4576, word 0 at +4576, -1 at
//      +4580 and +4584, 0 at +4588.
//   Nothing is returned.
// Inferred: the 66-record and 19-record tables are arrays of slot records initialised to a
// "no entry" value of -1; the four-float 0.25/1.0 group is a colour or scale; the part emitter
// array at +4616 is a template array constructed last.
// Unknown: the meaning of each field and the object size; whether the 66 and 19 counts are
// capacities. Nothing here ran on the EE.
extern "C" {
void __9VIHSprite(void* self);
void __t7VIArray1Z20VICSpritePartEmitter(void* self);
extern unsigned char _vt$9VICSprite[] __asm__("_vt$9VICSprite");

void fn_9VICSprite(void* self) __asm__("__9VICSprite");
void fn_9VICSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    auto word = [bytes](int at) -> unsigned& { return *reinterpret_cast<unsigned*>(bytes + at); };
    auto flt = [bytes](int at) -> float& { return *reinterpret_cast<float*>(bytes + at); };
    const unsigned none = static_cast<unsigned>(-1);
    __9VIHSprite(self);
    word(60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VICSprite));
    for (int i = 0; i < 66; ++i) {
        const int r = 424 + 52 * i;
        word(r) = none; word(r + 4) = none; flt(r + 8) = 1.0f;
        word(r + 12) = 0; word(r + 16) = 0; word(r + 20) = 0;
        word(r + 24) = none; word(r + 28) = none;
        flt(r + 32) = 1.0f; flt(r + 36) = 1.0f; flt(r + 40) = 0.0f; flt(r + 44) = 0.0f;
        word(r + 48) = 0;
    }
    for (int i = 0; i < 19; ++i) {
        const int r = 3896 + 24 * i;
        word(r) = 0; word(r + 4) = none; word(r + 8) = none; word(r + 12) = none;
        word(r + 16) = 0; word(r + 20) = 0;
    }
    word(4352) = 0; word(4356) = 2; word(4360) = 1;
    for (int at = 4424; at < 4448; at += 4) flt(at) = 0.0f;
    flt(4448) = 0.0f; flt(4452) = 0.0f;
    word(4592) = none; flt(4596) = 0.0f; word(4600) = none;
    flt(4548) = 0.0f; flt(4552) = 0.0f; flt(4556) = 0.0f; flt(4560) = 0.0f;
    *reinterpret_cast<unsigned long long*>(bytes + 4608) = 0;
    __t7VIArray1Z20VICSpritePartEmitter(bytes + 4616);
    word(52) = 5;
    word(408) = 9;
    word(400) = 1;
    word(416) = none; word(420) = none;
    word(4456) = 0; word(4588) = 0; word(4584) = none;
    word(4496) = none; word(4504) = none;
    word(404) = 0; word(412) = 0; word(4492) = 0; word(4500) = 0;
    auto group = [&](int g) { word(g) = 0; word(g + 4) = none; word(g + 8) = 0; word(g + 12) = none; };
    group(4460);
    group(4476);
    group(4508);
    word(4544) = 1;
    *reinterpret_cast<unsigned long long*>(bytes + 4528) = 0;
    *reinterpret_cast<unsigned long long*>(bytes + 4536) = 0;
    flt(4548) = 0.25f; flt(4552) = 0.25f; flt(4556) = 0.25f; flt(4560) = 1.0f;
    word(3856) = none;
    *reinterpret_cast<unsigned long long*>(bytes + 4568) = 0;
    word(4576) = 0; word(4580) = none;
    for (int i = 0; i < 3; ++i) {
        const int a = 4364 + 12 * i;
        word(a) = none; word(a + 4) = 11; word(a + 8) = none;
    }
    for (int at = 3860; at < 3896; at += 4) word(at) = none;
    for (int at = 4400; at < 4424; at += 4) word(at) = none;
}
}

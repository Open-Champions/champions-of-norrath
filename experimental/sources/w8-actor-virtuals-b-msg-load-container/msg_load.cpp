// Observed msg_load__9ContainerPc at 0x00217860, 252 bytes.
// Reads from the stream in this order and returns the 28 bytes consumed:
//   five words (+0, +4, +8, +12, +16), each passed to objectSaveGetFromStringPool and the
//   result stored at object+76, +80, +84, +88, +92 in that order;
//   a signed halfword (+20) stored as a float at object+28;
//   a signed halfword (+22) stored as a float at object+32;
//   a word (+24) stored at object+104.
// Then lumpFindResource(object[76], object[80]) is stored at object+96 and
// lumpFindResource(object[76], object[84]) at object+100. When bit 2 (value 4) of the
// word at +104 is set, bit 20 (0x100000) is set in the flag word at object+12.
// Unknown: what the strings select. The pool and lump arguments are 32-bit words.

extern "C" {
unsigned objectSaveGetFromStringPool__Fi(int index);
unsigned lumpFindResource__FPcT0(unsigned name, unsigned kind);
}

namespace {
typedef int Word __attribute__((may_alias));
typedef short Half __attribute__((may_alias));
typedef float FloatWord __attribute__((may_alias));
typedef unsigned UWord __attribute__((may_alias));
}

extern "C" int msg_load__9ContainerPc(void* self, const char* stream) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const Word* words = reinterpret_cast<const Word*>(stream);
    *reinterpret_cast<UWord*>(object + 76) = objectSaveGetFromStringPool__Fi(words[0]);
    *reinterpret_cast<UWord*>(object + 80) = objectSaveGetFromStringPool__Fi(words[1]);
    *reinterpret_cast<UWord*>(object + 84) = objectSaveGetFromStringPool__Fi(words[2]);
    *reinterpret_cast<UWord*>(object + 88) = objectSaveGetFromStringPool__Fi(words[3]);
    *reinterpret_cast<UWord*>(object + 92) = objectSaveGetFromStringPool__Fi(words[4]);
    const Half* halves = reinterpret_cast<const Half*>(stream + 20);
    *reinterpret_cast<FloatWord*>(object + 28) = static_cast<float>(halves[0]);
    *reinterpret_cast<FloatWord*>(object + 32) = static_cast<float>(halves[1]);
    *reinterpret_cast<Word*>(object + 104) = *reinterpret_cast<const Word*>(stream + 24);
    *reinterpret_cast<UWord*>(object + 96) =
        lumpFindResource__FPcT0(*reinterpret_cast<UWord*>(object + 76), *reinterpret_cast<UWord*>(object + 80));
    *reinterpret_cast<UWord*>(object + 100) =
        lumpFindResource__FPcT0(*reinterpret_cast<UWord*>(object + 76), *reinterpret_cast<UWord*>(object + 84));
    if ((*reinterpret_cast<Word*>(object + 104) & 4) != 0) *reinterpret_cast<Word*>(object + 12) |= 0x100000;
    return 28;
}

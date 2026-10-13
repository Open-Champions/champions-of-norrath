// Observed draw__11AddParticleG6Point3P6Point3i at 0x00223768, 1136 bytes. Arguments: the object, a position
// (a Point3, passed by address and copied to a local first), an optional direction pointer (a Point3*) and
// an int mode. Offsets are decimal bytes in the object; words are 32-bit. Nothing is returned.
// A. When the word at +80 and the word at +92 are both nonzero (the particle emitter is present):
//    - the frame counter at +384 is set to frameNm when mode is 0; when mode is 1 and the signed byte at
//      +117 is negative it becomes eRand__10GameObject(object) modulo the absolute value of that byte;
//      in every other case it is incremented;
//    - drawParticleEmitter is called with the position, the definition at +172, the emitter data at +100,
//      the counter, and four extra arguments. Without a direction these are zero. With a direction the
//      halfword at +132 is first reduced by iatan2(direction y, direction z) and the extra first argument is
//      iatan2(direction y, direction x); the original halfword is put back after the call;
//    - when mode is 0 or 3, the word at +96 is nonzero and the signed byte at +160 is zero, the words at +92
//      and +96 are cleared.
// B. When the word at +84 and the word at +252 are both nonzero (the light is present) a radius is chosen,
//    starting from the float at +284, by the word at +332 (a mode: 0 uses the integer at +324 as a float,
//    1 and 2 and 3 are timed changes between the integers at +324 and +328 driven by frameNm, the word at
//    +316 as the next change time, +336 as the interval and, for 1, a random fraction eRand * 2^-16 and, for
//    2, a signed slope at +320; other values keep the float at +284). The radius is limited: 300.0 or more
//    becomes 299.0, below 0.0 becomes 0.0. The position is copied to +304..+315; the halfwords at +340, +344,
//    +348, +352 go to +274..+280 and +266..+272 (as the constructor does), the radius goes to +284 and +292,
//    1 / (radius * radius) to +288 and +296, +300 is zeroed, the byte +264 is cleared and lightAdd is called
//    on the light at +256.
// C. When the word at +88 and the word at +252 are both nonzero a halo sprite is drawn with P_FaceSprite:
//    ef1Noise of (0x007efe41 + 32-bit object address * 0x0019660d + (frameNm << 4)) times 22.0, truncated and
//    shifted left by the word at +376 and added to the word at +372, as the second float; the first float is
//    the integer at +380; the integers at +356, +360, +364 and +368, the texture haloTexture2, 0 and 2 follow.
// Inferred only: the roles of the fields; the names of the modes. Unknown: what mode 3 of the caller is.
extern "C" {
extern int frameNm;
extern void* haloTexture2;
int eRand__10GameObject(void* object);
int iatan2__Fff(float y, float x);
void drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(const void* position, void* definition,
                                                                               void* data, int frame, short angle,
                                                                               bool flag, short second, short third);
float ef1Noise__Fi(int index);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* position, float first, float second, int a, int b, int c,
                                              int d, void* texture, short e, int f);
void lightAdd__FP5Light(void* light);
}

namespace {
inline int& intAt(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
inline float& floatAt(unsigned char* o, unsigned offset) { return *reinterpret_cast<float*>(o + offset); }
inline short& halfAt(unsigned char* o, unsigned offset) { return *reinterpret_cast<short*>(o + offset); }
}

extern "C" void draw__11AddParticleG6Point3P6Point3i(void* self, const void* positionIn, const float* direction,
                                                      int mode) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned position[3];
    const unsigned* source = static_cast<const unsigned*>(positionIn);
    position[0] = source[0];
    position[1] = source[1];
    position[2] = source[2];
    if (intAt(o, 80) != 0 && intAt(o, 92) != 0) {
        if (mode == 0) {
            intAt(o, 384) = frameNm;
        } else if (mode == 1 && static_cast<signed char>(o[117]) < 0) {
            int roll = eRand__10GameObject(o);
            int range = static_cast<signed char>(o[117]);
            if (range < 0) range = -range;
            intAt(o, 384) = roll % range;
        } else {
            intAt(o, 384) = intAt(o, 384) + 1;
        }
        if (direction != 0) {
            short saved = halfAt(o, 132);
            int first = iatan2__Fff(direction[1], direction[2]);
            halfAt(o, 132) = static_cast<short>(static_cast<unsigned short>(halfAt(o, 132)) - first);
            int second = iatan2__Fff(direction[1], direction[0]);
            drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(
                position, o + 172, o + 100, intAt(o, 384), static_cast<short>(second), false, 0, 0);
            halfAt(o, 132) = saved;
        } else {
            drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(
                position, o + 172, o + 100, intAt(o, 384), 0, false, 0, 0);
        }
        if (mode == 0 || mode == 3) {
            if (intAt(o, 96) != 0 && static_cast<signed char>(o[160]) == 0) {
                intAt(o, 92) = 0;
                intAt(o, 96) = 0;
            }
        }
    }
    if (intAt(o, 84) != 0 && intAt(o, 252) != 0) {
        float radius = floatAt(o, 284);
        int kind = intAt(o, 332);
        if (kind == 0) {
            radius = static_cast<float>(intAt(o, 324));
        } else if (kind == 1) {
            if (intAt(o, 316) < frameNm) {
                int low = intAt(o, 324);
                radius = static_cast<float>(low);
                float span = static_cast<float>(intAt(o, 328) - low);
                intAt(o, 316) = frameNm + intAt(o, 336);
                float fraction = static_cast<float>(eRand__10GameObject(o)) * 1.52587890625e-05f;
                span = span * fraction;
                radius = radius + span;
            }
        } else if (kind == 2) {
            int interval = intAt(o, 336);
            if (intAt(o, 316) < frameNm) {
                int slope = -intAt(o, 320);
                intAt(o, 320) = slope;
                intAt(o, 316) = frameNm + interval;
                radius = slope == 1 ? static_cast<float>(intAt(o, 324)) : static_cast<float>(intAt(o, 328));
            }
            if (interval != 0) {
                float step = static_cast<float>(intAt(o, 320)) * static_cast<float>(intAt(o, 328) - intAt(o, 324));
                radius = radius + step / static_cast<float>(interval);
            }
        } else if (kind == 3) {
            if (intAt(o, 316) < frameNm) {
                float low = static_cast<float>(intAt(o, 324));
                intAt(o, 316) = frameNm + intAt(o, 336);
                radius = radius == low ? static_cast<float>(intAt(o, 328)) : low;
            }
        }
        if (300.0f <= radius) radius = 299.0f;
        if (radius < 0.0f) radius = 0.0f;
        float inverse = 1.0f / (radius * radius);
        short a = halfAt(o, 340), b = halfAt(o, 344), c = halfAt(o, 348), d = halfAt(o, 352);
        unsigned* store = reinterpret_cast<unsigned*>(o + 304);
        store[0] = position[0];
        store[1] = position[1];
        store[2] = position[2];
        floatAt(o, 300) = 0.0f;
        halfAt(o, 274) = a;
        halfAt(o, 276) = b;
        halfAt(o, 278) = c;
        halfAt(o, 280) = d;
        floatAt(o, 296) = inverse;
        floatAt(o, 284) = radius;
        floatAt(o, 292) = radius;
        floatAt(o, 288) = inverse;
        halfAt(o, 266) = a;
        halfAt(o, 268) = b;
        halfAt(o, 270) = c;
        halfAt(o, 272) = d;
        o[264] = 0;
        lightAdd__FP5Light(o + 256);
    }
    if (intAt(o, 88) != 0 && intAt(o, 252) != 0) {
        unsigned address = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(o));
        int index = static_cast<int>(0x007efe41u + address * 0x0019660du + (static_cast<unsigned>(frameNm) << 4));
        float noise = ef1Noise__Fi(index) * 22.0f;
        unsigned shift = static_cast<unsigned>(intAt(o, 376)) & 31u;
        unsigned scaled = static_cast<unsigned>(static_cast<int>(noise)) << shift;
        int second = static_cast<int>(static_cast<unsigned>(intAt(o, 372)) + scaled);
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(position, static_cast<float>(intAt(o, 380)),
                                                  static_cast<float>(second), intAt(o, 356), intAt(o, 360),
                                                  intAt(o, 364), intAt(o, 368), haloTexture2, 0, 2);
    }
}

/* Observed doSuperSample__Fiib at 0x0018fe00, 992 bytes in the pinned
 * image. When the mode argument differs from the madePacket.769 word,
 * makeSamplePacket__Fb(mode != 0) rebuilds the sample packet and the word
 * is updated. The composite color defaults to 128 gray unless useComposite
 * is nonzero, in which case compositeAlpha/R/G/B are used. A nonzero
 * trippleBufferMode word with alpha below 128 builds a fixed 12-quadword
 * packet in frameStatePacket (written through the uncached alias, sent with
 * the ordinary address): the DMA/GIF header, one buffer word combining the
 * selected display-buffer base (buffer nonzero selects displayBuffer2, else
 * displayBuffer1) and the other base, then the constant words 80, 0, 81,
 * 0xe000000280, 82, 2, 83, with tag 0x70000005. The alternate tag arm keyed
 * on the shifted tag bits is dead for this construction. The packet is sent
 * with sceDmaSend(dmaGif, frameStatePacket) followed by sceGsSyncPath(0, 0).
 * copyScreenAlpha receives alpha clamped to 0..128. When
 * engineBrightnessCorrection is at least 0.2 (including NaN),
 * compositeAlpha grows by 3 and the scale becomes 0.2; otherwise the word is
 * used as-is. copyScreenColor receives the clamped R/G/B bytes of twice the
 * scaled composite color. Each of compositeR/G/B then steps toward the
 * 124..133 band: at or above 133 it drops by 4, inside 124..132 it becomes
 * 128, below 124 it grows by 4. copyScreenFrame receives the shifted
 * selected buffer base with 0xa0000 merged. FlushCache(0) runs, then
 * sceDmaSend(dmaGif, copyScreen) and sceGsSyncPath(0, 0).
 * engineBrightnessCorrection, the composite words, the display-buffer words,
 * frameStatePacket, copyScreen, and the copy-screen pointers stay imports.
 * Fresh compilation is not an original-byte match. FCSR rounding,
 * hardware packet effects, and caller argument meanings are unknown.
 */
using U64 = unsigned long long;
extern "C" int madePacket_769 asm("madePacket.769") = -1;
extern "C" int trippleBufferMode;
extern "C" int compositeAlpha;
extern "C" int compositeR;
extern "C" int compositeG;
extern "C" int compositeB;
extern "C" float engineBrightnessCorrection;
extern "C" int displayBuffer1StartAddress;
extern "C" int displayBuffer2StartAddress;
extern "C" U64 frameStatePacket[];
extern "C" U64 copyScreen[];
extern "C" unsigned char* copyScreenFrame;
extern "C" unsigned char* copyScreenColor;
extern "C" unsigned char* copyScreenAlpha;
extern "C" void* dmaGif;
extern "C" void makeSamplePacket__Fb(int full);
extern "C" void sceDmaSend(void* channel, void* packet);
extern "C" int sceGsSyncPath(int mode, int timeout);
extern "C" int FlushCache(int mode);

static int float_to_int_word(float value) {
    if (value > 2147483520.0f) return 0x7fffffff;
    if (value <= -2147483648.0f) return static_cast<int>(0x80000000u);
    const float shifted = value >= 0.0f ? value + 0.5f : value - 0.5f;
    int truncated = static_cast<int>(shifted);
    if (value > -16777216.0f && value < 16777216.0f) {
        const float doubled = value + value;
        const int doubled_int = static_cast<int>(doubled);
        if (static_cast<float>(doubled_int) == doubled && (doubled_int & 1) != 0 &&
            (truncated & 1) != 0)
            truncated += (value >= 0.0f ? -1 : 1);
    }
    return truncated;
}

static int clamp_byte(int value) {
    if (value > 255) return 255;
    if (value < 0) return 0;
    return value;
}

static int step_band(int value) {
    if (value >= 133) return value - 4;
    if (value >= 124) return 128;
    return value + 4;
}

static void store_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" void doSuperSample__Fiib(int buffer, int mode, bool useComposite) {
    if (mode != madePacket_769) {
        makeSamplePacket__Fb(mode != 0 ? 1 : 0);
        madePacket_769 = mode;
    }
    int alpha = 128, red = 128, green = 128, blue = 128;
    if (useComposite) {
        alpha = compositeAlpha;
        red = compositeR;
        green = compositeG;
        blue = compositeB;
    }
    if (trippleBufferMode != 0 && alpha < 128) {
        const int selected = buffer != 0 ? displayBuffer2StartAddress : displayBuffer1StartAddress;
        const int other = buffer != 0 ? displayBuffer1StartAddress : displayBuffer2StartAddress;
        const int selected_shift = selected >> 8;
        const int other_shift = other >> 8;
        const U64 buffer_word = (static_cast<U64>(static_cast<unsigned>(selected_shift)) << 32)
            | (0xa000ULL << 36)
            | static_cast<U64>(static_cast<unsigned>(other_shift | 0xa0000));
#ifdef NORRATH_HOST_CONTRACT
        U64* p = frameStatePacket;
#else
        U64* p = reinterpret_cast<U64*>(reinterpret_cast<unsigned long>(frameStatePacket) | 0x30000000u);
#endif
        p[1] = 0;
        p[2] = 0x1000000000008004ULL;
        p[3] = 14;
        p[4] = buffer_word;
        p[5] = 80;
        p[6] = 0;
        p[7] = 81;
        p[8] = 0xe000000280ULL;
        p[9] = 82;
        p[10] = 2;
        p[11] = 83;
        p[0] = 0x70000005ULL;
        sceDmaSend(dmaGif, frameStatePacket);
        sceGsSyncPath(0, 0);
    }
    int alpha_byte = alpha;
    if (alpha_byte > 128) alpha_byte = 128;
    if (alpha_byte < 0) alpha_byte = 0;
    *copyScreenAlpha = static_cast<unsigned char>(alpha_byte);
    float scale = engineBrightnessCorrection;
    if (!(scale < 0.2f)) {
        compositeAlpha += 3;
        scale = 0.2f;
    }
    const int packed = clamp_byte(float_to_int_word(static_cast<float>(red) * scale * 2.0f))
        | (clamp_byte(float_to_int_word(static_cast<float>(green) * scale * 2.0f)) << 8)
        | (clamp_byte(float_to_int_word(static_cast<float>(blue) * scale * 2.0f)) << 16);
    store_word(copyScreenColor, static_cast<unsigned>(packed));
    compositeR = step_band(red);
    compositeG = step_band(green);
    compositeB = step_band(blue);
    const int frame_base = buffer != 0 ? displayBuffer2StartAddress : displayBuffer1StartAddress;
    const unsigned frame_word =
        (static_cast<unsigned>(frame_base >> 13) | 0xa0000u);
    store_word(copyScreenFrame, frame_word);
    FlushCache(0);
    sceDmaSend(dmaGif, copyScreen);
    sceGsSyncPath(0, 0);
}

/* Observed sceGsSetDefDispEnv at 0x002d19a0, 884 bytes. The six arguments
 * are the environment, a four-bit pixel mode, width, height, and two
 * offsets. Each integer argument is sign-extended to 16 bits. gp.15 offset
 * 2 selects the display timing: 2, 3, or 80. Any other value prints
 * "sceGsDefDispEnv:Not support displaymode for 0x%x!!\n" and leaves offset
 * 24 unwritten. Offset 0 is 102. Offset 8 is 2 when gp.15 offset 0 is zero,
 * 3 when offset 4 is nonzero, and 1 otherwise. Offset 16 packs the pixel
 * mode at bit 15 and ((width + 63) >> 6) masked to six bits at bit 9.
 * Offset 32 is cleared. A zero width executes break 0,7 during the display
 * calculation; resumption after that breakpoint is unknown, so this source
 * returns without writing offsets 24 or 32. When gp.15 offset 2 is not 2 or
 * 3, checkModelVersion runs. A nonzero result calls _GetGsDxDyOffset with
 * that half and four stack slots, and those slots are added into the
 * display word. This source is not an original-byte match.
 */
extern "C" unsigned char* sceGsGetGParam();
extern "C" int checkModelVersion();
extern "C" void _GetGsDxDyOffset(int mode, int* dx0, int* dy0, int* dx1, int* dy1);
extern "C" int scePrintf(const char* text, ...);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_gs_width_break();
static void norrath_width_break() { norrath_gs_width_break(); }
#else
static void norrath_width_break() { asm volatile("break 0,7"); }
#endif

static short norrath_half_at(const unsigned char* param, int offset) {
    return static_cast<short>(param[offset] | (param[offset + 1] << 8));
}

static void norrath_store64(unsigned char* env, int offset, unsigned long long value) {
    for (int byte = 0; byte < 8; ++byte)
        env[offset + byte] = static_cast<unsigned char>(value >> (8 * byte));
}

static unsigned long long norrath_display(unsigned dx, unsigned dy, unsigned magh,
                                         unsigned dw, unsigned dh) {
    return static_cast<unsigned long long>(dx & 0xfffu)
        | (static_cast<unsigned long long>(dy & 0xfffu) << 12)
        | (static_cast<unsigned long long>(magh) << 23)
        | (static_cast<unsigned long long>(dw) << 32)
        | (static_cast<unsigned long long>(dh) << 44);
}

extern "C" void sceGsSetDefDispEnv(unsigned char* env, int psm_in, int width_in,
                                  int height_in, int dx_in, int dy_in) {
    const int psm = static_cast<short>(psm_in);
    const int width = static_cast<short>(width_in);
    const int height = static_cast<short>(height_in);
    const int dx = static_cast<short>(dx_in);
    const int dy = static_cast<short>(dy_in);
    unsigned char* param = sceGsGetGParam();
    const int frame = static_cast<unsigned short>(norrath_half_at(param, 2));
    const int frame_signed = static_cast<short>(frame);
    int off0 = 0;
    int off1 = 0;
    int unused0 = 0;
    int unused1 = 0;
    if (static_cast<unsigned>(frame_signed - 2) >= 2u) {
        if (checkModelVersion() != 0)
            _GetGsDxDyOffset(norrath_half_at(param, 2), &off0, &off1, &unused0, &unused1);
    }
    norrath_store64(env, 0, 102ull);
    int smode = 2;
    if (norrath_half_at(param, 0) != 0)
        smode = norrath_half_at(param, 4) != 0 ? 3 : 1;
    norrath_store64(env, 8, static_cast<unsigned long long>(smode));
    const int fbw = ((width + 63) >> 6) & 0x3f;
    const unsigned long long dispfb =
        (static_cast<unsigned long long>(psm & 0xf) << 15)
        | (static_cast<unsigned long long>(fbw) << 9);
    norrath_store64(env, 16, dispfb);

    const int interlace = static_cast<short>(norrath_half_at(param, 0));
    const int field4 = norrath_half_at(param, 4);
    if (frame_signed == 2 || frame_signed == 3) {
        if (width == 0) {
            norrath_width_break();
            return;
        }
        const int quotient = (width + 2559) / width;
        const int dx_base = frame_signed == 2 ? 636 : 656;
        const int dy_base = frame_signed == 2
            ? (interlace == 1 ? 50 : 25)
            : (interlace == 1 ? 72 : 36);
        const unsigned dx_field = static_cast<unsigned>(dx * quotient + off0 + dx_base);
        const unsigned dy_field = static_cast<unsigned>(dy + off1 + dy_base);
        const unsigned dw = static_cast<unsigned>(quotient * width - 1);
        int dh = height - 1;
        if (interlace == 1 && field4 != 0)
            dh = (height << 1) - 1;
        norrath_store64(env, 24, norrath_display(dx_field, dy_field,
            static_cast<unsigned>(quotient - 1), dw, static_cast<unsigned>(dh)));
    } else if (frame_signed == 80) {
        int delta = 720 - width;
        const int sign = static_cast<unsigned>(delta) >> 31;
        delta = ((delta + sign) >> 1) << 1;
        const unsigned dx_field = static_cast<unsigned>(delta + off0 + (dx * 2 + 232));
        const unsigned dy_field = static_cast<unsigned>(dy + off1 + 35);
        const unsigned dw = static_cast<unsigned>((width << 1) - 1);
        const unsigned dh = static_cast<unsigned>(height - 1);
        norrath_store64(env, 24, norrath_display(dx_field, dy_field, 1u, dw, dh));
    } else {
        scePrintf("sceGsDefDispEnv:Not support displaymode for 0x%x!!\n", frame_signed);
    }
    norrath_store64(env, 32, 0ull);
    (void)unused0;
    (void)unused1;
}

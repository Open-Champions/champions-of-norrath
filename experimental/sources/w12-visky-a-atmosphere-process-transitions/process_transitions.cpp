// Observed: ProcessTransitions__12VIAtmosphereR7VISceneff at 0x010691c8 is 3672 bytes. Arguments: the
// atmosphere (A), the scene, and two floats h (first) and g (second). Words are 32-bit; F[o] is a float
// and W[o] a word at byte offset o of A. The scene's words at +19352, +19356 and +19360 are a dictionary,
// the raster and a sound device; they are only passed on. Material handles and sprite handles are 32-bit
// words returned by the VI calls and passed on unchanged. Fields of A used (names are not known):
//   W[1516] and W[1520] two small state words (previous, next), F[1544] the blend between them,
//   W[1524], W[1528] a pair of texture handles, W[1532] and W[1536] the state indices of the weather
//   (current, next) and W[1540], W[1548], W[1552], W[1556] more state words, W[1476] a flag, F[1472]
//   and F[1564] two fade factors, F[72] the weather intensity, F[128] a mix, F[1192] a frame time,
//   W[1228], W[1232..1244] a flag and four handle words, W[1248], W[1260], W[1264], W[1268] four sprite
//   indices, W[4488] and W[4492] two sound ids, W[4496] and W[4500] two sound handles (-1 for none),
//   F[4504], F[4508], F[4512] three timers and F[4520] a float.
// Behaviour in order (t = F[1544]):
//  1. t == 0: if W[1516] is nonzero F[1472] and F[1564] become 0.0, otherwise F[1564] = starfade(h) and
//     F[1472] = 1.0; continue with step 8. starfade(h) is 1 - h * 0x3e2aaaaa when h < 6.0, (h - 18.0) *
//     0x3e2aaaaa when h > 18.0, else 0.0.
//  2. t != 0: F[1564] = starfade(h); t = min(t + F[1192] * 0x3d088888, 1.0) is stored; alpha (0..255) comes
//     from (W[1516], W[1520]): (0,1): 255 - (int)(t * 255) and F[1564] is scaled by (1 - t); (0, other):
//     255; (1,0): (int)(t * 255) and F[1564] is scaled by t; otherwise 255 when W[1520] is 0, else 0 and
//     F[1564] = 0. W[1476] = alpha != 0.
//  3. Three sprites (indices W[1260], W[1264], W[1268]) have the alpha byte of every layer colour of every
//     palette entry replaced: alpha, max(0, 255 - 2 * (255 - alpha)), alpha. The calls per entry are
//     GetLayerColor then SetLayerColor with the 4-byte colour copied and byte 3 replaced. F[1472] =
//     alpha * 0x3b808080.
//  4. If W[1532] > 0 or W[1536] > 0: classes c0, c1 (0 for <= 0, 1 for 1..3, 2 for >= 4) of the two weather
//     indices set F[128] (c0,c1 = (1,2): t; (2,1): 1 - t; (1,1),(1,0),(0,1): 0; otherwise 1.0). The target
//     intensity is lerp(T[W[1532]], T[W[1536]], t) with T = {0, .15, .5, 1, .15, .5, 1} (indexes are not
//     range checked). A positive target while F[72] is 0 calls ClearAllSplashes; F[72] = target; sounds
//     W[4496], W[4500] are started with Play(sound id W[4488], 127) if they are -1 (the second also gets pan
//     and pitch 4176) and their volumes are set to F[72] * g * ((1 - F[128]) * 0.5). Otherwise F[72] = 0
//     and both sounds are stopped (handles reset to -1).
//  5. Sprite W[1248], palette entry 0: for t < 0.5 three layers are set up (blend mode 3, copies, texture
//     W[1528]) and layer colour alphas become (int)((t + t) * 255) and (int)(((0.5 - t) + (0.5 - t)) * 64);
//     for 0.5 <= t < 1.0 two layers and layer 1 alpha (int)(((t - 0.5) + (t - 0.5)) * 64).
//  6. t == 1.0: the textures of layers 0 and 1 become W[1528], layer 0 alpha 255, layer 1 alpha 64, then the
//     state advances: W[1516] = W[1520], W[1524] = W[1528], t = 0, W[1532] = W[1536], W[1548] = W[1552];
//     when W[1556] differs from W[1552] and W[1228] is nonzero the next sky state is chosen from W[1556]
//     (0, 1 or another value) and the weather index W[1532] (positive or not): W[1552] = W[1556], t = 1e-5
//     (0x3727c5ac) or 0, and W[1520], W[1528] or W[1548] set from W[1232], W[1236] or W[1240]. A second
//     rule then compares W[1540] with W[1532]; when they differ, W[1228] is nonzero and t is 0 it adopts
//     W[1540] as the new W[1536] with t = 1e-5 and picks W[1520] and W[1528] again (see the code).
//  7. If F[72] is 0 the three timers F[4504..4512] are zeroed and the routine ends. If F[128] >= 1.0 it
//     also ends. Otherwise the timer F[4504] counts down by F[1192] (F[4508] = 0 and F[4512] = 1e-5
//     when it is below F[4508]); when it reaches 0 or less, two copies of sound W[4492] are played with
//     volume (1 - F[4520]) * g * ((1 - F[128]) * F[72]) * 0.9, pans -1 and +1, the second at pitch 2496,
//     and ResetLightning runs. Then the two weather sounds' volumes are refreshed as in step 4.
// Inferred: sky/weather blend with star fade, rain and thunder sounds. Unknown: all names, the unit of h
// (looks like an hour of day, 0 to 24), and the meaning of the state words. The 7-entry table is the
// 28 bytes at 0x011cd760 in the SLUS_205.65 image (0, .15, .5, 1, .15, .5, 1), reproduced as literals.
// The original copies that table to the stack and zero-fills an 8-byte local pair with memset before
// classifying the two weather indices; the authored version has no memset call and no stack table copy,
// which does not change any visible effect.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void ClearAllSplashes__12VIAtmosphere(unsigned char* self);
void ResetLightning__12VIAtmosphere(unsigned char* self);
unsigned Sprite__7VIScenei(unsigned char* scene, int index);
unsigned GetMaterialPal__C14VISimpleSprite(unsigned sprite);
int GetMaterialPalSize__8VIRasteri(unsigned raster, unsigned pal);
unsigned GetMaterialPalEntry__8VIRasterii(unsigned raster, unsigned pal, int index);
void GetLayerColor__C10VIMateriali(unsigned char* out, unsigned material, int layer);
void SetLayerColor__10VIMaterialiRC9VIColor32(unsigned material, int layer, const unsigned char* color);
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(unsigned material, int layer, unsigned texture, unsigned raster, unsigned dictionary);
void SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(unsigned material, int count, unsigned raster, unsigned dictionary);
void CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(unsigned material, int to, int from, unsigned raster, unsigned dictionary);
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(unsigned material, int layer, int mode);
int Play__13VISoundDeviceii12VISOUNDCLASS(unsigned device, int sound, int volume, int sound_class);
void SetVolume__13VISoundDeviceif(unsigned device, int handle, float volume);
void SetPan__13VISoundDeviceif(unsigned device, int handle, float pan);
void SetPitch__13VISoundDeviceii(unsigned device, int handle, int pitch);
void Stop__13VISoundDevicei(unsigned device, int handle);

static inline float K(unsigned bits) { union { unsigned u; float f; } v; v.u = bits; return v.f; }
static inline float& F(unsigned char* base, unsigned offset) { return *reinterpret_cast<float*>(base + offset); }
static inline unsigned& W(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }

static inline float starfade(float hour) {
    if (hour < K(0x40c00000)) return K(0x3f800000) - hour * K(0x3e2aaaaa);
    if (K(0x41900000) < hour) return (hour - K(0x41900000)) * K(0x3e2aaaaa);
    return 0.0f;
}

// Replaces the alpha byte of every layer colour of every palette entry of one sprite.
static void fade_sprite(unsigned char* scene, unsigned raster, unsigned index, int alpha) {
    const unsigned pal = GetMaterialPal__C14VISimpleSprite(Sprite__7VIScenei(scene, static_cast<int>(index)));
    const int count = GetMaterialPalSize__8VIRasteri(raster, pal);
    for (int e = 0; e < count; ++e) {
        const unsigned entry = GetMaterialPalEntry__8VIRasterii(raster, pal, e);
        int layer = 0;
        while (layer < static_cast<int>(W(NORRATH_WORD_TO_POINTER(entry), 8))) {
            unsigned char fetched[4];
            GetLayerColor__C10VIMateriali(fetched, entry, layer);
            unsigned char color[4] = {fetched[0], fetched[1], fetched[2], static_cast<unsigned char>(alpha)};
            SetLayerColor__10VIMaterialiRC9VIColor32(entry, layer, color);
            ++layer;
        }
    }
}

static void set_layer_alpha(unsigned entry, int layer, int alpha) {
    unsigned char fetched[4];
    GetLayerColor__C10VIMateriali(fetched, entry, layer);
    unsigned char color[4] = {fetched[0], fetched[1], fetched[2], static_cast<unsigned char>(alpha)};
    SetLayerColor__10VIMaterialiRC9VIColor32(entry, layer, color);
}

static unsigned first_entry(unsigned char* scene, unsigned char* self, unsigned raster) {
    const unsigned pal = GetMaterialPal__C14VISimpleSprite(Sprite__7VIScenei(scene, static_cast<int>(W(self, 1248))));
    return GetMaterialPalEntry__8VIRasterii(raster, pal, 0);
}

static void refresh_volumes(unsigned char* self, unsigned device, float g) {
    if (W(self, 4496) != 0)
        SetVolume__13VISoundDeviceif(device, static_cast<int>(W(self, 4496)), (F(self, 72) * g) * ((K(0x3f800000) - F(self, 128)) * K(0x3f000000)));
    if (W(self, 4500) != 0)
        SetVolume__13VISoundDeviceif(device, static_cast<int>(W(self, 4500)), (F(self, 72) * g) * ((K(0x3f800000) - F(self, 128)) * K(0x3f000000)));
}

void ProcessTransitions__12VIAtmosphereR7VISceneff(unsigned char* self, unsigned char* scene, float hour, float g) {
    const unsigned raster = W(scene, 19356);
    const unsigned dictionary = W(scene, 19352);
    const unsigned device = W(scene, 19360);
    if (F(self, 1544) != 0.0f) {
        const float t0 = F(self, 1544);
        F(self, 1564) = starfade(hour);
        const float stepped = t0 + F(self, 1192) * K(0x3d088888);
        const float t = stepped < K(0x3f800000) ? stepped : K(0x3f800000);
        F(self, 1544) = t;
        int alpha;
        const int a = static_cast<int>(W(self, 1516)), b = static_cast<int>(W(self, 1520));
        if (a == 0 && b == 1) {
            const float inverse = K(0x3f800000) - t;
            const float scaled = t * K(0x437f0000);
            F(self, 1564) = F(self, 1564) * inverse;
            alpha = 255 - static_cast<int>(scaled);
        } else if (a == 0) {
            alpha = 255;
        } else if (a == 1 && b == 0) {
            const float faded = F(self, 1564) * t;
            const float scaled = t * K(0x437f0000);
            F(self, 1564) = faded;
            alpha = static_cast<int>(scaled);
        } else if (b == 0) {
            alpha = 255;
        } else {
            alpha = 0;
            F(self, 1564) = 0.0f;
        }
        W(self, 1476) = alpha != 0 ? 1 : 0;
        fade_sprite(scene, raster, W(self, 1260), alpha);
        {
            int second = 255 - ((255 - alpha) << 1);
            if (second < 0) second = 0;
            fade_sprite(scene, raster, W(self, 1264), second);
        }
        fade_sprite(scene, raster, W(self, 1268), alpha);
        F(self, 1472) = static_cast<float>(alpha) * K(0x3b808080);

        const int c = static_cast<int>(W(self, 1532));
        const int d = static_cast<int>(W(self, 1536));
        if (c > 0 || d > 0) {
            const float levels[7] = {0.0f, K(0x3e199999), 0.5f, 1.0f, K(0x3e199999), 0.5f, 1.0f};
            int m0 = 0, m1 = 0;
            if (c > 0) m0 = c < 4 ? 1 : 2;
            if (d > 0) m1 = d < 4 ? 1 : 2;
            if (m0 == 1 && m1 == 2) F(self, 128) = F(self, 1544);
            else if (m0 == 2 && m1 == 1) F(self, 128) = K(0x3f800000) - F(self, 1544);
            else if ((m0 == 1 && m1 == 1) || (m0 == 1 && m1 == 0) || (m0 == 0 && m1 == 1)) F(self, 128) = 0.0f;
            else F(self, 128) = K(0x3f800000);
            const float from = levels[c], to = levels[d];
            const float target = from + (to - from) * F(self, 1544);
            if (0.0f < target) {
                if (F(self, 72) == 0.0f) ClearAllSplashes__12VIAtmosphere(self);
            }
            F(self, 72) = target;
            if (W(self, 4496) == 0xffffffffu) {
                W(self, 4496) = static_cast<unsigned>(Play__13VISoundDeviceii12VISOUNDCLASS(device, static_cast<int>(W(self, 4488)), 127, 0));
                ResetLightning__12VIAtmosphere(self);
            }
            if (W(self, 4500) == 0xffffffffu) {
                SetPan__13VISoundDeviceif(device, static_cast<int>(W(self, 4496)), -K(0x3f800000));
                W(self, 4500) = static_cast<unsigned>(Play__13VISoundDeviceii12VISOUNDCLASS(device, static_cast<int>(W(self, 4488)), 127, 0));
                SetPan__13VISoundDeviceif(device, static_cast<int>(W(self, 4500)), K(0x3f800000));
                SetPitch__13VISoundDeviceii(device, static_cast<int>(W(self, 4500)), 4176);
            }
            refresh_volumes(self, device, g);
        } else {
            F(self, 72) = 0.0f;
            if (W(self, 4496) != 0) {
                Stop__13VISoundDevicei(device, static_cast<int>(W(self, 4496)));
                W(self, 4496) = 0xffffffffu;
            }
            if (W(self, 4500) != 0) {
                Stop__13VISoundDevicei(device, static_cast<int>(W(self, 4500)));
                W(self, 4500) = 0xffffffffu;
            }
        }

        if (F(self, 1544) < K(0x3f000000)) {
            const unsigned entry = first_entry(scene, self, raster);
            SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(entry, 0, 3);
            if (W(NORRATH_WORD_TO_POINTER(entry), 8) != 3) {
                SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(entry, 3, raster, dictionary);
                CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(entry, 1, 2, raster, dictionary);
                CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(entry, 0, 1, raster, dictionary);
                SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(entry, 1, W(self, 1528), raster, dictionary);
            }
            unsigned char fetched[4];
            GetLayerColor__C10VIMateriali(fetched, entry, 1);
            {
                const float u = F(self, 1544);
                unsigned char color[4] = {fetched[0], fetched[1], fetched[2], 0};
                color[3] = static_cast<unsigned char>(static_cast<int>((u + u) * K(0x437f0000)));
                SetLayerColor__10VIMaterialiRC9VIColor32(entry, 1, color);
            }
            GetLayerColor__C10VIMateriali(fetched, entry, 2);
            {
                const float u = K(0x3f000000) - F(self, 1544);
                unsigned char color[4] = {fetched[0], fetched[1], fetched[2], 0};
                color[3] = static_cast<unsigned char>(static_cast<int>((u + u) * K(0x42800000)));
                SetLayerColor__10VIMaterialiRC9VIColor32(entry, 2, color);
            }
        } else if (F(self, 1544) < K(0x3f800000)) {
            const unsigned entry = first_entry(scene, self, raster);
            if (W(NORRATH_WORD_TO_POINTER(entry), 8) != 2) {
                CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(entry, 1, 0, raster, dictionary);
                CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(entry, 2, 1, raster, dictionary);
                SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(entry, 2, raster, dictionary);
                SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(entry, 1, W(self, 1528), raster, dictionary);
            }
            unsigned char fetched[4];
            GetLayerColor__C10VIMateriali(fetched, entry, 1);
            const float u = F(self, 1544) - K(0x3f000000);
            unsigned char color[4] = {fetched[0], fetched[1], fetched[2], 0};
            color[3] = static_cast<unsigned char>(static_cast<int>((u + u) * K(0x42800000)));
            SetLayerColor__10VIMaterialiRC9VIColor32(entry, 1, color);
        }
        if (F(self, 1544) == K(0x3f800000)) {
            const unsigned entry = first_entry(scene, self, raster);
            SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(entry, 0, W(self, 1528), raster, dictionary);
            SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(entry, 1, W(self, 1528), raster, dictionary);
            set_layer_alpha(entry, 0, 255);
            set_layer_alpha(entry, 1, 64);
            const unsigned next_state = W(self, 1520), next_texture = W(self, 1528), next_weather = W(self, 1536);
            const unsigned sky_current = W(self, 1552), sky_next = W(self, 1556);
            W(self, 1516) = next_state;
            W(self, 1524) = next_texture;
            F(self, 1544) = 0.0f;
            W(self, 1532) = next_weather;
            W(self, 1548) = sky_current;
            if (sky_next != sky_current && W(self, 1228) != 0) {
                W(self, 1552) = sky_next;
                F(self, 1544) = K(0x3727c5ac);
                const int weather = static_cast<int>(next_weather);
                if (sky_next == 0) {
                    if (weather <= 0) { W(self, 1520) = 0; W(self, 1528) = W(self, 1232); }
                    else { F(self, 1544) = 0.0f; W(self, 1548) = 0; }
                } else if (sky_next == 1) {
                    if (weather <= 0) { W(self, 1520) = 0; W(self, 1528) = W(self, 1236); }
                    else { W(self, 1548) = 1; F(self, 1544) = 0.0f; }
                } else {
                    if (weather <= 0) { W(self, 1520) = 1; W(self, 1528) = W(self, 1240); }
                    else { W(self, 1548) = sky_next; F(self, 1544) = 0.0f; }
                }
            }
            const unsigned held = W(self, 1540);
            const unsigned now = W(self, 1532);
            if (held != now && W(self, 1228) != 0 && F(self, 1544) == 0.0f) {
                W(self, 1540) = held;
                F(self, 1544) = K(0x3727c5ac);
                W(self, 1536) = held;
                if (held == 0) {
                    const unsigned sky = W(self, 1548);
                    W(self, 1520) = sky == 2 ? 1 : 0;
                    W(self, 1528) = W(self, 1232 + 4 * sky);
                } else {
                    W(self, 1520) = 1;
                    W(self, 1528) = W(self, 1240);
                }
            }
        }
    } else {
        if (W(self, 1516) != 0) {
            F(self, 1472) = 0.0f;
            F(self, 1564) = 0.0f;
        } else {
            F(self, 1564) = starfade(hour);
            F(self, 1472) = K(0x3f800000);
        }
    }

    if (F(self, 72) == 0.0f) {
        F(self, 4512) = 0.0f;
        F(self, 4508) = 0.0f;
        F(self, 4504) = 0.0f;
        return;
    }
    if (!(F(self, 128) < K(0x3f800000))) return;
    const float timer = F(self, 4504);
    if (timer < F(self, 4508)) {
        F(self, 4508) = 0.0f;
        F(self, 4512) = K(0x3727c5ac);
    }
    const float remaining = timer - F(self, 1192);
    F(self, 4504) = remaining;
    if (remaining <= 0.0f) {
        int handle = Play__13VISoundDeviceii12VISOUNDCLASS(device, static_cast<int>(W(self, 4492)), 127, 0);
        SetVolume__13VISoundDeviceif(device, handle, (((K(0x3f800000) - F(self, 4520)) * g) * ((K(0x3f800000) - F(self, 128)) * F(self, 72))) * K(0x3f666666));
        SetPan__13VISoundDeviceif(device, handle, -K(0x3f800000));
        handle = Play__13VISoundDeviceii12VISOUNDCLASS(device, static_cast<int>(W(self, 4492)), 127, 0);
        SetVolume__13VISoundDeviceif(device, handle, (((K(0x3f800000) - F(self, 4520)) * g) * ((K(0x3f800000) - F(self, 128)) * F(self, 72))) * K(0x3f666666));
        SetPan__13VISoundDeviceif(device, handle, K(0x3f800000));
        SetPitch__13VISoundDeviceii(device, handle, 2496);
        ResetLightning__12VIAtmosphere(self);
    }
    refresh_volumes(self, device, g);
}
}

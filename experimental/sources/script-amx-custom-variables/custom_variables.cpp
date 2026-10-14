// Observed cvProcess__FPPc, 1100 bytes at 0x0026fe88. Walks a null-terminated
// pointer list. Each string is matched by substring, in the original order.
// A failed conversion leaves the original stack slot uninitialized; these
// locals start at zero so that path is defined. Camera, dust, and water
// objects stay unresolved imports.
extern "C" {
char* norrath_factory_strstr(const char* text, const char* needle);
int norrath_player_sscanf(const char* input, const char* format, ...);
void SetMaxHeight__6Cameraf(void* camera, float height);
void SetPitchAtLevel__6Cameraii(void* camera, int level, int pitch);
void DisableZooming__6Camera(void* camera);
void SetDistanceAtLevel__6Cameraif(void* camera, int level, float distance);
void fxUpdateDust__Fiiiiiiii(int a, int b, int c, int d, int e, int f, int g, int h);
extern void* gameCam;
extern unsigned levelFlags;
extern int worldWaterAlpha;
extern int worldWaterAlpha2;
extern unsigned worldWaterColor;
extern unsigned worldWaterColor2;
extern float worldWaterBumpAmount;
extern float worldWaterBumpRadius;
extern int worldWaterBumpTimer;
extern float worldWaterWavyness;
extern float worldWaterWavyness2;
}

namespace {
float wavyness_scale() {
    // Observed multiplier word 0x3cc00000, applied after the wavyness scans.
    const unsigned bits = 0x3cc00000u;
    float value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(bits);
    out[1] = static_cast<unsigned char>(bits >> 8);
    out[2] = static_cast<unsigned char>(bits >> 16);
    out[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}

unsigned pack_color(int first, int second, int third) {
    return static_cast<unsigned>(first)
        | (static_cast<unsigned>(second) << 8)
        | (static_cast<unsigned>(third) << 16);
}
}

extern "C" void norrath_custom_variables_process(char** arguments) {
    if (arguments == nullptr || *arguments == nullptr) return;
    const float scale = wavyness_scale();
    for (char** cursor = arguments; *cursor != nullptr; ++cursor) {
        char* text = *cursor;
        if (norrath_factory_strstr(text, "camMaxHeight") != nullptr) {
            float height = 0.f;
            norrath_player_sscanf(text, "camMaxHeight=%f", &height);
            SetMaxHeight__6Cameraf(gameCam, height);
            continue;
        }
        if (norrath_factory_strstr(text, "dimLevel") != nullptr) {
            int level = 0;
            norrath_player_sscanf(text, "dimLevel=%d", &level);
            if (level == 2) levelFlags |= 0x20000000u;
            else if (level == 1) levelFlags |= 0x04000000u;
            continue;
        }
        if (norrath_factory_strstr(text, "dustColor") != nullptr) {
            int values[8] = {};
            const int count = norrath_player_sscanf(
                text, "dustColor=%d,%d,%d,%d,%d,%d,%d,%d\n",
                &values[0], &values[1], &values[2], &values[3],
                &values[4], &values[5], &values[6], &values[7]);
            if (count == 8)
                fxUpdateDust__Fiiiiiiii(values[0], values[1], values[2], values[3],
                                         values[4], values[5], values[6], values[7]);
            continue;
        }
        if (norrath_factory_strstr(text, "noFight") != nullptr) {
            int enabled = 0;
            norrath_player_sscanf(text, "noFight=%d", &enabled);
            if (enabled != 0) levelFlags |= 0x10000000u;
            continue;
        }
        if (norrath_factory_strstr(text, "camPitch") != nullptr) {
            int level = 0;
            int pitch = 0;
            norrath_player_sscanf(text, "camPitch%d=%d\n", &level, &pitch);
            SetPitchAtLevel__6Cameraii(gameCam, level - 1, pitch);
            continue;
        }
        if (norrath_factory_strstr(text, "camDisableZoom") != nullptr) {
            DisableZooming__6Camera(gameCam);
            continue;
        }
        if (norrath_factory_strstr(text, "camZoom") != nullptr) {
            int level = 0;
            int distance = 0;
            norrath_player_sscanf(text, "camZoom%d=%d\n", &level, &distance);
            SetDistanceAtLevel__6Cameraif(gameCam, level - 1, static_cast<float>(distance));
            continue;
        }
        if (norrath_factory_strstr(text, "waterColor2") != nullptr) {
            int first = 0;
            int second = 0;
            int third = 0;
            norrath_player_sscanf(text, "waterColor2=%d,%d,%d,%d\n",
                                  &first, &second, &third, &worldWaterAlpha2);
            worldWaterColor2 = pack_color(first, second, third);
            continue;
        }
        if (norrath_factory_strstr(text, "waterColor") != nullptr) {
            int first = 0;
            int second = 0;
            int third = 0;
            norrath_player_sscanf(text, "waterColor=%d,%d,%d,%d\n",
                                  &first, &second, &third, &worldWaterAlpha);
            worldWaterColor = pack_color(first, second, third);
            continue;
        }
        if (norrath_factory_strstr(text, "waterBumpAmount") != nullptr) {
            norrath_player_sscanf(text, "waterBumpAmount=%f\n", &worldWaterBumpAmount);
            continue;
        }
        if (norrath_factory_strstr(text, "waterBumpRadius") != nullptr) {
            norrath_player_sscanf(text, "waterBumpRadius=%f\n", &worldWaterBumpRadius);
            continue;
        }
        if (norrath_factory_strstr(text, "waterBumpTimer") != nullptr) {
            norrath_player_sscanf(text, "waterBumpTimer=%d\n", &worldWaterBumpTimer);
            continue;
        }
        if (norrath_factory_strstr(text, "waterWavyness2") != nullptr) {
            norrath_player_sscanf(text, "waterWavyness2=%f\n", &worldWaterWavyness2);
            worldWaterWavyness2 *= scale;
            continue;
        }
        if (norrath_factory_strstr(text, "waterWavyness") != nullptr) {
            norrath_player_sscanf(text, "waterWavyness=%f\n", &worldWaterWavyness);
            worldWaterWavyness *= scale;
        }
    }
}

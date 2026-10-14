#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" void norrath_custom_variables_process(char**);

extern "C" char* norrath_factory_strstr(const char* text, const char* needle) {
    return const_cast<char*>(std::strstr(text, needle));
}
extern "C" int norrath_player_sscanf(const char* input, const char* format, ...) {
    std::va_list arguments;
    va_start(arguments, format);
    const int count = std::vsscanf(input, format, arguments);
    va_end(arguments);
    return count;
}

static int max_height_calls, pitch_calls, zoom_disable_calls, distance_calls, dust_calls;
static float seen_height, seen_distance;
static int seen_pitch_level, seen_pitch, seen_distance_level;
static int dust[8];
static void* camera = reinterpret_cast<void*>(0x11);

extern "C" void* gameCam = camera;
extern "C" unsigned levelFlags = 0x1u;
extern "C" int worldWaterAlpha = -1;
extern "C" int worldWaterAlpha2 = -1;
extern "C" unsigned worldWaterColor = 0;
extern "C" unsigned worldWaterColor2 = 0;
extern "C" float worldWaterBumpAmount = 0;
extern "C" float worldWaterBumpRadius = 0;
extern "C" int worldWaterBumpTimer = 0;
extern "C" float worldWaterWavyness = 0;
extern "C" float worldWaterWavyness2 = 0;

extern "C" void SetMaxHeight__6Cameraf(void* cam, float height) {
    if (cam != camera) std::abort();
    seen_height = height;
    ++max_height_calls;
}
extern "C" void SetPitchAtLevel__6Cameraii(void* cam, int level, int pitch) {
    if (cam != camera) std::abort();
    seen_pitch_level = level;
    seen_pitch = pitch;
    ++pitch_calls;
}
extern "C" void DisableZooming__6Camera(void* cam) {
    if (cam != camera) std::abort();
    ++zoom_disable_calls;
}
extern "C" void SetDistanceAtLevel__6Cameraif(void* cam, int level, float distance) {
    if (cam != camera) std::abort();
    seen_distance_level = level;
    seen_distance = distance;
    ++distance_calls;
}
extern "C" void fxUpdateDust__Fiiiiiiii(int a, int b, int c, int d, int e, int f, int g, int h) {
    dust[0] = a; dust[1] = b; dust[2] = c; dust[3] = d;
    dust[4] = e; dust[5] = f; dust[6] = g; dust[7] = h;
    ++dust_calls;
}

int main() {
    norrath_custom_variables_process(nullptr);
    char* empty[] = {nullptr};
    norrath_custom_variables_process(empty);
    if (max_height_calls != 0) return 1;

    char height[] = "camMaxHeight=12.5";
    char dim[] = "dimLevel=2";
    char dust_text[] = "dustColor=1,2,3,4,5,6,7,8\n";
    char fight[] = "noFight=0";
    char pitch[] = "camPitch3=9\n";
    char disable[] = "camDisableZoom";
    char zoom[] = "camZoom4=30\n";
    char color2[] = "waterColor2=1,2,3,4\n";
    char color[] = "waterColor=5,6,7,8\n";
    char amount[] = "waterBumpAmount=2.5\n";
    char radius[] = "waterBumpRadius=3.5\n";
    char timer[] = "waterBumpTimer=11\n";
    char wavy2[] = "waterWavyness2=64\n";
    char wavy[] = "waterWavyness=64\n";
    char ignored[] = "unrelated";
    char* arguments[] = {
        height, dim, dust_text, fight, pitch, disable, zoom, color2, color,
        amount, radius, timer, wavy2, wavy, ignored, nullptr
    };
    norrath_custom_variables_process(arguments);
    if (max_height_calls != 1 || seen_height != 12.5f) return 2;
    if (levelFlags != (0x1u | 0x20000000u)) return 3;
    if (dust_calls != 1 || dust[0] != 1 || dust[7] != 8) return 4;
    if (pitch_calls != 1 || seen_pitch_level != 2 || seen_pitch != 9) return 5;
    if (zoom_disable_calls != 1) return 6;
    if (distance_calls != 1 || seen_distance_level != 3 || seen_distance != 30.f) return 7;
    if (worldWaterColor2 != (1u | (2u << 8) | (3u << 16)) || worldWaterAlpha2 != 4) return 8;
    if (worldWaterColor != (5u | (6u << 8) | (7u << 16)) || worldWaterAlpha != 8) return 9;
    if (worldWaterBumpAmount != 2.5f || worldWaterBumpRadius != 3.5f || worldWaterBumpTimer != 11) return 10;
    if (worldWaterWavyness != 1.5f || worldWaterWavyness2 != 1.5f) return 11;

    levelFlags = 0;
    char dim_one[] = "dimLevel=1";
    char dim_other[] = "dimLevel=3";
    char fight_on[] = "noFight=4";
    char short_dust[] = "dustColor=1,2,3\n";
    char* second[] = {dim_one, dim_other, fight_on, short_dust, nullptr};
    norrath_custom_variables_process(second);
    if (levelFlags != (0x04000000u | 0x10000000u)) return 12;
    if (dust_calls != 1) return 13;
    std::puts("custom variables passed");
}

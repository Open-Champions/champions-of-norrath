#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <utility>
#include "msg_draw.cpp"
using namespace vslots_b;
static int failures = 0;
// 1 draw, 2 tag, 3 script, 4 transform, 5 lump, 6 isin, 7 color
static std::vector<int> events;
static void* gameCamStorage;
extern "C" void* gameCam = nullptr;
extern "C" int frameNm = 0;
extern "C" unsigned char worldViewMatrix[64] = {};
static void *draw_self, *draw_model, *draw_texture, *draw_extra;
static char* tag_results[4];
static std::vector<std::pair<std::string, int>> tags_seen;
static char* script_seen;
static int script_result;
static int tag_exclamation_null, tag_short_null;
static float transform_in[3], transform_out[3], transform_w;
static void *transform_result_ptr, *transform_matrix;
static char lump_seen[32], resource_seen[32];
static unsigned char tex_storage[4];
static int isin_arg;
static float isin_result;
static void *color_texture;
static int color_x, color_y, color_z;
static unsigned color_value;
static char dummy_script[] = "x";
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra) {
    events.push_back(1); draw_self = self; draw_model = model; draw_texture = texture; draw_extra = extra;
}
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence) {
    (void)object; events.push_back(2); tags_seen.push_back({tag, occurrence});
    if (std::strcmp(tag, "exclamation") == 0) return tag_exclamation_null ? nullptr : dummy_script;
    if (std::strcmp(tag, "exclamationShort") == 0) return tag_short_null ? nullptr : dummy_script;
    return nullptr;
}
extern "C" int scriptStart__FPc(char* name) { events.push_back(3); script_seen = name; return script_result; }
extern "C" float matTransform__FP6Point3P8Matrix44T0(void* result, void* matrix, const void* point) {
    events.push_back(4); transform_result_ptr = result; transform_matrix = matrix;
    std::memcpy(transform_in, point, 12);
    std::memcpy(result, transform_out, 12);
    return transform_w;
}
extern "C" char* lumpFindResource__FPcT0(char* lump_name, char* resource_name) {
    events.push_back(5); std::strncpy(lump_seen, lump_name, 31); std::strncpy(resource_seen, resource_name, 31);
    return reinterpret_cast<char*>(tex_storage);
}
extern "C" float isin__Fi(int angle) { events.push_back(6); isin_arg = angle; return isin_result; }
extern "C" void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color) {
    events.push_back(7); color_texture = texture; color_x = x; color_y = y; color_z = z; color_value = color;
}
static unsigned char object[100], group[32], model[4], texture[4], camera[64];
static void fresh() {
    std::memset(object, 0, sizeof object);
    events.clear(); tags_seen.clear();
    tag_exclamation_null = tag_short_null = 0; script_result = 1;
    gameCam = camera;
    std::memset(camera, 0, sizeof camera);
    write_word(address(camera) + 52u, 1u);
    write_word(address(object) + 36u, address_bits(group));
    write_single(address(object) + 16u, 1.0f); write_single(address(object) + 20u, 2.0f); write_single(address(object) + 24u, 10.0f);
    transform_out[0] = 100.0f; transform_out[1] = 40.0f; transform_out[2] = 8.0f; transform_w = 2.0f;
    isin_result = 0.5f; frameNm = 3;
}
int main() {
    host_note(group); host_note(model); host_note(texture);
    write_word(address(group) + 12u, address_bits(model));
    write_word(address(group) + 20u, address_bits(texture));

    fresh(); gameCam = nullptr;
    msg_draw__14GnomeNavigator(object);
    expect(events == std::vector<int>({1}) && draw_self == object && draw_model == model && draw_texture == texture && draw_extra == nullptr, "no camera: only the creature draw");
    fresh(); write_word(address(camera) + 52u, 2u);
    msg_draw__14GnomeNavigator(object);
    expect(events == std::vector<int>({1}), "camera word not 1");
    fresh(); write_word(address(camera) + 52u, 0u);
    msg_draw__14GnomeNavigator(object);
    expect(events == std::vector<int>({1}), "camera word 0");
    fresh(); tag_exclamation_null = 1;
    msg_draw__14GnomeNavigator(object);
    expect(events == std::vector<int>({1, 2}) && tags_seen[0].first == "exclamation" && tags_seen[0].second == 0, "no tag");
    fresh(); script_result = 0;
    msg_draw__14GnomeNavigator(object);
    expect(events == std::vector<int>({1, 2, 2, 3}) && script_seen == dummy_script && tags_seen[1].first == "exclamation", "script not started");
    fresh(); script_result = 2;
    msg_draw__14GnomeNavigator(object);
    expect(events == std::vector<int>({1, 2, 2, 3}), "script result must be exactly 1");

    fresh();
    msg_draw__14GnomeNavigator(object);
    expect(events == std::vector<int>({1, 2, 2, 3, 2, 4, 5, 6, 7}), "full order");
    expect(tags_seen.size() == 3 && tags_seen[2].first == "exclamationShort" && tags_seen[2].second == 0, "short tag looked up with occurrence 0");
    expect(transform_in[0] == 1.0f && transform_in[1] == 2.0f && transform_in[2] == 58.0f, "point is z + 48 when the short tag is present");
    expect(transform_matrix == worldViewMatrix && transform_result_ptr != nullptr, "matrix argument");
    expect(std::strcmp(lump_seen, "icons.lmp") == 0 && std::strcmp(resource_seen, "exclaim.tex") == 0, "lump and resource names");
    expect(color_texture == tex_storage && color_z == 5, "texture and 5");
    // result / w = (50, 20, 4); x = int(50 * 0.5) + 293, y = 20 + 166
    expect(color_x == 25 + 293 && color_y == 20 + 166, "screen position from the result divided by w");
    expect(isin_arg == (3 << 10), "isin of frameNm << 10");
    expect(color_value == ((static_cast<unsigned>(0.5f * 16.0f + 112.0f) << 24) | 0x808080u), "colour: alpha 120 over 0x808080");

    fresh(); tag_short_null = 1;
    msg_draw__14GnomeNavigator(object);
    expect(transform_in[2] == 82.0f && events == std::vector<int>({1, 2, 2, 3, 2, 4, 5, 6, 7}), "without the short tag z + 72 is kept");
    fresh(); write_single(address(object) + 24u, -4.0f); tag_short_null = 1;
    msg_draw__14GnomeNavigator(object);
    expect(transform_in[2] == 68.0f, "z + 72 uses the object z");
    fresh(); write_single(address(object) + 24u, -4.0f);
    msg_draw__14GnomeNavigator(object);
    expect(transform_in[2] == 44.0f, "z + 48 uses the object z");

    fresh(); transform_out[0] = 3.0f; transform_out[1] = -3.0f; transform_w = 4.0f; isin_result = -1.0f;
    msg_draw__14GnomeNavigator(object);
    expect(color_x == static_cast<int>(0.75f * 0.5f) + 293 && color_y == static_cast<int>(-0.75f) + 166, "truncation toward zero");
    expect(color_value == ((static_cast<unsigned>(-1.0f * 16.0f + 112.0f) << 24) | 0x808080u), "colour at isin -1");
    fresh(); isin_result = 1.0f; frameNm = 1000;
    msg_draw__14GnomeNavigator(object);
    expect(isin_arg == (1000 << 10) && (color_value >> 24) == 128u, "colour at isin 1");
    fresh(); transform_out[0] = 100.0f; transform_w = 0.5f;
    msg_draw__14GnomeNavigator(object);
    expect(color_x == 100 + 293, "divides by w (100 / 0.5 * 0.5)");
    return failures == 0 ? 0 : 1;
}

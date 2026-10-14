#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_hsprite_hierarchy.cpp"

static std::string tr; static int begin_ret, end_ret, nodes_in, tcount_in, init1_ret, init2_ret, root_ret, add_ret, pose_ret, scale_ret, val_in, parent_in[4];
static unsigned short tag_in, ver_in; static int ri, rf; static float pose_extra[4], pose_rot[4][4], pose_pos[4][3], scales[4][3];
alignas(16) static unsigned char node_mem[256 * 4]; alignas(16) static unsigned char hs[400];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = ver_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRi(void*, int* v) {
    // sequence: table count (version != 0), node count, then per node: parent, [value]
    if (ri == 0 && ver_in != 0) *v = tcount_in; else if (ri == (ver_in != 0 ? 1 : 0)) *v = nodes_in; else if ((ri - (ver_in != 0 ? 2 : 1)) % (ver_in != 0 ? 2 : 1) == 0) *v = parent_in[(ri - (ver_in != 0 ? 2 : 1)) / (ver_in != 0 ? 2 : 1) % 4]; else *v = val_in;
    ++ri; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 0.5f + rf++; tr += "F;"; return 0; }
extern "C" int Init__t8VIVector1Z13VIHSpriteNodei(void* v, int c) { if (v != hs + 64 || c != nodes_in) tr += "BADI1"; tr += "N;";
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(v) + 8) = static_cast<unsigned>(reinterpret_cast<uintptr_t>(node_mem)); return init1_ret; }
extern "C" int Init__t8VIVector1Z10VIMatrix44i(void* v, int c) { if (v != hs + 76 || c != nodes_in) tr += "BADI2"; tr += "M;"; return init2_ret; }
extern "C" int AddRoot__9VIHSprite(void* h) { if (h != hs) tr += "BADH"; tr += "R;"; return root_ret; }
extern "C" int Add__9VIHSpritei(void* h, int p) { if (h != hs) tr += "BADH"; tr += "A" + std::to_string(p) + ";"; return add_ret; }
extern "C" int SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3(void* h, int node, const float* q, float f, const float* p) {
    if (h != hs) tr += "BADH"; tr += "P" + std::to_string(node) + ";"; memcpy(pose_rot[node], q, 16); pose_extra[node] = f; memcpy(pose_pos[node], p, 12); return pose_ret; }
extern "C" int SetNodeFixScale__9VIHSpriteiRC7VIVect3(void* h, int node, const float* s) {
    if (h != hs) tr += "BADH"; tr += "S" + std::to_string(node) + ";"; memcpy(scales[node], s, 12); return scale_ret; }
static int run() { alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse); w[9] = 0x55; tr.clear(); ri = 0; rf = 0; memset(node_mem, 0xee, sizeof node_mem); memset(hs, 0xcc, sizeof hs);
    return ParseHSpriteHierarchy__10VIESFParseR9VIHSprite(parse, hs); }
int main() {
    begin_ret = end_ret = init1_ret = init2_ret = pose_ret = scale_ret = 0; root_ret = 1; add_ret = 2; tag_in = 0x2400; ver_in = 2; nodes_in = 2; tcount_in = 2; val_in = 4242; parent_in[0] = -1; parent_in[1] = 5;
    if (run() != 0 || tr != "B;I;F;F;I;N;M;I;R;F;F;F;F;F;F;F;F;P1;I;F;F;F;S1;I;A5;F;F;F;F;F;F;F;F;P2;I;F;F;F;S2;E;") return 1;
    if (*reinterpret_cast<int*>(hs + 168) != 2 || *reinterpret_cast<float*>(hs + 172) != 0.5f || *reinterpret_cast<float*>(hs + 176) != 1.5f || *reinterpret_cast<int*>(hs + 180) != (int)0xcccccccc) return 2;
    if (*reinterpret_cast<int*>(node_mem + 256 + 12) != 4242 || *reinterpret_cast<int*>(node_mem + 512 + 12) != 4242 || *reinterpret_cast<int*>(node_mem + 12) != (int)0xeeeeeeee) return 3;
    if (pose_rot[1][0] != 2.5f || pose_rot[1][3] != 5.5f || pose_extra[1] != 6.5f || pose_pos[1][0] != 7.5f || pose_pos[1][2] != 9.5f || scales[1][0] != 10.5f || scales[1][2] != 12.5f) return 4;
    ver_in = 1; if (run() != 0 || scales[1][0] != 1.0f || scales[1][1] != 1.0f || scales[1][2] != 1.0f || tr.find("S1;I;A5;F;F;F;F;F;F;F;F;P2") == std::string::npos) return 5;
    ver_in = 0; if (run() != 0 || tr != "B;I;N;M;I;R;F;F;F;F;F;F;F;F;P1;S1;I;A5;F;F;F;F;F;F;F;F;P2;S2;E;" || scales[2][0] != 1.0f) return 6;
    ver_in = 2; tcount_in = 0; if (run() != 0 || tr.substr(0, 9) != "B;I;I;N;M") return 7;
    tcount_in = -3; if (run() != 0 || tr.substr(0, 9) != "B;I;I;N;M") return 8;
    tcount_in = 2; scale_ret = -1; if (run() != -1 || tr.find("E;") != std::string::npos || tr.find("S2") != std::string::npos) return 9;
    scale_ret = 0; pose_ret = -1; if (run() != -1 || tr.find("S1") != std::string::npos) return 10;
    pose_ret = 0; add_ret = -1; if (run() != -1 || tr.find("P2") != std::string::npos) return 11;
    add_ret = 2; root_ret = -1; if (run() != -1 || tr.find("P1") != std::string::npos) return 12;
    root_ret = 1; init2_ret = -1; if (run() != -1 || tr.find("I;R") != std::string::npos) return 13;
    init2_ret = 0; init1_ret = -1; if (run() != -1 || tr.find("M;") != std::string::npos) return 14;
    init1_ret = 0; end_ret = -1; if (run() != -1) return 15;
    end_ret = 0; nodes_in = 0; if (run() != 0 || tr != "B;I;F;F;I;N;M;E;") return 16;
    nodes_in = -1; if (run() != 0 || tr != "B;I;F;F;I;N;M;E;") return 17;
    nodes_in = 2; tag_in = 0x2401; if (run() != -1 || tr != "B;") return 18;
    tag_in = 0x2400; begin_ret = -1; if (run() != -1 || tr != "B;") return 19;
    return 0;
}

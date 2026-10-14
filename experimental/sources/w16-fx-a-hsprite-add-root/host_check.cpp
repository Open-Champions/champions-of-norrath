// Host check only; not a build input.
#include "add_root.cpp"
#include <string>
alignas(8) static unsigned char object[128];
static std::string trace;
static int nodeRet, matrixRet;
extern "C" int Add__t8VIVector1Z13VIHSpriteNode(void* v) { trace += "N" + std::to_string(static_cast<unsigned char*>(v) - object) + ";"; return nodeRet; }
extern "C" int Add__t8VIVector1Z10VIMatrix44(void* v) { trace += "M" + std::to_string(static_cast<unsigned char*>(v) - object) + ";"; return matrixRet; }
int main() {
    nodeRet = 3; matrixRet = 0;
    if (AddRoot__9VIHSprite(object) != 3 || trace != "N64;M76;") return 1;
    trace.clear(); nodeRet = -2;
    if (AddRoot__9VIHSprite(object) != -1 || trace != "N64;") return 2;
    trace.clear(); nodeRet = 5; matrixRet = -1;
    if (AddRoot__9VIHSprite(object) != -1 || trace != "N64;M76;") return 3;
    trace.clear(); matrixRet = 0;
    *reinterpret_cast<unsigned*>(object + 64) = 1;
    if (AddRoot__9VIHSprite(object) != -1 || !trace.empty()) return 4;
    return 0;
}

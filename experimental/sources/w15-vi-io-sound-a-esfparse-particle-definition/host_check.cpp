// Host check only; not a build input.
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> log_;
static int version, readIndex, findResult, findType, findId, motifs;
static std::vector<int> ints;
static void note(const std::string& s) { log_.push_back(s); }
extern "C" {
unsigned ObjectVersion__9VIObjFile(unsigned char*) { return version; }
int Read__9VIObjFileRUi(unsigned char*, unsigned* v) { note("RUi"); *v = ints[readIndex++]; return 4; }
int Read__9VIObjFileRi(unsigned char*, int* v) { note("Ri"); *v = ints[readIndex++]; return 4; }
int Read__9VIObjFileRf(unsigned char*, float* v) { note("Rf"); *v = 1.5f; return 4; }
int Read__9VIObjFilePUci(unsigned char*, unsigned char* b, int n) { note("Rb" + std::to_string(n)); std::memset(b, 'a', n); return n; }
void ReadEnd__9VIObjFile(unsigned char*) { note("end"); }
void SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE(unsigned char*, int m) { note("blend" + std::to_string(m)); }
void SetZWrite__20VIParticleDefinitionb(unsigned char*, bool b) { note(std::string("zw") + (b ? "1" : "0")); }
void SetZTest__20VIParticleDefinitionb(unsigned char*, bool b) { note(std::string("zt") + (b ? "1" : "0")); }
void SetTextureConfiguration__20VIParticleDefinitioni(unsigned char*, int c) { note("cfg" + std::to_string(c)); }
void SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary(unsigned char*, int t, unsigned r, unsigned d) { note("tex" + std::to_string(t) + "," + std::to_string(r) + "," + std::to_string(d)); }
int Find__12VIDictionaryUiR14VIResourceTypeRi(unsigned d, unsigned id, int* type, int* found) { note("find" + std::to_string(d) + "," + std::to_string(id)); *type = findType; *found = findId; return findResult; }
unsigned char* CreateMotif__22VIParticleDefinitionEx(unsigned char*) { note("motif"); static unsigned char m[8]; ++motifs; return m; }
void SetName__15VIParticleMotifPCc(unsigned char*, const char*) { note("name"); }
void SetFriction__20VIParticleAttributesf(unsigned char*, float v) { note("Friction" + std::to_string(static_cast<int>(v * 10))); }
void SetBirthrate__20VIParticleAttributesf(unsigned char*, float v) { note("Birthrate" + std::to_string(static_cast<int>(v * 10))); }
void SetBirthrateVar__20VIParticleAttributesf(unsigned char*, float v) { note("BirthrateVar" + std::to_string(static_cast<int>(v * 10))); }
void SetLifespan__20VIParticleAttributesf(unsigned char*, float v) { note("Lifespan" + std::to_string(static_cast<int>(v * 10))); }
void SetLifespanVar__20VIParticleAttributesf(unsigned char*, float v) { note("LifespanVar" + std::to_string(static_cast<int>(v * 10))); }
void SetVelocity__20VIParticleAttributesf(unsigned char*, float v) { note("Velocity" + std::to_string(static_cast<int>(v * 10))); }
void SetVelocityVar__20VIParticleAttributesf(unsigned char*, float v) { note("VelocityVar" + std::to_string(static_cast<int>(v * 10))); }
void SetStartSize__20VIParticleAttributesf(unsigned char*, float v) { note("StartSize" + std::to_string(static_cast<int>(v * 10))); }
void SetStartSizeVar__20VIParticleAttributesf(unsigned char*, float v) { note("StartSizeVar" + std::to_string(static_cast<int>(v * 10))); }
void SetEndSize__20VIParticleAttributesf(unsigned char*, float v) { note("EndSize" + std::to_string(static_cast<int>(v * 10))); }
void SetEndSizeVar__20VIParticleAttributesf(unsigned char*, float v) { note("EndSizeVar" + std::to_string(static_cast<int>(v * 10))); }
void SetInheritVelocity__20VIParticleAttributesf(unsigned char*, float v) { note("InheritVelocity" + std::to_string(static_cast<int>(v * 10))); }
void SetDeltaSpawn__20VIParticleAttributesf(unsigned char*, float v) { note("DeltaSpawn" + std::to_string(static_cast<int>(v * 10))); }
void SetStartColorVar__20VIParticleAttributesRC10VIColor32F(unsigned char*, const float*) { note("StartColorVar"); }
void SetEndColorVar__20VIParticleAttributesRC10VIColor32F(unsigned char*, const float*) { note("EndColorVar"); }
void SetGradientColor__20VIParticleAttributesiRC10VIColor32F(unsigned char*, int i, const float*) { note("Grad" + std::to_string(i)); }
void SetGradientRepeat__20VIParticleAttributesf(unsigned char*, float) { note("GradRepeat"); }
void SetInnerOffset__20VIParticleAttributesRC7VIVect3(unsigned char*, const float*) { note("InnerOffset"); }
void SetInnerHprVar__20VIParticleAttributesRC7VIVect3(unsigned char*, const float*) { note("InnerHprVar"); }
void SetOuterOffset__20VIParticleAttributesRC7VIVect3(unsigned char*, const float*) { note("OuterOffset"); }
void SetOuterHprVar__20VIParticleAttributesRC7VIVect3(unsigned char*, const float*) { note("OuterHprVar"); }
void SetNozzleAxis__20VIParticleAttributesRC7VIVect3(unsigned char*, const float*) { note("NozzleAxis"); }
void SetNozzleHprVar__20VIParticleAttributesRC7VIVect3(unsigned char*, const float*) { note("NozzleHprVar"); }
void SetGravityOn__20VIParticleAttributesi(unsigned char*, int on) { note("gravity" + std::to_string(on)); }
}
#include "particle_definition.cpp"
static int count(const char* what) { int n = 0; for (auto& e : log_) if (e == what) ++n; return n; }
int main() {
    alignas(8) unsigned char parser[64] = {};
    *reinterpret_cast<unsigned*>(parser + 32) = 0x500;
    *reinterpret_cast<unsigned*>(parser + 12) = 0x600;
    unsigned char object[8] = {};
    unsigned char definition[8] = {};
    // no texture, count 0 (one pass), version 1
    version = 1; readIndex = 0; findResult = 0;
    ints = {0, 2, 0, 5, 3, 0, 9};
    if (ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition(parser, object, definition) != 0) return 1;
    if (log_[0] != "RUi" || log_[2] != "blend2" || log_[4] != "zw0" || log_[6] != "zt1" || log_[8] != "cfg3") return 2;
    if (count("Friction15") != 1 || count("DeltaSpawn15") != 1 || count("motif") != 0 || count("name") != 0) return 3;
    if (count("Grad0") != 1 || count("Grad31") != 1 || count("StartColorVar") != 1 || count("EndColorVar") != 1 || count("GradRepeat") != 1) return 4;
    if (count("InnerOffset") != 1 || count("NozzleHprVar") != 1 || count("gravity9") != 1 || count("Rb12") != 6 || count("Rb16") != 34) return 5;
    if (log_.back() != "end" || count("find0,0") != 0) return 6;
    // texture found with type 1, two motifs (count 2), gravity absent for version 0
    log_.clear(); readIndex = 0; version = 0; findResult = 1; findType = 1; findId = 42; motifs = 0;
    ints = {77, 1, 1, 1, 0, 2};
    if (ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition(parser, object, definition) != 0) return 7;
    // 0x500 = 1280 is the dictionary word and 0x600 = 1536 the raster word
    if (count("find1280,77") != 1 || count("tex42,1536,1280") != 1) return 9;
    if (motifs != 2 || count("name") != 2 || count("gravity0") != 3 || count("Rb32") != 2) return 10;
    // a texture of another type aborts without ReadEnd
    log_.clear(); readIndex = 0; findType = 3; motifs = 0;
    ints = {77, 1, 1, 1, 0, 2};
    if (ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition(parser, object, definition) != -1) return 11;
    if (count("end") != 0 || motifs != 0 || count("Friction15") != 0) return 12;
    // negative count: no passes, still ReadEnd
    log_.clear(); readIndex = 0; findResult = 0;
    ints = {0, 1, 1, 1, 0, -3};
    if (ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition(parser, object, definition) != 0 || count("Friction15") != 0 || count("end") != 1) return 13;
    return 0;
}

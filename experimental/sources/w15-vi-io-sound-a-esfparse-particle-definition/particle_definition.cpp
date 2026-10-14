// Observed: ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition at 0x011790e8, 1148 bytes. Arguments: the parser
// (P), the object file (O) and a particle definition (D) to fill. Every Read__9VIObjFile call below ignores its result, so a short
// or failed read leaves the previous value in the same local and that value is used again.
// Setup: a 4-float colour and a 3-float vector are zeroed; version = ObjectVersion__9VIObjFile(O).
// Header, in file order: a word F (Read ...RUi); an int (Read ...Ri) given to SetBlendMode; an int given to SetZWrite as (value != 0);
// an int given to SetZTest as (value != 0); an int given to SetTextureConfiguration; an int N (the count).
// Texture: when F is nonzero and the word at P+32 (a dictionary, X) is nonzero, Find__12VIDictionaryUiR14VIResourceTypeRi(X, F,
// &type, &id) is called. If it succeeds and type is not 1 the whole function returns -1 (ReadEnd is not called). If it succeeds
// and type is 1, SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary(D, id, word at P+12 (a raster), X) is called. A
// zero result or a zero F/X skips the texture.
// Then N + 1 passes (the count is re-read each pass; none run when N + 1 is not positive). Pass 0 fills D itself; every later pass
// calls CreateMotif__22VIParticleDefinitionEx(D), reads a 32-byte name with Read__9VIObjFilePUci and calls
// SetName__15VIParticleMotifPCc, and fills the returned motif instead. Filling A (D or the motif) is, in order:
//   thirteen floats, each read then passed to its setter: Friction, Birthrate, BirthrateVar, Lifespan, LifespanVar, Velocity,
//   VelocityVar, StartSize, StartSizeVar, EndSize, EndSizeVar, InheritVelocity, DeltaSpawn;
//   a 16-byte read into the colour for SetStartColorVar, another for SetEndColorVar;
//   thirty-two 16-byte reads, each given to SetGradientColor with its index 0 to 31; a float for SetGradientRepeat;
//   six 12-byte reads into the vector for SetInnerOffset, SetInnerHprVar, SetOuterOffset, SetOuterHprVar, SetNozzleAxis,
//   SetNozzleHprVar;
//   if version is above zero an int is read for SetGravityOn, else SetGravityOn(A, 0).
// After the passes ReadEnd__9VIObjFile(O) is called and the result is 0.
// Inferred: the object is a particle-definition block: render state, an optional texture reference, then one set of emitter
// attributes for the definition and one more per named motif. Setter names follow the callees; their types are the mangled ones.
// Unknown: the member names, the field meanings beyond the setter names, what resource type 1 is, and whether the version gate
// on gravity marks an older file layout.
extern "C" {
unsigned ObjectVersion__9VIObjFile(unsigned char* object);
int Read__9VIObjFileRUi(unsigned char* object, unsigned* value);
int Read__9VIObjFileRi(unsigned char* object, int* value);
int Read__9VIObjFileRf(unsigned char* object, float* value);
int Read__9VIObjFilePUci(unsigned char* object, unsigned char* buffer, int count);
void ReadEnd__9VIObjFile(unsigned char* object);
void SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE(unsigned char* definition, int mode);
void SetZWrite__20VIParticleDefinitionb(unsigned char* definition, bool on);
void SetZTest__20VIParticleDefinitionb(unsigned char* definition, bool on);
void SetTextureConfiguration__20VIParticleDefinitioni(unsigned char* definition, int configuration);
void SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary(unsigned char* definition, int texture, unsigned raster, unsigned dictionary);
int Find__12VIDictionaryUiR14VIResourceTypeRi(unsigned dictionary, unsigned id, int* type, int* found);
unsigned char* CreateMotif__22VIParticleDefinitionEx(unsigned char* definition);
void SetName__15VIParticleMotifPCc(unsigned char* motif, const char* name);
void SetFriction__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetBirthrate__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetBirthrateVar__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetLifespan__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetLifespanVar__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetVelocity__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetVelocityVar__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetStartSize__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetStartSizeVar__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetEndSize__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetEndSizeVar__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetInheritVelocity__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetDeltaSpawn__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetStartColorVar__20VIParticleAttributesRC10VIColor32F(unsigned char* attributes, const float* color);
void SetEndColorVar__20VIParticleAttributesRC10VIColor32F(unsigned char* attributes, const float* color);
void SetGradientColor__20VIParticleAttributesiRC10VIColor32F(unsigned char* attributes, int index, const float* color);
void SetGradientRepeat__20VIParticleAttributesf(unsigned char* attributes, float value);
void SetInnerOffset__20VIParticleAttributesRC7VIVect3(unsigned char* attributes, const float* vector);
void SetInnerHprVar__20VIParticleAttributesRC7VIVect3(unsigned char* attributes, const float* vector);
void SetOuterOffset__20VIParticleAttributesRC7VIVect3(unsigned char* attributes, const float* vector);
void SetOuterHprVar__20VIParticleAttributesRC7VIVect3(unsigned char* attributes, const float* vector);
void SetNozzleAxis__20VIParticleAttributesRC7VIVect3(unsigned char* attributes, const float* vector);
void SetNozzleHprVar__20VIParticleAttributesRC7VIVect3(unsigned char* attributes, const float* vector);
void SetGravityOn__20VIParticleAttributesi(unsigned char* attributes, int on);

int ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition(unsigned char* parser, unsigned char* object, unsigned char* definition) {
    float scratch;
    int word;
    int count;
    unsigned flag;
    float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float vector[3] = {0.0f, 0.0f, 0.0f};
    char name[32];
    int type;
    int found;
    int version = static_cast<int>(ObjectVersion__9VIObjFile(object));
    Read__9VIObjFileRUi(object, &flag);
    Read__9VIObjFileRi(object, &word);
    SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE(definition, word);
    Read__9VIObjFileRi(object, &word);
    SetZWrite__20VIParticleDefinitionb(definition, word != 0);
    Read__9VIObjFileRi(object, &word);
    SetZTest__20VIParticleDefinitionb(definition, word != 0);
    Read__9VIObjFileRi(object, &word);
    SetTextureConfiguration__20VIParticleDefinitioni(definition, word);
    Read__9VIObjFileRi(object, &count);
    if (flag != 0) {
        unsigned dictionary = *reinterpret_cast<unsigned*>(parser + 32);
        if (dictionary != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, flag, &type, &found) != 0) {
            if (type != 1) return -1;
            SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary(definition, found, *reinterpret_cast<unsigned*>(parser + 12), *reinterpret_cast<unsigned*>(parser + 32));
        }
    }
    for (int pass = 0; pass < count + 1; ++pass) {
        unsigned char* attributes = definition;
        if (pass != 0) {
            attributes = CreateMotif__22VIParticleDefinitionEx(definition);
            Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(name), 32);
            SetName__15VIParticleMotifPCc(attributes, name);
        }
        Read__9VIObjFileRf(object, &scratch);
        SetFriction__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetBirthrate__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetBirthrateVar__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetLifespan__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetLifespanVar__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetVelocity__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetVelocityVar__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetStartSize__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetStartSizeVar__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetEndSize__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetEndSizeVar__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetInheritVelocity__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFileRf(object, &scratch);
        SetDeltaSpawn__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(color), 16);
        SetStartColorVar__20VIParticleAttributesRC10VIColor32F(attributes, color);
        Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(color), 16);
        SetEndColorVar__20VIParticleAttributesRC10VIColor32F(attributes, color);
        for (int index = 0; index < 32; ++index) {
            Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(color), 16);
            SetGradientColor__20VIParticleAttributesiRC10VIColor32F(attributes, index, color);
        }
        Read__9VIObjFileRf(object, &scratch);
        SetGradientRepeat__20VIParticleAttributesf(attributes, scratch);
        Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(vector), 12);
        SetInnerOffset__20VIParticleAttributesRC7VIVect3(attributes, vector);
        Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(vector), 12);
        SetInnerHprVar__20VIParticleAttributesRC7VIVect3(attributes, vector);
        Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(vector), 12);
        SetOuterOffset__20VIParticleAttributesRC7VIVect3(attributes, vector);
        Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(vector), 12);
        SetOuterHprVar__20VIParticleAttributesRC7VIVect3(attributes, vector);
        Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(vector), 12);
        SetNozzleAxis__20VIParticleAttributesRC7VIVect3(attributes, vector);
        Read__9VIObjFilePUci(object, reinterpret_cast<unsigned char*>(vector), 12);
        SetNozzleHprVar__20VIParticleAttributesRC7VIVect3(attributes, vector);
        if (version > 0) {
            Read__9VIObjFileRi(object, &word);
            SetGravityOn__20VIParticleAttributesi(attributes, word);
        } else {
            SetGravityOn__20VIParticleAttributesi(attributes, 0);
        }
    }
    ReadEnd__9VIObjFile(object);
    return 0;
}
}

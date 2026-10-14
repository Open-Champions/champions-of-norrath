// Observed: SetGravityOn__20VIParticleAttributesi is 36 bytes. A non-zero argument stores
// -5.0f (bits 0xc0a00000) at offset 720 of the attribute block; zero stores 0.0f there.
// It then adds one to the word at offset 716. The reader GravityOn__C20VIParticleAttributes
// (not authored here) returns 1 when the float at 720 is less than zero.
// Inferred: offset 720 is a gravity value where -5.0 is the "on" setting.
// Unknown: the unit of that value and what consumes the counter at 716.
extern "C" {
void SetGravityOn__20VIParticleAttributesi(unsigned char* attrs, int on) {
    *reinterpret_cast<float*>(attrs + 720) = on != 0 ? -5.0f : 0.0f;
    *reinterpret_cast<unsigned*>(attrs + 716) += 1;
}
}

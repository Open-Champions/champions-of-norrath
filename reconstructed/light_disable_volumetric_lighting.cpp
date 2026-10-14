// Sets the shared numeric lighting flag to one. Other state users and the
// original local translation-unit linkage are not reconstructed here.
typedef char unsigned_width[sizeof(unsigned) == 4 ? 1 : -1];
extern "C" unsigned norrath_disable_volumetric_lighting;
extern "C" void norrath_light_disable_volumetric_lighting()
{
    norrath_disable_volumetric_lighting = 1;
}

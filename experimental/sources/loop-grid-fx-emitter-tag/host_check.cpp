// Host contract for norrath_factory_particle_emitter_tag_bytes. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_factory_particle_emitter_tag_bytes[];

int main() {
    if (std::memcmp(norrath_factory_particle_emitter_tag_bytes, "ParticleEmitter", 16) != 0) {
        std::fprintf(stderr, "fail: bytes differ\n");
        return 1;
    }
    norrath_factory_particle_emitter_tag_bytes[0] = 'p';   // writable, as the lowercase pass requires
    if (norrath_factory_particle_emitter_tag_bytes[0] != 'p') return 2;
    std::printf("emitter-tag contract passed\n");
    return 0;
}

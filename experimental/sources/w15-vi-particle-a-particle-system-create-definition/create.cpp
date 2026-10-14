// Observed: CreateParticleDefinition__16VIParticleSystemi at 0x010b7f30 is 236 bytes. Arguments: the system (S) and a share flag.
// It allocates 784 bytes with __builtin_new. If that returns non-null it constructs a VIParticleDefinitionEx in it
// (__22VIParticleDefinitionEx(D), one argument) and stores into the new object: zero at +748, +752, +764, +768 and +772; one at +756 and
// +760; and -1 at +776 and +780. A null allocation returns -1 without calling anything else. Otherwise it calls
// Create__20VIParticleDefinitionR8VIRaster(D, the word at S+0). A negative result destroys the object again
// (_$_22VIParticleDefinitionEx(D, 0) then __builtin_delete(D)) and returns -1. A non-negative result increments the word at D+768 when
// the share flag is non-zero, and the routine returns the value of
// Add__t6VIPool1ZP20VIParticleDefinitionRCP20VIParticleDefinition(S + 84, &D), which is passed a reference to a stack slot holding D.
// Inferred: S+0 is the raster, S+84 the pool of definition pointers; D+768 is the reference count that ShareParticleDefinition
// increments (+764 starts a two-word pair); the return value is the pool index.
// Unknown: the meaning of the other stored words.
extern "C" {
void* __builtin_new(unsigned size);
void __builtin_delete(void* object);
void __22VIParticleDefinitionEx(void* object);
void _$_22VIParticleDefinitionEx(void* object, int flags);
int Create__20VIParticleDefinitionR8VIRaster(void* definition, void* raster);
int Add__t6VIPool1ZP20VIParticleDefinitionRCP20VIParticleDefinition(void* pool, void* const* element);

#ifdef NORRATH_HOST_ARENA
void* norrath_host_pointer(unsigned address);
#define POINTER_OF(word) norrath_host_pointer(word)
#else
#define POINTER_OF(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

int CreateParticleDefinition__16VIParticleSystemi(unsigned char* system, int share) {
    unsigned char* definition = static_cast<unsigned char*>(__builtin_new(784));
    if (definition != 0) {
        __22VIParticleDefinitionEx(definition);
        *reinterpret_cast<unsigned*>(definition + 764) = 0;
        *reinterpret_cast<unsigned*>(definition + 768) = 0;
        *reinterpret_cast<unsigned*>(definition + 748) = 0;
        *reinterpret_cast<int*>(definition + 776) = -1;
        *reinterpret_cast<unsigned*>(definition + 760) = 1;
        *reinterpret_cast<unsigned*>(definition + 752) = 0;
        *reinterpret_cast<unsigned*>(definition + 756) = 1;
        *reinterpret_cast<int*>(definition + 780) = -1;
        *reinterpret_cast<unsigned*>(definition + 772) = 0;
    }
    if (definition == 0) return -1;
    if (Create__20VIParticleDefinitionR8VIRaster(definition, POINTER_OF(*reinterpret_cast<unsigned*>(system))) < 0) {
        if (definition != 0) {
            _$_22VIParticleDefinitionEx(definition, 0);
            __builtin_delete(definition);
        }
        return -1;
    }
    if (share != 0) *reinterpret_cast<int*>(definition + 768) += 1;
    return Add__t6VIPool1ZP20VIParticleDefinitionRCP20VIParticleDefinition(system + 84, reinterpret_cast<void* const*>(&definition));
}
}

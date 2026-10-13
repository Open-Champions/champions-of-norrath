// Observed msg_run__6Badger at 0x001ef2e0, 1988 bytes. Offsets are into the object; M is the word
// at +12 of the structure pointed to by the 32-bit pointer at +36 (the model, from the callees'
// declared parameter types); h is the signed halfword at +56, y the float at +28, S the signed
// halfword at +572 and A the result of creatureAdvanceAnimation. setState(n) is the object's
// virtual function at vtable byte offset 184 called with n. All Creature names below are the
// callees' own. Every pass:
//   1. newDamageHandler(object, M); when everyFrame(object, M) returns nonzero, return;
//   2. when objectActivationCheck(object, 600) is zero, return;
//   3. A = creatureAdvanceAnimation(object, 1.0); slideBack(object, 0.2); the object's virtual
//      function at vtable byte offset 168 is called with no arguments;
//   4. S above 16 (unsigned compare) stops here; 2, 4, 7, 11 and 12 do nothing more. Otherwise:
//   S = 0: enemyInZone(400.0, 32767) or enemyInZone(500.0, 16000) true: setState(3); else, when the
//      word at +256 is below the word at +808 (signed): setState(1).
//   S = 1: the same two tests true: setState(0); else smartWander.
//   S = 3: when the word at +576 is zero: setState(0). Otherwise the first test that succeeds
//      ends the pass, in this order (Z2 = enemyInZone2(f, g, angle, 3000, 0.0, 0), AZ =
//      alternateEnemyInZone(0.0, y + 25, angle, 3000)):
//        Z2(0, 60, h): a roll (eRand % 5, low 16 bits) picks 14 (0), 15 (1) or 6 (other);
//        Z2(y + 25, 100, h): 16; Z2(0, 60, h + 8000): 14; Z2(0, 60, h - 8000): 15;
//        AZ(h): the same roll; AZ(h + 8000): 14; AZ(h - 8000): 15;
//      each picked value is passed to setState. If none succeeds: enemyInZone(y + 20, 32767)
//      then turnToFaceEnemy, else steerAndRouteToEnemy.
//   S = 5: when the word at +576 is zero: bExitStatePersueIdle nonzero -> setState(3). Otherwise
//      Z2(0, y + 25, h) true or AZ(h) true: setState(6); else bExitStatePersueIdle nonzero ->
//      setState(3).
//   S = 6, 13, 14, 15, 16: move2(object, a zero Point3). When bit 0 of A is set: the damage flags
//      F are 8, unless the signed byte at +964 is not 1 and S is 6, 16 or 13, when F is 0x40008
//      for a nonzero (eRand % 3) and 0x80008 for zero; produceGeneralDamage(object, 0.0, y + 25, h,
//      3000, halfword +200, +202, +204, F, null) and the word at +960 = 1. When the word at +576
//      is nonzero, bit 3 of the halfword at +294 is set and enemyInZone(y + 10, 3000): S = 6 (with
//      the word at +960 zero), then S = 16, then S = 13 (each re-reading S) call turnToFaceEnemy.
//      When bit 1 of A is set and S is not 6 and Z2(0, 60, h): setState(6). Finally, when bit 3
//      of the halfword at +294 is clear: setState(3).
//   S = 8: setState(3) unless bit 4 of the halfword at +294 is set.
//   S = 9: setState(3) when the word at +804 is below the word at +808 (signed).
//   S = 10: when bit 7 of the halfword at +294 is set, objectAddToSlowRunList; when bit 2 of A is
//      set, bit 0 of the word at +12 is set; when bit 0 of A is set: floorAdd(copy of the +16 point,
//      (short) of one roll, (35 + a second roll % 10) as a float, 0, 1).
// Returns nothing. Unknown: what the zones, the bits of A and the numeric states mean.
// The host check defines NORRATH_W10AC_HOST so that 32-bit address words map to host memory.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float step);
void slideBack__8Creaturef(void* self, float amount);
void move2__8CreatureG6Point3(void* self, const void* movement);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
int enemyInZone__8Creaturefs(void* self, float range, short zone);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short angle, short zone, float c, int d);
int alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short angle, short zone);
int produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void* self, float a, float b, short angle,
                                                                   short zone, int p, int q, int r, int flags,
                                                                   void* description);
void objectAddToSlowRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const void* position, short angle, float size, int a, int b);
int eRand__10GameObject(void* object);
#if defined(NORRATH_W10AC_HOST)
extern unsigned char* norrath_w10ac_host_base;
void* norrath_w10ac_host_function(unsigned word);
#endif
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
short read_half(const unsigned char* p) { return static_cast<short>(p[0] | (p[1] << 8)); }
unsigned read_uhalf(const unsigned char* p) { return static_cast<unsigned>(p[0] | (p[1] << 8)); }
float read_float(const unsigned char* p) {
    union { unsigned u; float f; } v;
    v.u = read_word(p);
    return v.f;
}
#if defined(NORRATH_W10AC_HOST)
// Host check only: pointer words are offsets from a base chosen by the check.
unsigned char* address_of(unsigned word) { return norrath_w10ac_host_base + word; }
void* function_of(unsigned word) { return norrath_w10ac_host_function(word); }
#else
unsigned char* address_of(unsigned word) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
void* function_of(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
#endif
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
const unsigned char* slot_entry(unsigned char* target, unsigned byte_offset) {
    return address_of(read_word(target + 72u)) + byte_offset;
}
void call_slot(unsigned char* target, unsigned byte_offset) {
    const unsigned char* entry = slot_entry(target, byte_offset);
    typedef void (*Function)(void*);
    reinterpret_cast<Function>(function_of(read_word(entry + 4)))(target + read_half(entry));
}
void set_state(unsigned char* target, int state) {
    const unsigned char* entry = slot_entry(target, 184);
    typedef void (*Function)(void*, int);
    reinterpret_cast<Function>(function_of(read_word(entry + 4)))(target + read_half(entry), state);
}
// A roll (eRand % 5, low 16 bits) picks one of three states.
void set_rolled_state(unsigned char* object) {
    unsigned pick = (static_cast<unsigned>(eRand__10GameObject(object)) % 5u) & 0xffffu;
    set_state(object, pick == 0u ? 14 : (pick == 1u ? 15 : 6));
}
}

extern "C" void msg_run__6Badger(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    unsigned char* shared = address_of(read_word(object + 36u));
    newDamageHandler__8CreatureP12_modelHeader(object, address_of(read_word(shared + 12u)));
    shared = address_of(read_word(object + 36u));
    if (everyFrame__8CreatureP12_modelHeader(object, address_of(read_word(shared + 12u))) != 0) return;
    if (objectActivationCheck__8Creaturei(object, 600) == 0) return;
    int advance = creatureAdvanceAnimation__8Creaturef(object, 1.0f);
    slideBack__8Creaturef(object, 0.2f);
    call_slot(object, 168);
    short state = read_half(object + 572u);
    if (static_cast<unsigned>(static_cast<int>(state)) >= 17u) return;
    switch (state) {
    case 0:
        if (enemyInZone__8Creaturefs(object, 400.0f, 32767) || enemyInZone__8Creaturefs(object, 500.0f, 16000)) {
            set_state(object, 3);
        } else if (static_cast<int>(read_word(object + 256u)) < static_cast<int>(read_word(object + 808u))) {
            set_state(object, 1);
        }
        break;
    case 1:
        if (enemyInZone__8Creaturefs(object, 400.0f, 32767) || enemyInZone__8Creaturefs(object, 500.0f, 16000))
            set_state(object, 0);
        else
            smartWander__8Creature(object);
        break;
    case 3: {
        if (read_word(object + 576u) == 0u) {
            set_state(object, 0);
            break;
        }
        if (enemyInZone2__8Creatureffssfi(object, 0.0f, 60.0f, read_half(object + 56u), 3000, 0.0f, 0)) {
            set_rolled_state(object);
            break;
        }
        if (enemyInZone2__8Creatureffssfi(object, read_float(object + 28u) + 25.0f, 100.0f, read_half(object + 56u),
                                          3000, 0.0f, 0)) {
            set_state(object, 16);
            break;
        }
        if (enemyInZone2__8Creatureffssfi(object, 0.0f, 60.0f, static_cast<short>(read_uhalf(object + 56u) + 8000u),
                                          3000, 0.0f, 0)) {
            set_state(object, 14);
            break;
        }
        if (enemyInZone2__8Creatureffssfi(object, 0.0f, 60.0f, static_cast<short>(read_uhalf(object + 56u) - 8000u),
                                          3000, 0.0f, 0)) {
            set_state(object, 15);
            break;
        }
        if (alternateEnemyInZone__8Creatureffss(object, 0.0f, read_float(object + 28u) + 25.0f,
                                                read_half(object + 56u), 3000)) {
            set_rolled_state(object);
            break;
        }
        if (alternateEnemyInZone__8Creatureffss(object, 0.0f, read_float(object + 28u) + 25.0f,
                                                static_cast<short>(read_uhalf(object + 56u) + 8000u), 3000)) {
            set_state(object, 14);
            break;
        }
        if (alternateEnemyInZone__8Creatureffss(object, 0.0f, read_float(object + 28u) + 25.0f,
                                                static_cast<short>(read_uhalf(object + 56u) - 8000u), 3000)) {
            set_state(object, 15);
            break;
        }
        if (enemyInZone__8Creaturefs(object, read_float(object + 28u) + 20.0f, 32767))
            turnToFaceEnemy__8Creature(object);
        else
            steerAndRouteToEnemy__8Creature(object);
        break;
    }
    case 5:
        if (read_word(object + 576u) == 0u) {
            if (bExitStatePersueIdle__8Creature(object) != 0) set_state(object, 3);
            break;
        }
        if (enemyInZone2__8Creatureffssfi(object, 0.0f, read_float(object + 28u) + 25.0f, read_half(object + 56u),
                                          3000, 0.0f, 0)) {
            set_state(object, 6);
            break;
        }
        if (alternateEnemyInZone__8Creatureffss(object, 0.0f, read_float(object + 28u) + 25.0f,
                                                read_half(object + 56u), 3000)) {
            set_state(object, 6);
            break;
        }
        if (bExitStatePersueIdle__8Creature(object) != 0) set_state(object, 3);
        break;
    case 6:
    case 13:
    case 14:
    case 15:
    case 16: {
        float stillness[3] = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(object, stillness);
        if ((advance & 1) != 0) {
            int flags = 8;
            if (static_cast<signed char>(object[964]) != 1) {
                short current = read_half(object + 572u);
                if (current == 6 || current == 16 || current == 13)
                    flags = (static_cast<unsigned>(eRand__10GameObject(object)) % 3u) != 0u ? 0x40008 : 0x80008;
            }
            produceGeneralDamage__8CreatureffssiiiiPQ24item11description(
                object, 0.0f, read_float(object + 28u) + 25.0f, read_half(object + 56u), 3000, read_half(object + 200u),
                read_half(object + 202u), read_half(object + 204u), flags, nullptr);
            write_word(object + 960u, 1u);
        }
        if (read_word(object + 576u) != 0u) {
            if (read_half(object + 572u) == 6 && read_word(object + 960u) == 0u && (read_uhalf(object + 294u) & 8u) != 0u &&
                enemyInZone__8Creaturefs(object, read_float(object + 28u) + 10.0f, 3000))
                turnToFaceEnemy__8Creature(object);
            if (read_half(object + 572u) == 16 && (read_uhalf(object + 294u) & 8u) != 0u &&
                enemyInZone__8Creaturefs(object, read_float(object + 28u) + 10.0f, 3000))
                turnToFaceEnemy__8Creature(object);
            if (read_half(object + 572u) == 13 && (read_uhalf(object + 294u) & 8u) != 0u &&
                enemyInZone__8Creaturefs(object, read_float(object + 28u) + 10.0f, 3000))
                turnToFaceEnemy__8Creature(object);
        }
        if ((advance & 2) != 0 && read_half(object + 572u) != 6 &&
            enemyInZone2__8Creatureffssfi(object, 0.0f, 60.0f, read_half(object + 56u), 3000, 0.0f, 0))
            set_state(object, 6);
        if ((read_uhalf(object + 294u) & 8u) == 0u) set_state(object, 3);
        break;
    }
    case 8:
        if ((read_uhalf(object + 294u) & 0x10u) == 0u) set_state(object, 3);
        break;
    case 9:
        if (static_cast<int>(read_word(object + 804u)) < static_cast<int>(read_word(object + 808u))) set_state(object, 3);
        break;
    case 10: {
        if ((read_uhalf(object + 294u) & 0x80u) != 0u) objectAddToSlowRunList__FP10GameObject(object);
        if ((advance & 4) != 0) write_word(object + 12u, read_word(object + 12u) | 1u);
        if ((advance & 1) != 0) {
            unsigned char position[12];
            copy_bytes(position, object + 16u, 12u);
            int first = eRand__10GameObject(object);
            int second = eRand__10GameObject(object);
            floorAdd__FG6Point3sfii(position, static_cast<short>(first),
                                    static_cast<float>(static_cast<int>(static_cast<unsigned>(second) % 10u) + 35), 0, 1);
        }
        break;
    }
    default:
        break;
    }
}

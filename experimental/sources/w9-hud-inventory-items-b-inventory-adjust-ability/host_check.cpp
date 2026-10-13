#include <cstring>
extern "C" {
unsigned char player[2 * 23984];
extern const unsigned char norrath_inventory_ability_sound_lump_view[8] = {'f','x','.','l','m','p',0,0};
extern const unsigned char norrath_inventory_ability_sound_resource_view[19] = {'m','e','n','u','_','c','h','a','r','_','i','n','c','r','.','a','n','m',0};
void inventoryAdjustAbility__Fii(int, int);
}

static int local_flag, local_arg, find_calls, play_calls, recompute_calls, order[8], norder;
static char *seen_lump, *seen_name;
static void* seen_header; static void* seen_obj; static float seen_rate;
static int sound_token;

extern "C" int netPlayerIsLocal__Fi(int p) { local_arg = p; order[norder++] = 1; return local_flag; }
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { ++find_calls; seen_lump = l; seen_name = n; order[norder++] = 2; return &sound_token; }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float r) { ++play_calls; seen_header = h; seen_obj = o; seen_rate = r; order[norder++] = 3; return 0; }
extern "C" void playerRecomputeAbilityModifiers__Fv() { ++recompute_calls; order[norder++] = 4; }

static void reset() { memset(player, 0, sizeof player); local_flag = local_arg = find_calls = play_calls = recompute_calls = norder = 0; }
static int& ability(int p, int a) { return *reinterpret_cast<int*>(player + p * 23984 + 120 + 4 * a); }

int main() {
    reset();
    player[23984 + 240] = 3; ability(1, 5) = 10; ability(1, 4) = 7; ability(1, 6) = 9;
    local_flag = 1;
    inventoryAdjustAbility__Fii(1, 5);
    if (ability(1, 5) != 11 || ability(1, 4) != 7 || ability(1, 6) != 9) return 1;
    if (player[23984 + 240] != 2 || player[23984 + 242] != 0) return 2;
    if (local_arg != 1 || find_calls != 1 || play_calls != 1 || recompute_calls != 1) return 3;
    if (strcmp(seen_lump, "fx.lmp") != 0 || strcmp(seen_name, "menu_char_incr.anm") != 0) return 4;
    if (seen_header != &sound_token || seen_obj != nullptr || seen_rate != 1.0f) return 5;
    if (norder != 4 || order[0] != 1 || order[1] != 2 || order[2] != 3 || order[3] != 4) return 6;
    if (player[240] != 0 || player[242] != 0) return 7;

    // last point: byte reaches zero, flag at +242 set
    reset(); local_flag = 1;
    player[240] = 1;
    inventoryAdjustAbility__Fii(0, 0);
    if (ability(0, 0) != 1 || player[240] != 0 || player[242] != 1) return 8;

    // nothing to spend: no change to the ability, but sound (if local), flag and recompute still happen
    reset(); local_flag = 1;
    ability(0, 2) = 4;
    inventoryAdjustAbility__Fii(0, 2);
    if (ability(0, 2) != 4 || player[240] != 0 || player[242] != 1) return 9;
    if (play_calls != 1 || recompute_calls != 1) return 10;

    // non-local player: no lookup, no sound
    reset(); local_flag = 0;
    player[240] = 2;
    inventoryAdjustAbility__Fii(0, 1);
    if (find_calls != 0 || play_calls != 0 || recompute_calls != 1 || ability(0, 1) != 1 || player[240] != 1 || player[242] != 0) return 11;
    return 0;
}

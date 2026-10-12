#include "add_spell.cpp"
static int calls;
static void* seen[3];
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value) {
    ++calls;
    seen[0] = head;
    seen[1] = object_head;
    seen[2] = value;
    return 0;
}
int main() {
    unsigned char creature[1024] = {0};
    unsigned char spell[104] = {0};
    addSpell__8CreatureP5Spell(creature, 0);
    if (calls != 0) return 1;
    addSpell__8CreatureP5Spell(creature, spell);
    if (calls != 1) return 2;
    if (seen[0] != creature + 888) return 3;
    if (seen[1] != spell + 4) return 4;
    if (seen[2] != spell) return 5;
    return 0;
}

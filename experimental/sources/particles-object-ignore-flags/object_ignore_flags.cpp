// objectIgnoreFlags at 0x00483ac0, four .sdata bytes, next objectKillingEveryone.
// The file word is zero. drawObjectsTask passes it to the list iterator.
// The reviewed -G0 profile may place this zero word in BSS.
extern "C" {
unsigned objectIgnoreFlags;
}
static_assert(sizeof objectIgnoreFlags == 4u, "objectIgnoreFlags width");

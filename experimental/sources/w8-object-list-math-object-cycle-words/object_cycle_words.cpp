// Three zero-initialised int words in the object module's small data, each 4
// bytes by the distance to the next symbol:
//   objectKillingEveryone 0x483ac4  set to 1 while objectClear deletes every object
//   slowListCycle         0x483ad0  next slow run-list head to use
//   mediumListCycle       0x483ad4  next medium run-list head to use
// The file image holds zero for all three. Observed users in this package:
// objectClear writes all three. Other authored sources keep their own views of
// the cycle words (norrath_medium_list_cycle_word and an actor-package view of
// 0x483ad0); whether those are the same symbols is for the integrator.
extern "C" {
int objectKillingEveryone;
int slowListCycle;
int mediumListCycle;
}

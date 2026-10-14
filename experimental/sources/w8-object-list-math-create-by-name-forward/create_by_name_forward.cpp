// createByName__FPcG6Point3sPPc: the name the dialog start-up code passes to
// objectSetCreateByNameFunction. Observed in the private image at 0x14c410,
// extent 15600: a class-name dispatch that builds one object per name and
// returns it (or runs a side effect and returns null). That body is already
// authored, with the same four-argument interface, as norrath_game_create_by_name
// (game-object-factory-build-v2; it cites selection points inside 0x14c410).
// Repeating its roughly 100 branches here would open about as many constructor
// imports under different names, so this unit only supplies the original symbol
// and forwards its arguments and result unchanged. THIS IS NOT A
// RECONSTRUCTION OF THE BODY and earns no reconstruction credit.
// Inferred: the two are the same function. Unknown: whether the integrator wants
// one symbol for both.
#include "../w8-object-list-math-common/object_types.hpp"

extern "C" unsigned char* norrath_game_create_by_name(char* name, Point3 position, short type, char** arguments);

extern "C" unsigned char* createByName__FPcG6Point3sPPc(char* name, Point3 position, short type, char** arguments) {
    return norrath_game_create_by_name(name, position, type, arguments);
}

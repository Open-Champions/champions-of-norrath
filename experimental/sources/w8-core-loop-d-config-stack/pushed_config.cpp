// Observed pushedConfig, 304 BSS bytes at 0x007d0b70 (a local symbol in the
// image). It is zero storage until gamePushConfig__Fv writes it.
#include "pushed_config.hpp"
extern "C" {
PushedConfig pushedConfig = {};
}

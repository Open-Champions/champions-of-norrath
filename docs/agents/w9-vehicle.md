# Vehicle, boat and tractor (w9-vehicle)

Branch `agents/w9-vehicle`. Private reference: `work/private/SLUS_205.65` of the
main checkout; addresses below are that file's symbol values. No game bytes or
disassembly listings are committed. Every function was compiled with the
reviewed EE g++ `authored_cpp` flags and has a host contract
(`host_check.cpp`, not an EE input) that returns non-zero when a store, call
order or result is wrong. Each function was also run against the original
image in a private R5900 interpreter (outside git) from identical memory
snapshots with identical stub callees, comparing call logs, argument words and
every changed byte; instruction coverage of the original was recorded and the
only uncovered instructions are alignment nops or the NaN branch noted below.
Compiled bodies are not the same size as the originals; these are behaviour
reconstructions, not byte matches. Nothing here has run on the EE or as
gameplay.

Toolchain lesson: decimal float literals in this compiler can be off by one
unit in the last place (a literal for 0x3f8ccccc compiled to ...cb). Floats
with fractions are therefore written as hexadecimal float literals.

## Functions (units under `experimental/sources/w9-vehicle-*`)

See the table at the end of this file.

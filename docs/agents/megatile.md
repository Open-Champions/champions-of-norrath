# Megatile agent notes

Package: world megatile system. Branch `agents/megatile`. Private executable
`work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
GP is `0x004879f0`.

The partial-build unresolved `norrath_megatile_*` set used here is the 23 names
from `work/initsys-build-InitTimer/unresolved-imports.json` (2026-10-11). It
matches the portable expanded build.

## specialparse

Observed `specialparse__FPPciiG6Point3Pc` at `0x00268928`, size `0x630`.
Host check: `c++ -std=c++17 -O2 -fwrapv -Wall -Wextra -Werror -DNORRATH_MEGATILE_HOST experimental/sources/megatile-specialparse/specialparse.cpp experimental/sources/megatile-specialparse/host_test.cpp`.
EE check uses the reviewed `authored_cpp` flags and emits only
`norrath_megatile_specialparse`.

This section is updated as later symbols land. The closing summary is at the end of the file.

# w11-data-a: particle, resource-map and item data (1 of 3)

Branch `agents/w11-data-a`. Private executable `work/private/SLUS_205.65`; addresses and extents
are from its symbol table (size-0 BSS symbols take the gap to the next symbol). Nothing here ran on
hardware or inside the game. Fresh EE objects are not original-byte matches.

## Checks performed

- Every unit compiles alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0, `-G0 -O2`).
- Every unit has a `host_check.cpp` built with the host compiler; all 29 exit 0.
  `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).
- Private paired runs (R5900 interpreter outside git, original and authored function from one
  memory snapshot with identical stub callees, compared on call log, return value and every changed
  byte outside the stack): `P_CreateParticleSystem__Fv` 12 scenarios, `drawWorldParticleEmitter__FP13PolysetHeader`
  16 scenarios, `drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25` 120
  scenarios; all 0 differences. For `drawParticleEmitter` 447 of its 452 words were reached (the
  rest are nops) and six deliberate source mutations (comparison, flip, spread constant, z offset sign, matrix
  angle) were each detected.
- The data tables were compared privately: the compiled `.data` of each item table matches the image
  on every non-pointer word and on pointer nullness.

## Functions

- `P_CreateParticleSystem__Fv` (0x00180260, 212): observed, finds the first of 16 `PS` records
  (312 bytes) whose word at +304 is zero, stores `(psSequenceNm << 8) | index`, bumps the sequence,
  makes the 48 bytes at +256 an identity matrix, clears +308 and returns the id (0 if none is free).
- `drawWorldParticleEmitter__FP13PolysetHeader` (0x0017c930, 268): builds the origin from three header
  words times 0.25, calls `drawParticleEmitter` and writes the schedule word and the top-bit state back.
- `drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25` (0x00186b58, 1808):
  observed: burst size (signed byte +17), an alternating two-interval schedule in +44/+48/+56/+60
  using `eRandState`, and per particle three random words for a position offset, one for velocity
  spread, two for a velocity turn, then `P_AddParticle`. The full byte-offset description is in the
  source header. Inferred: the names flipA/flipB/swapXY for the three trailing booleans, and
  that the two intervals are "on" and "off" times. Unknown: the real field names of the emitter data.

## Data

| Name | Address | Extent | Content |
|---|---|---|---|
| `_4item$itemTable` | 0x0032b320 | 27456 | 429 rows of 64 bytes, last name word zero |
| `_4item$namedItemTable` | 0x0031fbe0 | 46912 | 733 rows of 64 bytes, last name word zero |
| `_4item$prefixTable` | 0x00331e60 | 2320 | 58 rows of 40 bytes, last row zero; words 0, 3 and 4 are string pointers |
| `_4item$suffixTable` | 0x00332770 | 1820 | 91 rows of 20 bytes, last row zero; words 0, 2, 3 and 4 are string pointers |
| `_4item$undEnable` | 0x00483220 | 4 | zero word |
| `_6Wraith$s_wraithCount` | 0x004852a8 | 4 | zero word |
| twelve particle objects (`ballParticle` ... `jumpParticleOn`) and `ant_msg_run_particle_7d7170` | see sources | 80 each | zero storage; filled by a constructor at run time |
| `femBarbResourceMap`, `femaleDarkElfResourceMap`, `femaleEruditeResourceMap`, `femaleHighElfResourceMap`, `femaleWoodElfResourceMap`, `innoruukResourceMap` | see sources | pointer + zero word slots | file name tables, null last slot |
| `chestResourceList` | see source | | pointers to the existing chest map views |

The item and named-item units include the `rows.hpp` data of the items-player view units
(`items-player-item-table`, `items-player-named-item-table`) instead of keeping a second copy. The
prefix and suffix words other than strings keep their observed bits; their meaning is unknown
(`item::getName` only reads the key string). Pointer identity of strings shared between tables is not
preserved.

## Imports closed

P_CreateParticleSystem__Fv, _4item$itemTable, _4item$namedItemTable, _4item$prefixTable,
_4item$suffixTable, _4item$undEnable, _6Wraith$s_wraithCount, ant_msg_run_particle_7d7170,
ballParticle, ballSmokeParticle, ballSparkParticle, cFireParticle, cSmokeParticle, candle2Particle,
chestResourceList, drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25,
drawWorldParticleEmitter__FP13PolysetHeader, ectopileParticle1, femBarbResourceMap,
femaleDarkElfResourceMap, femaleEruditeResourceMap, femaleHighElfResourceMap,
femaleWoodElfResourceMap, innoruukParticle2, innoruukParticle3, innoruukResourceMap, jumpParticle2,
jumpParticleJump, jumpParticleOn.

## Imports opened (left unresolved, other packages)

- `drawParticleEmitter`: `matIdent__FP8Matrix34`, `matRot__FP8Matrix34is`,
  `matTransform__FP6Point3P8Matrix34T0`, `icos__Fi`, `isin__Fi`,
  `P_AddParticle__FP11ParticleDefP6Point3T1f`, `eRandState`.
- `P_CreateParticleSystem`: `PS`, `psSequenceNm`, `matIdent__FP8Matrix34`.
- `drawWorldParticleEmitter`: `ParticleEmitterArray`, `frameNm`.
- `chestResourceList`: eight address-named views `norrath_ctors_ant_lantern_view_42c760` ...
  `_42c878` from the ctors-ant-lantern package.

## Build-plan entries needed

`docs/agents/w11-data-a-plan-entries.json` holds all 29 entries (source, sha256, profile,
header closure, emitted functions) in the plan's entry format. The two big item tables need their
`header_closure` (already filled in) to include the items-player `rows.hpp` files. If the integrator
selects these two units, the `norrath_item_table_view` / `norrath_named_item_table_view` units of
items-player would duplicate the same bytes under other names and can be dropped from the plan if
no source still imports the view names.

## Authored but never executed

All of it. The three functions were run only inside the private interpreter against stubbed callees;
no unit has been linked into a running game. The data objects are the static image only: the
particle objects stay zero until a constructor outside this package fills them.

## Unknown

- The meaning of most of the 15 non-name words per item row and of prefix/suffix words other than the key strings.
- Which of the duplicate local `ballParticle` BSS symbols (0x007d6010 and 0x008d6320) importers mean;
  one global 80-byte object is defined.
- The field layout of `ParticleDef` and the real names of the `ParticleEmitterData` fields.

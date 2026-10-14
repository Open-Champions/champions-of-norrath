# w14-objfile: object file reader/writer, string table, resource containers and list primitives

Branch `agents/w14-objfile`. One commit per function. Source is under `experimental/sources/w14-objfile-*`; each directory holds the
authored source and a `host_check.cpp` (not a plan input). Addresses and sizes come from the pinned private `SLUS_205.65`. Nothing here
ran on the EE or as gameplay; compiled sizes are the reviewed EE `authored_cpp` objects and are not claims of byte identity.

## How each function was checked

* Every unit compiles with the reviewed EE g++ and the `authored_cpp` flags (36 objects; `docs/agents/w14-objfile-plan-entries.py` prints
  the plan entries from a fresh compile of each).
* Every unit has a native host check (`c++ -std=c++17 -Wall -Wextra -Werror`, exit 0) with stubbed callees. Functions that keep 32-bit
  addresses in words use an address arena (`NORRATH_HOST_ARENA`) so the checks run on a 64-bit host.
* Every unit was compared with the image in a private paired run (an R5900 interpreter outside git): the original function and the freshly
  compiled authored function run from identical memory snapshots with identical stubbed callees; call logs, return values where
  they are defined, and every changed byte outside the stack must be equal. Mutations (a changed constant, offset, branch sense, call order
  or dropped store) were run for every unit and each was detected by the paired run. Every paired run was grepped for `HALT`. For the multimap `DestroyPool`, two mutations changed the result and two made the walk non-terminating (stopped by a timeout), so those two are weaker evidence.
* Two defects were found by the paired run and corrected before commit: `LoadRawFile__13VIStringTablePCcRt7VIArray1Zc` does not receive the
  table (it is a static taking the name and the array), and the left child of the erased map node is read from the erased node, not the
  replacement (visible only in aliased trees).
* A host check found one wrong guess: the free branch of the flat-file-structure constructor is reachable only if the list constructor
  stores into word 0, because word 0 is cleared first.
* The commit message of `Add VIStringTable load.` quotes a wrong second address (`0x01017d78 region`); the function is at 0x0101bd78.

## Units

| Function | Image address | Image bytes (symbol size) | Compiled bytes |
|---|---|---|---|
| `__19VIFlatFileStructure` | 0x01004fe8 | 132 | 144 |
| `_$_19VIFlatFileStructure` | 0x01005070 | 84 | 88 |
| `strlen16__8VIStringPCUs` | 0x0101a998 | 52 | 52 |
| `str16to8__8VIStringPcUiPCUs` | 0x0101ac48 | 224 | 200 |
| `__13VIStringTable` | 0x0101bb78 | 20 | 20 |
| `_$_13VIStringTable` | 0x0101bb90 | 84 | 76 |
| `Unload__13VIStringTable` | 0x0101bbe8 | 88 | 128 |
| `Load__13VIStringTablePCc` | 0x0101bd78 | 192 | 208 |
| `__7VILNode` | 0x01072670 | 12 | 12 |
| `__7VILList` | 0x01072788 | 20 | 8 |
| `Init__7VILList` | 0x010727d0 | 20 | 20 |
| `AddTail__7VILListP7VILNode` | 0x01072810 | 36 | 40 |
| `WriteFileHeader__9VIObjFilei` | 0x010a52e0 | 308 | 368 |
| `ReadFileHeader__9VIObjFilei` | 0x010a5418 | 216 | 256 |
| `ReadEnd__9VIObjFile` | 0x010a5ab0 | 196 | 312 |
| `AbortOpen__9VIObjFile` | 0x010a6468 | 60 | 56 |
| `Flush__9VIObjFile` | 0x010a6550 | 88 | 112 |
| `WriteEnd__9VIObjFile` | 0x010a66d8 | 28 | 28 |
| `ReadBegin__9VIObjFileRUsT1` | 0x010a6da8 | 32 | 32 |
| `Find__12VIDictionaryUiR14VIResourceTypeRi` | 0x010bf2e0 | 144 | 160 |
| `DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry` | 0x011f4b80 | 132 | 364 |
| `_$_t7VIArray1Z14VIResourceElem` | 0x012067d8 | 116 | 148 |
| `__t7VIArray1Z14VIResourceElem` | 0x012069e8 | 12 | 12 |
| `Erase__t6VIPool1ZP20VIParticleDefinitioni` | 0x01208c10 | 228 | 232 |
| `Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi` | 0x01209b80 | 512 | 584 |
| `Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi` | 0x0120a5a8 | 124 | 104 |
| `Allocate__t7VIArray1Z11VIWorldNode` | 0x012100f8 | 136 | 132 |
| `DestroyPool__t7VIArray1Z11VIWorldNode` | 0x01210180 | 76 | 8 |
| `DestroyPool__t7VIArray1Z11VIWorldLeaf` | 0x012101d0 | 76 | 8 |
| `Allocate__t7VIArray1Z11VIWorldLeaf` | 0x01210220 | 120 | 72 |
| `MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi` | 0x01210ae0 | 240 | 244 |
| `Clear__t7VIArray1Z10VIZoneRoom` | 0x01213ae0 | 100 | 120 |
| `Allocate__t7VIArray1Z10VIZoneRoom` | 0x01213ea0 | 156 | 184 |
| `Init__t7VIArray1Z10VIZoneNodei` | 0x01214e28 | 92 | 72 |
| `Init__t7VIArray1Z10VIZoneLeafi` | 0x01214e88 | 92 | 72 |
| `Init__t10VIMultiMap2ZfZ14VIEmitterEntryi` | 0x01226638 | 92 | 72 |

Image bytes are the symbol sizes in the image; for `DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry` the symbol size (132) is too small,
the routine runs to 0x011f4d10 (about 0x170 bytes). Compiled bytes differ from the image because the compiler removes redundant stores
or merges the reloads of the base pointer the image repeats (this is also why the empty-loop `DestroyPool` and `Allocate` leaf variants are
shorter); none of these is claimed to be byte identical.

## Observed, inferred, unknown

The leading comment of each source file is the record. Short form:

* Observed: the list primitives form a sentinel list: `Init__7VILList` stores this+4 at +0, zero at +4 and this at +8; `AddTail` links a
  node as {next = list+4, previous = old tail}, writes the node into the old tail's first word and into +8.
* Observed: the container template instances share the shape count at +0 and storage pointer at +4 (arrays); the allocators call
  `__builtin_vec_new(count * element size)`, store the pointer before testing it for null, and construct elements in place
  (the node element zeroes four floats, the room element calls `__10VIZoneRoom`, the leaf element needs nothing). The multimap `Init` stores
  count + 1 at +4.
* Observed: the object file header (32 bytes at +200: magic, two words, a gap word, a 64-bit start and a 64-bit cursor) is read and written
  in place with five `VIBufferSwap` calls when the global `VIEndian` is not 1. The read-scope stack is a 32-byte record array at +256 indexed by
  the int at +232, with remaining length at +8 and child length at +20 of each record.
* Observed: the dictionary is a map from 32-bit keys to nodes of 28 bytes (key at +16, children at +8/+12, parent at +4, entry words from +20),
  root index at +12 and array base at +8; Erase is a red-black style deletion that calls `Next`, `EraseFixup` and `FreeNode`, and writes node 0.
* Observed: `str16to8` truncates to `limit - 1` bytes, copies units below 256 as their low byte, and maps others through a 32-entry table
  (index i becomes byte i + 128) with `?` for misses.
* Inferred: class and member names, element types, the colour reading of the node word at +0 in the map, that origin 1 means "from the start"
  and 2 "from the end" for `Seek`, the LRU orientation of the raster pool, and the purpose of the string table blocks at +8 and +12.
* Unknown: every callee body listed below, object sizes (only stored ranges are known), `VIEndian` values other than 1, the meaning of the
  value 2 in the flat-file structure, hardware behaviour, and the negative-count behaviour of `DestroyPool` for the node and leaf arrays (the
  image loop would run for billions of iterations; the authored function returns at once).

## Imports closed

All 36 names in the package scope: `AbortOpen__9VIObjFile`, `AddTail__7VILListP7VILNode`, `Allocate__t7VIArray1Z10VIZoneRoom`, `Allocate__t7VIArray1Z11VIWorldLeaf`, `Allocate__t7VIArray1Z11VIWorldNode`, `Clear__t7VIArray1Z10VIZoneRoom`, `DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry`, `DestroyPool__t7VIArray1Z11VIWorldLeaf`, `DestroyPool__t7VIArray1Z11VIWorldNode`, `Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`, `Erase__t6VIPool1ZP20VIParticleDefinitioni`, `Find__12VIDictionaryUiR14VIResourceTypeRi`, `Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi`, `Flush__9VIObjFile`, `Init__7VILList`, `Init__t10VIMultiMap2ZfZ14VIEmitterEntryi`, `Init__t7VIArray1Z10VIZoneLeafi`, `Init__t7VIArray1Z10VIZoneNodei`, `Load__13VIStringTablePCc`, `MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi`, `ReadBegin__9VIObjFileRUsT1`, `ReadEnd__9VIObjFile`, `ReadFileHeader__9VIObjFilei`, `Unload__13VIStringTable`, `WriteEnd__9VIObjFile`, `WriteFileHeader__9VIObjFilei`, `_$_13VIStringTable`, `_$_19VIFlatFileStructure`, `_$_t7VIArray1Z14VIResourceElem`, `__13VIStringTable`, `__19VIFlatFileStructure`, `__7VILList`, `__7VILNode`, `__t7VIArray1Z14VIResourceElem`, `str16to8__8VIStringPcUiPCUs`, `strlen16__8VIStringPCUs`.

## Imports opened (referenced by these units, not defined by them)

Heuristic search of the shared tip (definition lines in other `experimental/sources` units) found definitions for `Close__6VIFile`,
`End__Ct10VIMultiMap2ZfZ14VIEmitterEntry`, `End__Ct6VIPool1ZP20VIParticleDefinition`, `VIBufferSwap__FiPUc` and `_$_10VIMatrix44`. The
rest were not found and are unresolved imports until another package supplies them:

`Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry`, `Allocate__t7VIArray1Z10VIZoneLeaf`, `Allocate__t7VIArray1Z10VIZoneNode`,
`Clear__t10VIMultiMap2ZfZ14VIEmitterEntry`, `Clear__t7VIArray1Z10VIZoneLeaf`, `Clear__t7VIArray1Z10VIZoneNode`,
`DestroyPool__t7VIArray1Z10VIZoneRoom`, `DestroyPool__t7VIArray1Z14VIResourceElem`,
`End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry`, `EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`,
`FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`, `Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`,
`FixupTextBuffer__13VIStringTable`, `LoadRawFile__13VIStringTablePCcRt7VIArray1Zc`, `ProcessRawFile__13VIStringTableRCt7VIArray1Zc`,
`ReadBegin__9VIObjFileRUsT1RUlRUi`, `WriteEnd__9VIObjFileRUi`, `Flush__6VIFile`, `Read__6VIFilePvi`, `Seek__6VIFilelQ26VIFile10SeekOrigin`,
`Tell__6VIFile`, `Write__6VIFilePCvi`, `_$_19VIFlatFileDirectory`, `__t6VIList1ZP10VIFlatFile`, `__t7VIArray1Zc`, `_$_t7VIArray1Zc`,
`__10VIZoneRoom`, `__builtin_delete`, `__builtin_vec_delete`, `__builtin_vec_new`, `__wrap_free`, `__wrap_strdup`;
data: `VIEndian`, `_vt$19VIFlatFileDirectory`, `_8VIString$ConversionTable1252`, `ConversionTableSize.180` (a file-local symbol in the image;
imported here by its image name and stored as a 32-bit count of 32). Several of these (`Close`, `Tell`, `Seek`, `Read` and the `__builtin_*`
names) may already be supplied by other packages of this wave; the integrator should check before declaring them open.

## Build-plan entries needed

Thirty-six entries, one per directory `experimental/sources/w14-objfile-*`, printed by `python3 docs/agents/w14-objfile-plan-entries.py
--toolchain /path/to/ee` (optionally `--private-root DIR` to write a private plan copy). Each entry is `authored_cpp`, has an empty header
closure and names the source file in its directory (`host_check.cpp` is never an input). The script hashes the source, so the sources must
not change after the entries are taken. No private partial build with these units was run.

## Authored but never executed

All 36 functions have only been run in the host checks and in the private paired interpreter; none has run on the EE, in a linked image or in
the game. The `DestroyPool__t10VIMultiMap2...` mutation runs and the paired runs use trees that are valid binary trees (random cyclic trees
would not terminate in either implementation).

## What is unknown

* Names of the container fields, the real element types, and why `__7VILList` keeps a stack frame around its tail jump in the image
  (this source compiles to a bare jump).
* Whether the image's `VIStringTable` second block at +8 is used by anything in this package.
* The code-page tables behind `str16to8` (only the 32-entry table and its count word are used; their owner is another package).
* The negative-count loop of `DestroyPool__t7VIArray1Z11VIWorldNode/Leaf`.

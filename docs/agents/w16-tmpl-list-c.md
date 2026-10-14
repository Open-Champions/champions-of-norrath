# List, pool, array and vector templates (part 3)

Package work is on `agents/w16-tmpl-list-c`. New source is under `experimental/sources/w16-tmpl-list-c-*`,
one directory per function. Addresses and sizes are from the pinned SLUS-20565 symbol table and the reviewed EE
decoder; they are local evidence, not reproduced by the relocatable build. Every function was compiled with the
reviewed `authored_cpp` flags (EE g++ 15.2.0). Fresh EE objects are not original-byte matches.

## How each function was checked

1. A host check (`host_check.cpp`, not a build input) that builds a small record graph in an arena and checks
   the result, every changed word and the call order of stubbed callees.
2. A paired run on a private R5900 interpreter kept outside git: the authored object is linked next to the
   original image with every undefined symbol bound to the original address; the original and authored function run
   from identical memory snapshots with identical Python stubs for callees, and the harness compares the call log,
   the return value and every changed byte outside the stack. Paired runs are not repeatable from the repository.
   Mutations of the authored source (wrong offset, wrong end constant, swapped link, wrong capacity rule) were
   rejected by the paired runs for `Next`, `Erase` (pool and list) and `Insert`.

| Function | Address | Scenarios | Original words covered |
|---|---|---|---|
| `End__Ct6VIPool1Z17VIHSpritePlayNode`, `...Z19VIRasterMaterialLRU`, `...ZP12VIPrimBuffer`, `...ZP13VIColorBuffer` | `0x012039f0`, `0x01212e90`, `0x01211690`, `0x012115b8` | 2 each | all |
| `Next__Ct6VIPool1Z10VIEEBucketi` | `0x01213618` | 24 | all of its own words |
| `Next__Ct6VIPool1Z17VIHSpritePlayNodei` | `0x012037b8` | 24 | all |
| `Next__Ct6VIPool1Z19VIRasterMaterialLRUi`, `...ZP12VICollBufferi`, `...ZP12VIPrimBufferi`, `...ZP13VIColorBufferi` | `0x01212ea0`, `0x01207330`, `0x012116a0`, `0x012115c8` | 24 each | all |
| `Next__Ct6VIPool1ZP13VISpellEffecti`, `...ZP20VIParticleDefinitioni` | `0x011f3990`, `0x01209038` | 24 each | all |
| `Erase__t6VIPool1ZP12VIPrimBufferi`, `...ZP13VIHSpriteAnimi`, `...ZP13VIMaterialPali`, `...ZP16VIStaticLightingi`, `...ZP8VIRefMapi`, `...ZP8VISpritei` | `0x01210780`, `0x01222fe8`, `0x01210cd8`, `0x01223188`, `0x012230b8`, `0x01222f18` | 20 each | 49 of 51 (the rest are padding nops) |
| `Erase__t6VIPool1ZP7VIRFonti` | `0x012123e8` | 20 | 56 of 57 |
| `Erase__t6VIList1ZP10VIFlatFilei` | `0x011f3c00` | 6 | 32 of 34 (nops) |
| `Erase__t6VIList1ZQ27VIScene12VISceneOccupi` | `0x01225270` | 6 | 37 of 39 (nops) |
| `Init__t7VIArray1Z14VIResourceElemi`, `...Z16VIHSpriteTriggeri` | `0x01227360`, `0x01227180` | 18 each | 22 of 23 (nop) |
| `Init__t8VIVector1Z10VIMatrix44i`, `...Z13VIHSpriteNodei` | `0x01227120`, `0x012270c0` | 18 each | 22 of 23 (nop) |
| `InsertAfter__7VILNodeP7VILNode` | `0x010726b8` | 4 | all |
| `Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi` | `0x012211a8` | 48 | 89 of 91 (nops) |
| `_$_10VIZoneRoom` | `0x0111f338` | 6 | 26 of 27 (nop) |

## Observed

- Every pool `Next` reads the entries pointer at pool+8 and one link word per entry, and returns 0x7fffffff for -1.
  Strides and link offsets: 12/+4 for `VICollBuffer`, `VIPrimBuffer`, `VIColorBuffer`, `VISpellEffect`, `VIParticleDefinition`;
  24/+16 for `VIEEBucket` and `VIRasterMaterialLRU`; 52/+44 for `VIHSpritePlayNode`. `VISpellEffect`, `VIParticleDefinition`
  and `VIHSpritePlayNode` tail-call their `End__Ct6VIPool1...` for -1; the others carry the constant inline, and the
  four `End` functions here all return 0x7fffffff regardless of argument.
- All seven pool `Erase` functions have the same 12-byte-record body (the six without a call are byte-identical
  machine code); only `VIRFont` tail-calls `End__Ct6VIPool1ZP7VIRFont` when the erased record was the last in its chain.
  The pool words used: +0 count, +8 records, +12 free head, +16 first, +20 last. Record words +4 next, +8 previous, -2 in +8 marks free.
- The two list `Erase` routines unlink a node, call `__builtin_delete` on it, decrement the count and return the next
  node. The `VISceneOccup` node keeps its links at +24 (next) and +28 (previous); `VIFlatFile` at +4 and +8.
  The scene one tail-calls `End__Ct6VIList1ZQ27VIScene12VISceneOccup` at the end of the list.
- The `Init` functions call `Clear`, store the count (array: +0, vector: +4), then call `Allocate`; -1 if either fails.
- `Insert__...VIEEBucket...i` copies only 16 bytes of the bucket (two unaligned 8-byte copies), sets the record's
  words at +16 and +20 to the position and the position's previous link, repairs the neighbour (or pool+16), and
  increments pool+0. An exhausted free chain doubles pool+4 (1 when zero) before calling `Allocate`.
- `_$_10VIZoneRoom` calls `Clear__10VIZoneRoom`, then the int-array destructor on +68 and +60 with the constant 2,
  then deletes itself when flags bit 0 is set.

## Inferred (not proven)

Role names in comments: doubly linked chains, free chain, iterator end marker, delete-after flag, the doubling
rule meaning capacity growth. Types in the symbol names come only from the mangled names.

## Not mine / already present

`Remove__7VILNode` is already authored in `w12-vimath-b-vilnode-remove` (not added to the plan). I ran it in a paired
comparison (4 scenarios, all 14 original words, no mismatches). It needs a plan entry; I did not add a duplicate.

## Imports closed (28 names, 27 new sources plus the existing Remove unit)

End__Ct6VIPool1Z17VIHSpritePlayNode, End__Ct6VIPool1Z19VIRasterMaterialLRU, End__Ct6VIPool1ZP12VIPrimBuffer,
End__Ct6VIPool1ZP13VIColorBuffer, Erase__t6VIList1ZP10VIFlatFilei, Erase__t6VIList1ZQ27VIScene12VISceneOccupi,
Erase__t6VIPool1ZP12VIPrimBufferi, Erase__t6VIPool1ZP13VIHSpriteAnimi, Erase__t6VIPool1ZP13VIMaterialPali,
Erase__t6VIPool1ZP16VIStaticLightingi, Erase__t6VIPool1ZP7VIRFonti, Erase__t6VIPool1ZP8VIRefMapi,
Erase__t6VIPool1ZP8VISpritei, Init__t7VIArray1Z14VIResourceElemi, Init__t7VIArray1Z16VIHSpriteTriggeri,
Init__t8VIVector1Z10VIMatrix44i, Init__t8VIVector1Z13VIHSpriteNodei, InsertAfter__7VILNodeP7VILNode,
Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi, Next__Ct6VIPool1Z10VIEEBucketi, Next__Ct6VIPool1Z17VIHSpritePlayNodei,
Next__Ct6VIPool1Z19VIRasterMaterialLRUi, Next__Ct6VIPool1ZP12VICollBufferi, Next__Ct6VIPool1ZP12VIPrimBufferi,
Next__Ct6VIPool1ZP13VIColorBufferi, Next__Ct6VIPool1ZP13VISpellEffecti, Next__Ct6VIPool1ZP20VIParticleDefinitioni,
_$_10VIZoneRoom. `Remove__7VILNode`: source exists, only the plan entry is missing.

## Imports opened

A private build (shared plan plus these 28 units, 7,355 inputs, one link, no retry) produced 5,862 function bodies and 819
unresolved symbols. The names in this package's scope are all defined except `Remove__7VILNode`, whose existing
unit `w12-vimath-b-vilnode-remove` is not in the shared plan (add it; this script does not). Callees these sources
use that nothing in the plan defines (all were stubbed in the checks):
Clear__ and Allocate__ for t7VIArray1Z14VIResourceElem, t7VIArray1Z16VIHSpriteTrigger, t8VIVector1Z10VIMatrix44 and
t8VIVector1Z13VIHSpriteNode; Add__t6VIPool1Z10VIEEBucketRC10VIEEBucket; Allocate__t6VIPool1Z10VIEEBucket;
End__Ct6VIPool1ZP7VIRFont; End__Ct6VIList1ZQ27VIScene12VISceneOccup; Clear__10VIZoneRoom.
Already defined in the plan: End__Ct6VIPool1ZP13VISpellEffect, End__Ct6VIPool1ZP20VIParticleDefinition,
_$_t7VIArray1Zi, __builtin_delete.

## Build-plan entries needed

`python3 docs/agents/w16-tmpl-list-c-plan-entries.py --toolchain <ee> [--private-root DIR]` prints 28 JSON entries
(one per unit directory, object names `w16tlc_*.o`) and can write a private plan/snapshot copy. The shared files
were not edited.

## Authored but never executed

All of it ran only on the private interpreter and the host checks: no function ran inside the game. The `__builtin_delete`,
`Clear`, `Allocate`, `Add` and `End` callees were stubs. The `Insert` bucket copy was run at 4-byte alignment only.

## Unknown

The record fields beyond the link words (strides 12, 24 and 52); pool+4 in the 12-byte pools; pool+20 of
`VIEEBucket`; the element types behind the mangled names; why the nm size of `Next__Ct6VIPool1Z10VIEEBucketi` (0x44)
overshoots the 0x30 bytes before the next symbol.

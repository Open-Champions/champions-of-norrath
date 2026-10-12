# game-loop

Package work is on `agents/game-loop`. The branch was merged with
thirteen units and no summary note, so this file records the integrator's
selection. Eleven of the thirteen units were selected. Each unit has a
host check beside its source that returns failure on wrong behavior; all
thirteen passed on this machine. No game binary or disassembly listing
was committed.

## Observed

`gameSetCameraShake__Ffb` at `0x001d0a88`, 212 bytes. `gameCameraShake`
is replaced only on the observed path.

`gameCheckPlayerAlive__Fv` at `0x001d0fa8`, 76 bytes. The signed word
`nmPlayers` is loaded once; a nonpositive count returns 0.

`frontEndStartInNetMenu__Fv` at `0x001ca9b8`, 68 bytes. Stores 1 in
`isNetGame` and 2 in `menuNm`.

`gameNoDraw` at `0x00483f18`: four `.sdata` bytes, all zero.

`gameOver` at `0x00484f99`: one `.sdata` byte, zero.

`hudNoDraw` at `0x00484af8`: one `.sdata` byte, zero.

`gameNetKick__Fi` at `0x001d0900`, 224 bytes.

`gamePlayerLightAttenuate` at `0x00484f24`: four `.sdata` bytes, all
zero.

`gameRecallInhibit` at `0x00486694`: four `.sdata` bytes, all zero.

`runObjectsTask__Fv` at `0x00140070`, 924 bytes. Two file-local helpers
(`walk_run_list`, `call_slot`) are recorded alongside it.

`transformSceneLightTask__Fv` at `0x0013c918`, 292 bytes.

## Inferred

The defined names are reconstructions of the observed originals; call
order, stored values, and depreciation paths beyond the host cases remain
unknown.

## Not selected

game-loop-draw-particles redefines `drawParticlesTask__Fv` from the
particles package. game-loop-object-draw-enable redefines
`objectDrawEnable` from the particles package. Both files stay in the
source snapshot.

## Imports closed

The eleven selected names, to the extent each was an unresolved import
on the integration tip.

## Imports opened

Whatever the eleven units reference that the tip does not define; the
measured link delta decides the list.

## Build-plan entries needed

Eleven inputs with fresh `input_order` values, profile `authored_cpp`,
empty `header_closure`, and the measured emitted-function lists including
the two `runObjectsTask` file-local helpers. Host files stay out of the
plan. Snapshot `files` records every new source and host file, including
the two unselected units' files.

## Authored but never executed

All eleven selected functions and five data words above, plus the two
unselected units. Host checks ran on this machine. The reviewed EE
compiler produced relocatable objects and the partial link succeeded. No
routine has run on the EE, under the original process, or as gameplay.

## Unknown

Game-loop ordering, callback scope, and runtime behavior remain
unverified.

# actor-view-tables

Package work is on `agents/actor-view-tables`. The branch was merged with
six data-view commits and no summary note, so this file records the
integrator's selection. All six units are data objects; no function was
added. Each unit has a host file under
`experimental/sources/actor-view-tables-views/host/` that returns failure
on a wrong size or nonzero byte. All six passed on this machine. No game
binary or disassembly listing was committed.

## Observed

`norrath_actor_view_486878`: observed `.sbss` symbol `runList` at
`0x486878`. The ELF size field is 0; the view is 8 bytes, the gap before
`deleteList`. Emitted in BSS by the reviewed profile.

`norrath_actors_a_view_438e98`: observed `.data` symbol `bombResMap` at
`0x438e98`, 32 bytes of name-pointer/zero-word slots. FireBomb's
constructor passes this address as the resource map.

`norrath_actors_a_view_449430`: observed `.data` symbol `_vt$8FireBomb`
at `0x449430`, 144 bytes of adjustment-word/function-pointer slots.

`norrath_actors_a_view_44ccb8`: observed `.data` symbol `_vt$9Container`
at `0x44ccb8`, 144 bytes of adjustment-word/function-pointer slots.

`norrath_actors_a_view_44d0a8`: observed `.data` symbol `_vt$9WorldPart`
at `0x44d0a8`, 144 bytes of adjustment-word/function-pointer slots.

`norrath_actors_a_view_485358`: observed unnamed sdata C string at
`0x485358`. The view is `"name"` plus the terminating NUL (5 bytes);
3 zero bytes follow the NUL and are not part of this object. DoorSecret,
DoorSwing, and FloorSwitch pass it to `findTagString`.

## Inferred

The view names are reconstructions; only the addresses, sizes, and the
named original symbols above were observed. Slot indexes used by the
game, vtable dispatch and overrides, and pointer identity with other
copies of these names remain unknown.

## Imports closed

`norrath_actor_view_486878`, `norrath_actors_a_view_438e98`,
`norrath_actors_a_view_449430`, `norrath_actors_a_view_44ccb8`,
`norrath_actors_a_view_44d0a8`, `norrath_actors_a_view_485358`,
to the extent each was an unresolved import on the integration tip.

## Imports opened

The three 144-byte vtables hold adjustment-word/function-pointer slots,
so the link newly references the 18 slot targets: the three `_$_*`
destructors, the three `__tf*` typeinfo functions, the three `clone__*`
functions, and the nine Container/FireBomb message slots
(`msg_collision__9ContainerP10GameObject`, `msg_draw__9Container`,
`msg_getPokeName__9ContainerPUsi`,
`msg_hurt__9ContainerfP10GameObjecti`, `msg_load__9ContainerPc`,
`msg_poke__9Containeri`, `msg_run__8FireBomb`, `msg_run__9Container`,
`msg_save__9ContainerPc`). The resource map, name tag, and run-list head
reference nothing.

## Build-plan entries needed

Six data inputs with fresh `input_order` values, profile `authored_cpp`,
empty `header_closure`, and empty emitted-function lists. Host files stay
out of the plan. Snapshot `files` records every new source and host file.

## Authored but never executed

All six data objects above. Host checks ran on this machine. The reviewed
EE compiler produced relocatable objects and the partial link succeeded.
No routine has run on the EE, under the original process, or as gameplay.

## Unknown

Which run-list callers use the second head word; which resource-map slot
indexes the game uses; vtable dispatch and overrides; whether other
copies of these names are pointer-identical.

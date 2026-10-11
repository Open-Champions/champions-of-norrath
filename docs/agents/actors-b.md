# Actors-b constructor notes

Package: `norrath_construct_<Class>` imports whose class name starts with G–Z.
Worktree branch `agents/actors-b`. Base constructors call parents owned by other
packages; those parents stay imports.

Private reference: `work/private/SLUS_205.65` SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
Symbol addresses below are that file's symbol values. No game bytes are copied
into this tree.

## HackDoor

Observed `__8HackDoorPcG6Point3sPPc` at `0x2264e8`, 96 bytes. It copies the 12
point bytes, sign-extends the short, and calls `__8AutoPropPcG6Point3sPPc`.
Then it stores `_vt$8HackDoor` (`0x44ee08`) at object+72 and ORs 1 into the
word at object+12. Low V0 is AutoProp's low V0. The factory discards that word.

Unknown: AutoProp's own effects, vtable payload, and whether a caller ever
uses this V0.

Host contract: `experimental/sources/actors-b-constructors/host/hack_door_contract.cpp`
(not an EE input). EE object `hack_door.o` defines `norrath_construct_HackDoor`
at 164 body bytes and imports `norrath_construct_AutoProp` and
`norrath_actors_b_view_44ee08`.

Build-plan entry:

```json
{
  "header_closure": ["sources/actors-b-constructors/detail.hpp"],
  "kind": "independently_authored_source",
  "object_name": "hack_door.o",
  "profile": "authored_cpp",
  "source": "sources/actors-b-constructors/hack_door.cpp",
  "sha256": "68c5ab661c21dfab5c890cf870f087156c116f2c56ecee53a7c756b7135f01da",
  "emitted_functions": [{"name": "norrath_construct_HackDoor", "compiled_body_bytes": 164}]
}
```

`detail.hpp` sha256 `925e44ea6cd7b79765b3576f5118da8cd06862040bfb7ab070ad8032f4efeb4e`.
The host contract sha256 is `1769d9f65604c35c342989b4a6e08a172f3700af8efb123e105cd35b111a8b8d`.
`snapshot.json` currently rejects unlisted files under `experimental/sources/`;
these three paths need recording there. This package does not edit that file.

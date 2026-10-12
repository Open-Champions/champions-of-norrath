# w8-net-play-c: network play and player state (3/3)

Addresses are from the pinned `SLUS_205.65` image (symbol table, hash-pinned by the repository). Everything below was read from that image and compiled with the reviewed EE g++ `authored_cpp` profile. Nothing was executed on an EE or in the game. Compiled sizes differ from image sizes because the compiler repacks and tail-calls; no byte match is claimed.

Sources live under `experimental/sources/w8-net-play-c-*`. Each directory holds the unit and a `host_check.cpp` (not a build-plan input). Host checks are run against stub callees and an independently written model or a table of expected stores; where the unit reads a 32-bit pointer word from a record, the check is built for x86_64 with `-Wl,-pagezero_size,0x10000` so the fake objects can live below 4 GiB (Rosetta on this machine).

## Functions

| Symbol | Address | Image bytes | Compiled bytes |
|---|---|---|---|
| `playerSelectionRunning__Fv` | `0x00280508` | 8 | 12 |
| `startNetLaunch__Fv` | `0x001ca920` | 68 | 72 |
| `playerGetFeatRank__Fix` | `0x001abb48` | 148 | 124 |
| `playerDeallocateArmorCaches__Fv` | `0x00195bb8` | 256 | 312 |
| `playerRecomputeWeight__Fi` | `0x001a5358` | 292 | 292 |
| `recievePacket__FiPUc` | `0x002cc2f0` | 440 | 252 |
| `playerGlobalInit__Fv` | `0x0019e3e0` | 928 | 292 + copy helper |
| `playerRecomputeAbilityModifiers__Fv` | `0x001a5480` | 1344 | 1708 |

## Data defined (`w8-net-play-c-player-select-state`)

`playerMenu` (80 bytes, five 16-byte records; the first four hold a pointer to the C strings `menu_oneplayer`, `menu_twoplayer`, `menu_threeplayer`, `menu_fourplayer`), `playerSelectNextName` and `playerSelectPrevName` (64 zero bytes each), `worldCharViewMatrix` (64 zero bytes, 16-byte aligned), `playerRecalled`, `recallEnter`, `playerSelectCount`, `playerSelectStartPlayer`, `playerSelectEnforce` (one byte each, zero), `playerSelectSuccessful` (one byte, initial value 1), `playerSelectExitMessage` and `syncState` (words, zero). Sizes and initial bytes match the image; the compiled symbol sizes were compared with the ELF sizes. Layout beyond size and initial bytes (for example whether the 64-byte objects are matrices or name buffers) is unknown.

## Observed

- `playerGetFeatRank__Fix` indexes `gSkillTree` (`0x00422b90`, 17960 bytes) as five rows of 3592 bytes with 64 entries of 56 bytes; entry+40 holds the 64-bit feat id, and the rank is the signed byte at `player + index*23984 + 243 + entry`. Negative ranks are reported as 0.
- `playerGlobalInit__Fv` initializes every `gSkillTree` entry (byte 32 = -1, byte 33 = 1, word 48 = 0) and copies five default tables into rows: Warrior to row 0, Cleric 1, ShadowKnight 2, Ranger 3, Wizard 4, each up to an entry named `END`. Row index equals the class mapping of `playerInit` (0..4).
- `recievePacket__FiPUc` writes into `packetbuffer` (`0x00442668`, 8512 bytes = 4 sources x 16 ring slots x 133 bytes) at the position one past the last occupied ring slot, or position 4 when the ring is empty; position 16 drops the packet.
- `playerDeallocateArmorCaches__Fv` clears a 3520-byte table at `0x007cb770` that has no symbol in the image. The unit imports it as `norrath_player_armor_cache_view_7cb770`; that name is mine and needs a link alias or an owner.
- `playerRecomputeWeight__Fi` and `playerRecomputeAbilityModifiers__Fv` read the item lists at record+328.. (three bags of 64 items of 72 bytes, count words at record+4936+4616*b) and the 16 equipment pointers at record+21896.

## Inferred

- The sentinel string the skill-table copy compares against is `END`; the original loads it from a string object that the symbol table does not name.
- `playerRecomputeAbilityModifiers__Fv` clamps a "current" float to a "maximum" float at record+172/176 and record+196/200 (the smaller wins); this is read from the compare direction only.

## Unknown

Field meanings of the player record and of `gSkillTree` entries (including what feat id `0x10000`, `128`, `256`, `64` and item kind byte `8` denote); the item/stat ids 7, 8, 9, 10, 30; why an empty packet ring starts at position 4; the real owner of the armor cache table; the type of `worldCharViewMatrix` and the name buffers.

## Not attempted or not finished

| Symbol | Why |
|---|---|
| `startPacket__Fi` (`0x001d34d0`, 720 bytes) | Touches about 20 global dialog words that have no symbol (they only show up as offsets from the global pointer) and four calls into the font/script code. Needs the owner of the dialog state to name them. |
| `preCacheArmor__6Player` (`0x00195cb8`, 1076 bytes) | 11-way jump table, the unnamed armor cache table, and the file-local `cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture`. |
| `playerDamageHandler__6Player` (`0x001a2740`, 2012 bytes) | Many callees, sprintf-built animation names, unnamed float literals and a random-number branch. |
| `playerPlaySound__Fii` (`0x001a6760`, 3896 bytes) | 10-way jump table of sprintf-built sound names. |
| `setupDefaultEquipment__Fi` (`0x0019e780`, 11948 bytes) | Large. |
| `playerSelectionTask__Fv` (`0x0027a658`, 12200 bytes) | Large. |

## Imports closed

See the end of this file.

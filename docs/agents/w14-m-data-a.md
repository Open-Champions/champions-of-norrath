# w14-m-data-a: particle, resource-map and effect data imports (part a)

Branch `agents/w14-m-data-a`. Private reference: `work/private/SLUS_205.65` in the main checkout
(read-only; not committed). Addresses and local symbol names come from that image's symbol table.
Nothing derived from its bytes, disassembly or decompiler output is committed, and no fresh
object is an original-byte match.

## Result in one paragraph

Of the 26 names in the scope, this package closes **none in the shared build as it stands**.
Two are closable by integrating a plan entry that already exists in the `w10-render-b` notes
(lava pair). Two more are closable by a caller-side label in another package (resource maps).
Twenty-two are file-local particle records in other packages' static-initializer units, except
`mStarParticleDist`, which is blocked on its table data. A
global 80-byte copy in this package would link, but the caller would read zeros while the
owner's initializer writes its own private copy. Earlier work in `w12-particle-data-a` made
the same call for the same reason, so I did not create those copies. The owner packages need
a linkage change, and their names need a decision first, so those imports stay open and are
listed under "Owner changes needed".

## Build state checked

- A fresh full partial build was started but stopped at 107 of 6,235 source compiles. The
  driver was interrupted and left `build-status.json` as `in_progress`. A full run is about
  hours of compile time, so I did not repeat it.
- The newest complete run is `/Users/case/norrath-agents/repo/work/plan14-build`. Its plan
  SHA-256 is `18e2fc1a78cecfae97029b12a0b04db1fe84f596d846b1aaac45f9300fc6570f`, the same as
  the current `experimental/build-plan.json`. Its `unresolved-imports.json` has 957 entries
  and contains all 26 scope names, so I used that list. Status there: 6,235 implicit assemblies,
  one link, 4,921 functions, 957 unresolved, zero COMMON symbols.

## Observed (from the private image's symbol table)

| Scope name | Address | Local symbol | Size (gap) | Owner unit (authored) | Owner storage form |
|---|---|---|---|---|---|
| blueSpark | 0x007cb5a0 | blueSpark | 80 | w8-net-play-a-global-init-nm-players | `static unsigned char blueSpark[80]` |
| bombResMap | 0x00438e98 | bombResMap (`d`) | 32 | actor-view-tables-views `norrath_actors_a_view_438e98` | global view, 32 bytes |
| coldSmoke | 0x008d6e60 | coldSmoke | 80 | w8-spell-static-init-a-cold-arrow-projectile | anonymous namespace |
| coldSpeck | 0x008d6eb0 | coldSpeck | 80 | same | anonymous namespace |
| diseaseshield_run_particle_8d69b0 | 0x008d69b0 | diseaseshieldParticle1 | 80 | w8-spell-static-init-a-spell-disease-shield | anonymous namespace |
| diseaseshield_run_particle_8d6a00 | 0x008d6a00 | diseaseshieldParticle2 | 80 | same | anonymous namespace |
| disintoInnoruukParticle | 0x007d7060 | disintoInnoruukParticle | 80 | w8-fx-particles-init-innoruuk | `static` |
| dustcloud_msg_run_particle_7d69c0 | 0x007d69c0 | hitParticleDust2 (fx copy) | 80 | w8-fx-particles-init-fx | `static` |
| explode_arrow_msg_run_particle_8d7090 | 0x008d7090 | explodeArrowParticle | 80 | w10-actor-e-static-init-explode-arrow-projectile | anonymous namespace |
| fxflyingfire_particle_7d6830 | 0x007d6830 | sparkParticle | 80 | w8-fx-particles-init-fx | `static` |
| fxflyingfire_particle_7d6880 | 0x007d6880 | flameParticle | 80 | w8-fx-particles-init-fx | `static` |
| fxflyingfire_particle_7d68d0 | 0x007d68d0 | smokeParticle | 80 | w8-fx-particles-init-fx | `static` |
| fxflyingfire_particle_7d6920 | 0x007d6920 | smokeParticleSmall | 80 | w8-fx-particles-init-fx | `static` |
| goblinWeaponResourceMap | 0x0042cfc8 | goblinWeaponResourceMap (`D`) | 128 | w9-ctors-lantern-a-views `norrath_ctors_ant_lantern_view_42cfc8` | global view, 128 bytes |
| harm1 | 0x008d6cd0 | harm1 | 80 | w8-spell-static-init-a-spell-harm-touch | anonymous namespace |
| harm2 | 0x008d6d20 | harm2 | 80 | same | anonymous namespace |
| hitParticleDust2 | 0x007d70c0 | hitParticleDust2 (cyclops copy) | 80 | w8-fx-particles-init-cyclops | `static` |
| holyshield_particle_8d7720 | 0x008d7720 | hshit | 80 | w10-actor-a-static-init-skill-holy-shield | anonymous namespace |
| hornomen_run_particle_8d8650 | 0x008d8650 | hornomenParticle1 | 80 | w10-actor-a-static-init-spell-horn-omen-effect | anonymous namespace |
| iceSwordParticle | 0x007d6560 | iceSwordParticle | 80 | w8-fx-particles-init-fx | `static` |
| lampParticle | 0x007d7d80 | lampParticle | 80 | w8-spell-static-init-a-torch | anonymous namespace |
| lavaGoopParticle | 0x008d56f8 | lavaGoopParticle | 80 | w10-render-b-data-lava-particles | global, `alignas(4)` |
| lavaSteamParticle | 0x008d5748 | lavaSteamParticle | 80 | w10-render-b-data-lava-particles | global, `alignas(4)` |
| mStarParticleDist | 0x0118f118 | mStarParticleDist (`t`, in `.text`) | 384 | w11-vi-render-a-atmosphere-init (extern only) | no production definition |
| multifire_msg_run_particle_8d7040 | 0x008d7040 | trail (local) | 80 | w8-spell-static-init-a-skill-multi-fire | anonymous namespace |
| newFlameSwordParticle1 | 0x007d6470 | newFlameSwordParticle1 | 80 | w8-fx-particles-init-fx | `static` |

The 80-byte extents are gaps to the next symbol. The owner units' address comments give the same
spacing for the records I checked, but I did not check every record against the image.

## Closed by existing source (not yet integrated)

- **lavaGoopParticle, lavaSteamParticle.** `w10-render-b-data-lava-particles/data_lava_particles.cpp`
  defines both as global 80-byte, 4-aligned storage. Its plan entry is already written in
  `docs/agents/w10-render-b.md` (the `data_lava_particles.o` record, SHA-256
  `24b2fa42d858969747819bee88daf95271267ce18e6b7b99a924e9b155a82350`) but is not in
  `experimental/build-plan.json`. Checked privately with the reviewed EE g++ and `ld -m elf32lr5900 -r`
  in an isolated link with the two vehicle units that import them: `nm -S` reports both as global
  `B` with size 0x50 (80 bytes), and neither name is undefined in the link. Integrating that entry closes both imports.
  No authored lava bytes are initialized, so the record is zero storage until its initializer
  is authored; that gap is listed under unknown.

## Closable by a caller-side label (owner packages, verified privately)

- **bombResMap** and **goblinWeaponResourceMap.** Each import is a caller spelling of an address that
  already has a global view under another name. The existing `w10-actor-g-msg-draw-6goblin`
  unit already uses the same asm-label pattern for the goblin map. Private check: I copied the two
  callers and two view units into `/tmp`, added an asm label to each extern (`norrath_actors_a_view_438e98`
  and `norrath_ctors_ant_lantern_view_42cfc8`), compiled all four with the authored_cpp flags and
  linked them with `ld -r`. Neither scope name is undefined, and the views are 0x20 and 0x80 bytes. The
  two callers live in other packages, so the edits are proposed here, not applied:
  - `w12-actor-spell-a-ctor-firebomblet-jr/ctor.cpp`, line 23: `extern unsigned char bombResMap[] __asm__("norrath_actors_a_view_438e98");`
  - `w11-actor-a-drop-item-6goblin/drop_item.cpp`, line 20: `extern unsigned char goblinWeaponResourceMap[] __asm__("norrath_ctors_ant_lantern_view_42cfc8");`

  Both view units are already in the plan (indices 1113 and 2802). This is a same-object link through an
  asm label, not a plan alias; the owner decides whether to accept it.

## Owner changes needed (not closable here without a zero-storage duplicate)

Each record above is file-local in its owner's static-initializer unit. A caller's extern
name links to a separate zero object, so the initializer's writes would never reach the
caller. The fix belongs to the owner: make the storage global (drop `static`, or move the
`ParticleDefStorage` declaration out of the anonymous namespace) and export it under the
name the callers import. I did not create duplicates for them.

- Owners using `static unsigned char NAME[80]`: `w8-net-play-a-global-init-nm-players` (blueSpark),
  `w8-fx-particles-init-innoruuk` (disintoInnoruukParticle), `w8-fx-particles-init-cyclops`
  (hitParticleDust2 at 0x007d70c0), `w8-fx-particles-init-fx` (newFlameSwordParticle1, iceSwordParticle,
  sparkParticle, flameParticle, smokeParticle, smokeParticleSmall, and the fx copy of hitParticleDust2 at
  0x007d69c0).
- Owners using `ParticleDefStorage NAME;` in an anonymous namespace: `w8-spell-static-init-a-cold-arrow-projectile`
  (coldSmoke, coldSpeck), `w8-spell-static-init-a-spell-disease-shield` (diseaseshieldParticle1/2),
  `w8-spell-static-init-a-spell-harm-touch` (harm1, harm2), `w8-spell-static-init-a-torch` (lampParticle),
  `w10-actor-a-static-init-skill-holy-shield` (hshit), `w10-actor-a-static-init-spell-horn-omen-effect`
  (hornomenParticle1), `w10-actor-e-static-init-explode-arrow-projectile` (explodeArrowParticle),
  `w8-spell-static-init-a-skill-multi-fire` (trail at 0x008d7040).

Naming collisions the owners must resolve first:
- The scope name `hitParticleDust2` covers two different objects (0x007d70c0 for cyclops, 0x007d69c0 for
  the fx unit). The dustcloud caller names the fx one `dustcloud_msg_run_particle_7d69c0`. Only one
  global can export `hitParticleDust2`.
- `smokeParticle` appears at several addresses (0x007d68d0, 0x007d7300, 0x007d7770, 0x007d79a0, 0x007d7f20,
  0x008d6550) and `sparkParticle` at several more. The fx-flying-fire import chooses one by address, so its
  owner must give each record a distinct exported name.
- `trail` at 0x008d7040 and 0x008d7090 is the same local name as the particle-system `trail` buffer at
  0x00780b00 (`particles-trail`). The multi-fire owner's global `ParticleDefStorage trail;` must not be exported under
  that name.

## mStarParticleDist (blocked on data)

The image has a 384-byte read-only table in `.text` at 0x0118f118. The only definition of it in
the authored sources is `w11-vi-render-a-atmosphere-init/host_contract.cpp`, which is not a
plan input. To close it, the package needs the table's contents authored from a source
other than the image bytes. I did not copy them, and I did not confirm a formula. This import
stays open.

## Checks performed

- Symbol addresses and sizes from the private image's symbol table, via the reviewed EE `nm`.
- Owner storage forms by reading each owner source file.
- The lava isolated compile and `ld -r` link (three units, `nm -S` and `nm -u`).
- The two asm-label closures, isolated (four units, `ld -r`, `nm -u` and `nm` on the views).
- Not run: a full partial build (stopped at 107 of 6,235 compiles), the owner package builds, and any runtime test.
## Imports closed (names)

None are closed in the shared build by this branch. Closable after integration or owner edits:
`lavaGoopParticle`, `lavaSteamParticle` (plan entry), `bombResMap`, `goblinWeaponResourceMap`
(caller asm labels). Everything else is listed above under "Owner changes needed" or
"mStarParticleDist".

## Imports opened

None. The isolated lava check references only names that already exist in the plan:
`P_AddParticle__FP11ParticleDefP6Point3T1f`, `getParticleVelocity__7VehicleP6Point3`, and
`slideDustParticle`, `slideDustParticleBig`, `slideDustParticleBigDark`, `slideDustParticleDark`.

## Build-plan entries needed

- Integrate the existing `data_lava_particles.o` entry from `docs/agents/w10-render-b.md`. No other entry
  is needed from this package.
- No new authored sources were added here, so the plan and its snapshot are unchanged.

## Authored but never executed

Nothing from this package ran on the EE or in the game. The lava link check and the
asm-label check were isolated and only inspect symbols and extents. The records they close
are zero storage; their initializers are not authored in this package.

## Unknown

Observed: the 26 addresses, local symbol names and extents above; each owner unit's storage form; the
absence of any authored initializer for the lava records.

Inferred: an extern from a different authored unit cannot bind to an owner's `static` or anonymous-namespace
object, so those imports stay open until the owner exports the storage. The original's unit boundaries are
not established here.

Unknown:
- The particle definition fields for every record. The constructors fill them in, and their
  call sites are in other packages.
- Whether the lava records have a static initializer in the original. None is authored.
- The contents of `mStarParticleDist`, and whether it is a formula or a table.
- Which `smokeParticle`/`sparkParticle` address each caller means. The owners must settle this.
- Whether a full build with these owner changes links without new duplicate symbols.

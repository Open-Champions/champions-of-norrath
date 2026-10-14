# Full reconstruction completion criteria

This is an ongoing reconstruction project. A matched function, passing tool test,
compiler installation or merged pull request is a milestone, not completion.
The active target is the exact executable hash in
[baseline.json](../targets/SLUS-20565/baseline.json); the unknown retail pressing
must remain unknown unless independently established.

## Evidence required before calling the reconstruction complete

| Requirement | Evidence required | Current gap |
|---|---|---|
| Stable input identity | Fresh executable hash and independently checked layout | Identified for this exact private input; no retail-authenticity claim |
| Complete executable scope | Inventory of all loaded code, initialized data, zero-initialized memory, overlays and non-runtime ELF records; unresolved mappings explicitly resolved | Main `.text` is only part of the image; `.srs`, VU code, overlays and data remain |
| Authored source | Readable independently authored implementations, validated boundaries, dependencies, interfaces and data layouts | Eleven exact-matched functions; experimental source for 262 partial-build inputs is published separately, with unresolved dependencies and held drafts |
| Reproducible build | Clean-checkout commands, pinned toolchain/components/options and an explicit private-input manifest | Current commands compile isolated functions; no game link target exists |
| Complete link and comparison | Rebuilt ELF, section and load-layout checks, and whole-file comparison against the pinned original; every mismatch investigated | No rebuilt game ELF exists; equal selected functions do not establish a whole image |
| Runtime verification | Executed startup and meaningful game flows compared with the same baseline under recorded conditions; no inferred runtime success from static checks | No reconstructed executable has been executed |
| Reviewable delivery | Reviewed source/build instructions, sanitized reproducibility evidence, and private originals/assets kept out of published history | Tools, exact-matched bodies, and a broader experimental source snapshot are public; no clean-checkout game build or runtime result |

The build must not obtain its apparent code coverage by copying original code
sections, opaque retail bodies, assembly dumps or generated decompiler output into
the result. Any required original assets remain explicit private runtime inputs;
their presence is not reconstructed-source credit. Compiler runtime and middleware
dependencies also need explicit provenance and verification rather than implicit
exclusion from the target.

Runtime-loaded code files are part of the source/rebuild requirement. In particular,
the original disc's `SRS.BIN` payload is byte-identical to the ELF's `.srs` section,
and the observed launch path reads that file into the scripting-code region.
A source build must generate the required runtime code image as well as the ELF.
Running a rebuilt ELF that subsequently reloads original SRS code is not proof of
complete reconstructed-source execution. See [startup evidence](STARTUP.md).

Whole-file equality is a separate claim from loaded-image equality, and both are
separate from runtime behavior. If a future comparison checks only an identified
subset, it must name that subset and leave the remaining completion criteria open.
Non-runtime symbol/overlay records cannot silently disappear to make a narrower
comparison pass.

## Current engineering priorities

1. Build interacting game systems and close the dependencies required for real
   startup and gameplay. Use separate implementation tasks for subagents.
2. Integrate completed source and fix demonstrated build, link, and runtime
   failures. Resolve load layout, data ownership, and ABI questions where they
   block the next working system; preserve accepted toolchain evidence.
3. Check changed behavior and specific unresolved risks. Do not repeatedly audit
   unchanged artifacts. Keep exact-byte match admission separate from experimental
   source publication, and keep both separate from runtime success.
4. Resolve runtime prerequisites alongside source development, execute meaningful
   gameplay milestones, and publish authored source at useful checkpoints. Final
   completion still requires every criterion above; a partial or placeholder
   executable cannot satisfy it.

The shared native Godot client is a separate repository and verification effort.
Success here must not be advertised as a playable native port, and a native
prototype must not be substituted for this PS2 reconstruction goal.

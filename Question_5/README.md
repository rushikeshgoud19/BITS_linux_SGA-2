# Question 5 — Evaluating vi Recovery Mechanisms After a Crash

## Evaluation of vi Recovery Mechanisms

1. **Swap files (`.swp`)** — `vi`/`vim` automatically creates a hidden swap file (e.g. `.filename.swp`) the moment editing starts. It's updated continuously in the background as changes are made, independent of whether the file has been saved. Running `vi -r filename` after a crash reads this swap file and restores the buffer to its last-tracked state.

2. **Undo history / persistent undo (`undodir`)** — Normal undo history lives only in memory and is lost the instant the process dies in a crash. If `undofile`/`undodir` is explicitly configured, undo history is written to disk and survives a reboot, but this is opt-in and not vi's default behavior.

3. **Registers** — Yanked/deleted text held in registers lives in RAM only. None of it persists through a crash.

4. **Backup files (`~` files)** — If `backup` is enabled, vi keeps a copy of the file as it was *before* the current edit began. This protects against a bad overwrite but has nothing to do with recovering unsaved changes made during the crashed session.

5. **Auto-recovery** — Some `vim` builds have autosave-style plugins/macros, but plain `vi` has no built-in autosave beyond the swap file mechanism.

## Most Reliable Strategy

**The swap file recovery mechanism (`vi -r filename`) is the most reliable option.**

**Justification:** The swap file is the only one of these mechanisms that is (a) automatic — no configuration required, and (b) continuously updated at the block level as the user types, not just at save time. Undo history is lost unless `undodir` was pre-configured, registers never persist, and backup files only capture the pre-edit state, not in-progress work. The swap file specifically captures the delta of unsaved changes right up to the moment of the crash, which is exactly what's needed to recover a session that died before a save.

## Files in this folder
- `README.md` — this file
- `Q5_screenshot.png` — optional, only if required by your instructor

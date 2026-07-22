# Question 4 — Real-Time Log Monitoring Pipeline

## Commands Executed
Terminal pane 1 (monitor):
```
touch system.log error_report.log
tail -f system.log 2>/dev/null | grep --line-buffered "ERROR" | tee -a error_report.log
```

Terminal pane 2 (log writer, run while pane 1 is still watching):
```
echo "INFO: Server started" >> system.log
echo "ERROR: DB connection failed" >> system.log
echo "ERROR: Timeout on request" >> system.log
```

## Output (verified)
Pane 1 / `error_report.log` only shows the ERROR lines — the INFO line never appears:
```
ERROR: DB connection failed
ERROR: Timeout on request
```

## Explanations

**`touch system.log error_report.log`** — Created the log file to be monitored and the report file the errors will be saved to.

**`tail -f system.log 2>/dev/null`** — I ran `tail -f` in follow mode so it keeps streaming any new line appended to `system.log` in real time, and redirected stderr to `/dev/null` so any transient "file not found" type errors (e.g. if the file briefly doesn't exist yet) don't clutter the terminal.

**`| grep --line-buffered "ERROR"`** — Piped the live stream into `grep` to filter for only lines containing `ERROR`. `--line-buffered` is important here — without it, `grep`'s output would be block-buffered and wouldn't appear in real time when reading from a pipe instead of a terminal.

**`| tee -a error_report.log`** — Piped the filtered ERROR lines into `tee -a`, which does two things at once: prints the line to the terminal *and* appends it to `error_report.log`, so nothing needs to be run twice to get both a live view and a persistent record.

**`echo "..." >> system.log`** — Simulated new log activity being written by the "server" in a second terminal, to confirm the pipeline in pane 1 picks it up live.

## Technique Justification

* **`tail -f`** — Efficient for real-time monitoring because it doesn't re-read the whole file on each check; it watches for file growth and only emits new bytes.
* **Pipes (`|`)** — Let `tail`, `grep`, and `tee` run as concurrent processes connected by streams, so filtering happens as data arrives rather than after the fact.
* **`/dev/null`** — Used as a "black hole" for unwanted stderr output, keeping the monitoring view clean without losing stdout.
* **`grep --line-buffered`** — Necessary specifically because the input is a pipe, not a terminal; disables `grep`'s default block buffering so matches show up immediately instead of only after a chunk fills up.
* **`tee -a`**  — Avoids needing two separate commands (one to print, one to save); `-a` appends so previous error history in the report isn't lost on each restart.

## Files in this folder
- `README.md` — this file (script is a command pipeline, not a standalone file)
- `Q4_screenshot.png` — screenshot of split terminal (add your own)

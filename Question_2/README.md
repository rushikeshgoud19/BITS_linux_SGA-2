# Question 2 — Zombie-Safe Server Process Monitor

## Commands Executed
```
gcc server_monitor.c -o server_monitor
./server_monitor
```

## Output (verified)
```
Parent monitoring Child (PID: 793). Waiting 3 seconds...
Child (PID: 793) is unresponsive. Terminating...
Child successfully terminated and reaped.
```
(PID will differ on each run — this is expected since it's assigned by the OS.)

## Explanations

**`gcc server_monitor.c -o server_monitor`** — I compiled the C source into an executable binary. No warnings were produced, confirming the syscalls were used with correct signatures.

**`./server_monitor`** — Running the program forked a child that sleeps for 10 seconds (simulating a hung request handler). The parent only waited 3 seconds before checking on it, found it still running via `WNOHANG`, force-killed it, and reaped it — matching the expected unresponsive-child scenario.

## Technique Justification

* **Process creation (`fork()`)** — Used to spin off a separate child process to handle the simulated request, so the parent (acting as the server) can keep monitoring instead of blocking on the request itself.
* **Non-blocking check (`waitpid(pid, &status, WNOHANG)`)** — `WNOHANG` lets the parent *poll* whether the child has exited without freezing if it hasn't. A return value of `0` specifically means "still running," which is how the program decides the child has exceeded its time budget.
* **Zombie prevention** — After `kill()`, I call `waitpid(pid, &status, 0)` a second time — this blocking call reaps the now-dead child and removes its entry from the process table. Without this second call, the killed child would sit as a zombie until the parent exits.
* **Signal handling (`kill(pid, SIGKILL)`)** — `SIGKILL` is used specifically because it can't be caught, blocked, or ignored — appropriate for a process assumed to be unresponsive (a catchable signal like `SIGTERM` might never be handled if the process is genuinely stuck).

## How it fits together
`fork()` creates the concurrency, `waitpid(..., WNOHANG)` gives the parent a way to monitor without blocking, and `kill()` + a final blocking `waitpid()` gives it a clean way to terminate and reap a misbehaving child — directly solving the "excessive child processes / unresponsive server" problem in the prompt.

## Files in this folder
- `server_monitor.c` — the program
- `README.md` — this file
- `Q2_screenshot.png` — terminal screenshot (add your own)

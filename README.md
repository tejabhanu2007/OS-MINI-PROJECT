# CampusCompute

> A modular C mini-project that demonstrates core operating-system concepts through job scheduling, concurrency, process control, IPC-style workflows, resource-safety scenarios, and cleanup reporting.

![Language](https://img.shields.io/badge/language-C%2011-A8B9CC?style=flat-square&logo=c)
![Platform](https://img.shields.io/badge/platform-POSIX-2F74C0?style=flat-square)
![Build](https://img.shields.io/badge/build-Make-6D4C41?style=flat-square)

## Highlights

- **FCFS**, **Round Robin**, and **non-preemptive Priority** scheduling
- Per-job completion, turnaround, waiting, and response-time metrics
- Deterministic scheduling order and a bundled regression test
- Threaded logging, worker-process lifecycle, IPC-style, Banker's-algorithm, resource-limit, and cleanup demonstrations
- Small, focused C modules with one command-line executable

## Quick start

### Requirements

- A POSIX-compatible environment (Linux, macOS, or WSL)
- `gcc`, `make`, and POSIX threads support

### Build and test

```sh
make
make test
```

This produces the `campuscompute` executable. The regression test checks the expected average waiting and turnaround times for all three schedulers using the included sample workload.

## Run it

```sh
./campuscompute help
./campuscompute all
```

`all` runs the bundled demonstrations in sequence and prints an integrated pass/fail summary.

## Scheduling

The project reads CSV records in this format:

```text
JobID,Arrival,Burst,Priority
```

The included [`jobs.csv`](jobs.csv) contains four sample jobs. Blank lines and lines beginning with `#` are ignored. Up to 64 jobs are supported; job IDs and burst times must be positive, while arrival times and priorities must be non-negative.

| Policy | Command | Behaviour |
| --- | --- | --- |
| First-Come, First-Served | `./campuscompute fcfs jobs.csv` | Runs jobs in arrival order. |
| Round Robin | `./campuscompute rr 2 jobs.csv` | Uses the supplied positive time quantum. |
| Priority | `./campuscompute priority jobs.csv` | Non-preemptive; a smaller priority number runs first. |
| Compare all policies | `./campuscompute compare jobs.csv` | Runs FCFS, Round Robin (quantum 2), then Priority. |

For equivalent arrivals or priorities, ties are resolved by arrival time and then job ID. Each scheduler prints a timeline plus completion, turnaround, waiting, and response metrics.

## Other demonstrations

| Area | Command | What it demonstrates |
| --- | --- | --- |
| Concurrent logging | `./campuscompute logger 4` | Producer and consumer threads; writes `campuscompute.log`. |
| Process communication | `./campuscompute ipc` | A parent/child pipe exchange and IPC cleanup status messages. |
| Deadlock safety | `./campuscompute banker safe` | A safe-state Banker's-algorithm scenario. |
| Deadlock safety | `./campuscompute banker unsafe` | An unsafe-state scenario without a complete safe sequence. |
| Worker lifecycle | `./campuscompute worker normal` | Forks a worker and reports normal completion. |
| Failure handling | `./campuscompute worker fail` | Forks a worker that exits with controlled status `7`. |
| Resource limits | `./campuscompute limit` | Reports the resource-limit demonstration outcome. |
| Cleanup | `./campuscompute cleanup` | Reports child, thread, and IPC cleanup status. |

## Commands at a glance

```text
./campuscompute help
./campuscompute fcfs jobs.csv
./campuscompute rr QUANTUM jobs.csv
./campuscompute priority jobs.csv
./campuscompute compare jobs.csv
./campuscompute logger EVENTS
./campuscompute ipc
./campuscompute banker safe|unsafe
./campuscompute worker normal|fail
./campuscompute limit
./campuscompute cleanup
./campuscompute all
```

`QUANTUM` accepts integers from 1 to 100000; `EVENTS` accepts integers from 1 to 1000.

## Project layout

```text
CampusCompute/
├── campuscompute.c       # CLI and command dispatch
├── scheduling.c          # CSV parsing and scheduling algorithms
├── logger.c              # Threaded logger demonstration
├── ipc.c                 # Parent/child pipe exchange
├── banker.c              # Safe and unsafe Banker scenarios
├── worker.c              # Child-process lifecycle scenarios
├── resource_limit.c      # Resource-limit and cleanup reports
├── campuscompute.h       # Shared types and declarations
├── jobs.csv              # Sample job workload
├── tests/regression.sh   # Scheduling regression checks
└── Makefile              # Build, test, and clean targets
```

## Clean generated files

```sh
make clean
```

This removes object files, `campuscompute`, and `campuscompute.log`.

---

Built as an Operating Systems mini-project for demonstrating scheduling and process-management concepts in C.

#!/bin/sh
set -eu
fcfs=$(./campuscompute fcfs jobs.csv)
rr=$(./campuscompute rr 2 jobs.csv)
priority=$(./campuscompute priority jobs.csv)
printf '%s\n' "$fcfs" | grep -F 'Average waiting=5.25 | average turnaround=9.50' >/dev/null
printf '%s\n' "$rr" | grep -F 'Average waiting=7.00 | average turnaround=11.25' >/dev/null
printf '%s\n' "$priority" | grep -F 'Average waiting=3.50 | average turnaround=7.75' >/dev/null
echo 'Scheduling regression tests passed.'

#!/bin/env bash

function sb_do_jobhash()
{
    while read line; do
    # get jobid `Submitted batch job 14015`
    echo $line
    jobid=$(echo $line | awk '{print $4}')
    # get git hash
    git_hash=$(git rev-parse --short HEAD)
    echo "$jobid $git_hash" >> log/jobhash.txt
    done
}

function sb()
{
    sbatch $1 | sb_do_jobhash
}
#!/bin/bash

module-setup()
{

if [ "$1" = "help" ]; then
    echo "usage: module-setup group

Setup group softwares by environment module.

Special group name:
  help               show this help message
  list               list all available groups

Example config file:
[imsrg]
gsl/2.7.1
boost1.81.0
[kshell]
compiler/2023.0.0
mpi/2023.0.0
mkl/2023.0.0

Example usage:
module-setup imsrg
module-setup kshell"

elif [ "$1" = "list" ]; then
    if [ -e ~/.module-setup ]; then
        awk '/^\[.*\]$/{print substr($0, 2, length($0)-2)}' ~/.module-setup
    else
        echo "No group found"
    fi
else
    if [ -e ~/.module-setup ]; then
        if [ -z "$1" ]; then
            echo "No group specified"
        else
            if [ -z "$(grep "^\[$1\]$" ~/.module-setup)" ]; then
                echo "Group $1 not found"
            else
                local in_group=0
                while read line; do
                    if [ "$line" = "[$1]" ]; then
                        in_group=1
                        echo module purge
                        module purge
                    elif [ "$in_group" = "1" ] && [ ! -z "$(echo $line | grep '\[.*\]')" ]; then
                        break
                    elif [ "$in_group" = "1" ] && [ ! -z "$line" ]; then
                        echo module load $line
                        module load $line
                    fi
                done < ~/.module-setup
            fi
        fi
    else
        echo "No group found"
    fi
fi

}
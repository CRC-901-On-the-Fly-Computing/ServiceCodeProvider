#!/bin/bash

CPU_ONLY='false'

while getopts 'c' flag; do
	case "${flag}" in 
		c) CPU_ONLY='true' ;;
	esac
done

if [ $CPU_ONLY = 'true' ] ; then
	make cpu_only
else 
	make
fi

#!/bin/bash

CPU_ONLY='false'

while getopts 'c' flag; do
	case "${flag}" in 
		c) CPU_ONLY='true' ;;
	esac
done

if [ $CPU_ONLY  = 'true' ] ; then
	make -f Makefile.service_plugins_cpu_only all
else 
	make -f Makefile.service_plugins all
fi

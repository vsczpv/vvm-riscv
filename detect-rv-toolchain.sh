#!/bin/sh

# List of known toolchains
RISCV_PREFIXES="riscv64-elf
                riscv64-linux-gnu"

# Cicle through all prefixes
for prefix in $RISCV_PREFIXES ; do

	which $prefix-gcc > /dev/null

	# Prefix found
	if [ $? -eq '0' ] ; then
		echo $prefix
	fi

done

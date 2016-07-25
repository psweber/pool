#!/bin/bash

f="makeGraphics.sh"
rm -f $f
touch $f
chmod +x $f

echo "mkdir -p graphics" >> $f
ls graphics | while read g; do
	echo "if [ ! -f graphics/$g ]; then ln -s ../../latex/$g graphics/$g; fi"
done >> $f


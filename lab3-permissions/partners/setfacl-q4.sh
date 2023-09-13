#!/bin/sh
PARTNER=$1

# Set permissions for Q4
setfacl -m "user:$PARTNER:r--" shared.txt


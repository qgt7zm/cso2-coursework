#!/bin/sh
PARTNER=$1

# Reset permissions for Q4
chmod u=rwx dir
chmod u=rw dir/*

# Reset permissions for Q5
setfacl -x "user:$PARTNER" shared.txt


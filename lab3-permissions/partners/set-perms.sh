#!/bin/sh
PARTNER=$1

# Set permissions for Q4
chmod u=r dir/foo
chmod u-r dir/baz
chmod u=x dir

# Set permissions for Q5
setfacl -m "user:$PARTNER:r--" shared.txt


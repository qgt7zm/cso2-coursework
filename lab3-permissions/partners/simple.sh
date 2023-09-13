#!/bin/sh

# Q5
# Add the date to a file
# Source: https://www.baeldung.com/linux/date-command
echo "$(date)" >> simple_runs

# Count the number of lines
# Source: https://www.baeldung.com/linux/bash-count-lines-in-file
echo "There. Are. $(wc -l < simple_runs). Lines!"

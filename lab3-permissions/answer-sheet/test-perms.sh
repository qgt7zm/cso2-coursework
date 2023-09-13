#!/bin/sh

./set-perms.sh

# Q2: Read only
cat q2.txt
echo "Question" > q2.txt

# Q3: Write only
cat q3.txt
echo "Question 3" > q3.txt

# Q4: Execute only
./q4
cat q4

./q4.sh
cat q4.sh

# Q5: Execute only
./q5
echo "Question 5" > q5

./q5.sh
echo "Question 5" > q5.sh

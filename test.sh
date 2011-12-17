#!/bin/bash

export LD_LIBRARY_PATH=.

test()
{
  echo -n "$1" > /tmp/password.txt
  ./not24get_test < /tmp/password.txt
  ret=$?
  rm /tmp/password.txt
  return $ret
}

echo -n "test 1: "
test "horse battery staple correct" && echo "OK" || echo "failure"
echo -n "test 2: "
test "horse battery" 2> /dev/null && echo "failure" || echo "OK"
echo -n "test 3: "
test "Nu9sDj6@1ak," 2> /dev/null && echo "OK" || echo "failure"
echo -n "test 4: "
test "horse battery staple" 2> /dev/null && echo "failure" || echo "OK"

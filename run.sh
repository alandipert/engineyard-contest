#!/bin/bash

./engine2 | while read line; do
  num=$(echo "$line" | cut -f1)
  str=$(echo "$line" | cut -f2 | tr -d '"')
  curl -d "distance=$num&string=$str" http://engine.dipert.org/
  echo $line
done

#!/bin/bash
set -xe

javac *.java
case "$1" in
  rmireg) rmiregistry;;
  server) java DaytimeServer;;
  client) java DaytimeClient $2;;
esac

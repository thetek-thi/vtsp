#!/bin/bash
set -xe

javac *.java
case "$1" in
  rmireg) rmiregistry -J-Djava.rmi.server.logCalls=true;;
  server) java DaytimeServer;;
  client) java DaytimeClient $2;;
esac

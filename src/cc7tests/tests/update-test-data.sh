#!/bin/sh

set -e

rm ./test-data.generated/*
../../tools/conv-tool/data-converter.sh test-data.conf

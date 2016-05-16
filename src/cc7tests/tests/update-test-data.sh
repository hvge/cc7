#!/bin/bash

set -e

DEST_DIR=test-data.generated
CONVERT=../../tools/conv-tool/data-converter.sh

touch ${DEST_DIR}/foo
rm    ${DEST_DIR}/*
${CONVERT} test-data.conf

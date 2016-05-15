#!/bin/bash
#
# Copyright 2016 Juraj Durech <durech.juraj@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

VERBOSE=no
PROGNAME=`basename $0`
PROGDIR=`dirname $0`

log () {
	if [ "$VERBOSE" = "yes" ] ; then
        echo "$1"
    fi
}

panic () {
	echo "$PROGNAME: $1" 1>&2;
    exit 1
}

CONV_NAME="binary-data-converter"
CONV_SRC="$PROGDIR/data-converter.cpp"
CONV_BIN="$PROGDIR/$CONV_NAME"
CC=clang

test_data_converter () {
	log "Testing binary converter..."
	"$CONV_BIN" --test
	if [ $? -ne 42 ]; then
		panic "Seems that $CONV_NAME utility doesn't work properly."
	fi
}

build_data_converter() {
	# check if binary converter is up to date
	if [ -e "$CONV_BIN" ]; then
		if test $CONV_BIN -nt $CONV_SRC; then
			test_data_converter
			return
		fi
		log "Updating binary converter..."
	else
		log "Building binary converter..."
	fi
	# compile
	$CC $CONV_SRC -lc++ -std=c++11 -o $CONV_BIN
	if [ $? -ne 0 ]; then
		panic "Unable to compile binary converter!"
	fi
	# and test that binary
	test_data_converter
}

build_data_converter
# launch converter
$CONV_BIN -pn "$PROGNAME" "$@"

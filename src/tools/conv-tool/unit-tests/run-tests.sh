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

PROGNAME=`basename $0`
PROGDIR=`dirname $0`
APP="$PROGDIR/../data-converter.sh"

panic () {
	echo "-------------------------------------------------------------------------"
	echo "TEST FAIL: $1" 1>&2;
	echo "-------------------------------------------------------------------------"
    exit 1
}

MUST_FAIL () {
	eval "$@"
	if [ $? -eq 0 ]; then
		panic "Command must fail: $@"
	fi
}

MUST_PASS () {
	eval "$@"
	if [ $? -ne 0 ]; then
		panic "Command must pass: $@"
	fi
}

MUST_FAIL "$APP $PROGDIR/invalid-conf-01.conf"
MUST_FAIL "$APP $PROGDIR/invalid-conf-02.conf"
MUST_FAIL "$APP $PROGDIR/invalid-conf-03.conf"
MUST_FAIL "$APP $PROGDIR/invalid-conf-04.conf"
#MUST_FAIL "$APP $PROGDIR/invalid-conf-05.conf"

MUST_PASS "$APP $PROGDIR/valid-conf-01.conf"
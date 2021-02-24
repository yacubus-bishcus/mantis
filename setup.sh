#!/bin/bash
#
# name: setup.sh
# date: 17 Sep 14
# auth: Zach Hartwig
#
#
export MANTIS_TOPDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export PATH=$MANTIS_TOPDIR/scripts:$PATH
export LD_LIBRARY_PATH=$MANTIS_TOPDIR/lib:$LD_LIBRARY_PATH

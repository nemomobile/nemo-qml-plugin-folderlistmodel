#!/bin/sh

TMP_DIR=../nemo-qml-plugin-folderlistmodel-regression-test-build

FOLDERLISTMODEL_DIR=`dirname $0`

mkdir $TMP_DIR
cd    $TMP_DIR

CONFIG=""
#=============================
# disable the comment below to test open files
#CONFIG="DEFINES+=TEST_OPENFILES"


qmake $CONFIG $FOLDERLISTMODEL_DIR/test/regression/regression_folderlilstmodel.pro
make clean && make check

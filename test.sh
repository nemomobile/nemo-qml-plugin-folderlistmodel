#!/bin/sh

TMP_DIR=../nemo-qml-plugin-folderlistmodel-regression-test-build

mkdir $TMP_DIR
cd    $TMP_DIR

qmake ../nemo-qml-plugin-folderlistmodel/test/regression/regression_folderlilstmodel.pro
make clean && make check

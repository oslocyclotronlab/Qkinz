#!/bin/bash

if [ ${TRAVIS_OS_NAME} == "osx" ]; then
	/usr/local/opt/qt5/bin/qmake
else
	qmake
fi
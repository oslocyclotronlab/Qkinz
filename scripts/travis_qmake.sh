#!/bin/bash

if [ ${TRAVIS_OS_NAME} == "osx" ]; then
	/usr/local/opt/qt5/bin/qmake
else
	/usr/lib/x86_64-linux-gnu/qt5/bin/qmake
fi
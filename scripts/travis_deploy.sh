#!/bin/bash

if [ ${TRAVIS_OS_NAME} == "osx" ]; then
	/usr/local/opt/qt5/bin/macdeployqt app/Qkinz.app -dmg

else
	exit 0
fi
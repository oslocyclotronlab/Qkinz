TEMPLATE = subdirs
#	CONFIG += ordered

SUBDIRS = \
    src \
    app \
    tests

app.depends = src
tests.depends = src

QMAKE_DISTCLEAN += -r
QMAKE_DISTCLEAN +=  $$PWD/Makefile \
					$$PWD/src/Makefile \
					$$PWD/app/Makefile \
					$$PWD/tests/Makefile \
                    $$PWD/*.pro.* \
                    $$PWD/build
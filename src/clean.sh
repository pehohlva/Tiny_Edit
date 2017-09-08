#!/bin/bash
dir=$(pwd)
echo "$dir"
echo RUNNING Clean .... hight voltage...rm!!
rm -rf OasisEdit.app o .moc .obj .rcc bin mkspecs  lib Makefile .qmake.stash .qmake.cache
find . -name '.DS_Store' -type f -delete
find . -name 'Makefile' -type f -delete
find . -name 'Makefile.Release' -type f -delete
find . -name 'Makefile.Debug' -type f -delete
qmake && make && macdeployqt OasisEdit.app -dmg
# && macdeployqt OasisEdit.app


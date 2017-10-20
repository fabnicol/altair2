#!/bin/sh
LD_LIBRARY_PATH=/home/fab/Qt5.7/5.7/Src/qtbase/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
export LD_LIBRARY_PATH
QT_PLUGIN_PATH=/home/fab/Qt5.7/5.7/Src/qtbase/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec /home/fab/Qt5.7/5.7/Src/qtbase/bin/uic "$@"

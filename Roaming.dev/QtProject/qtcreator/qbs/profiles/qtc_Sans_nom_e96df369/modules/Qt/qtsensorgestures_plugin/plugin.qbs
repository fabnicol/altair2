import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qtsensorgestures_plugin"
    Depends { name: "Qt"; submodules: []}

    className: "QtSensorGesturePlugin"
    staticLibsDebug: ["C:/msys64/mingw64/qt5-static/lib/libQt5Sensorsd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred"]
    staticLibsRelease: ["C:/msys64/mingw64/qt5-static/lib/libQt5Sensors.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qtsensorgestures_plugind"
    libNameForLinkerRelease: "qtsensorgestures_plugin"
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/sensorgestures/libqtsensorgestures_plugind.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/sensorgestures/libqtsensorgestures_plugin.a"
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

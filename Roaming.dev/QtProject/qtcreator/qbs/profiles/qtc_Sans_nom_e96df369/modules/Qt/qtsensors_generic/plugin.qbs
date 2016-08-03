import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qtsensors_generic"
    Depends { name: "Qt"; submodules: []}

    className: "genericSensorPlugin"
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
    libNameForLinkerDebug: "qtsensors_genericd"
    libNameForLinkerRelease: "qtsensors_generic"
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/sensors/libqtsensors_genericd.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/sensors/libqtsensors_generic.a"
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

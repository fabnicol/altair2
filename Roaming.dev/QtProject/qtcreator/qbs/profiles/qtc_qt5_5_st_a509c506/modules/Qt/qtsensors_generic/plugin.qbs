import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qtsensors_generic"
    Depends { name: "Qt"; submodules: []}

    className: "genericSensorPlugin"
    staticLibsDebug: []
    staticLibsRelease: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5Sensors.a", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "z", "qtpcre"]
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
    libFilePathDebug: ""
    libFilePathRelease: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/plugins/sensors/libqtsensors_generic.a"
    cpp.libraryPaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib"]
    isStaticLibrary: true
}

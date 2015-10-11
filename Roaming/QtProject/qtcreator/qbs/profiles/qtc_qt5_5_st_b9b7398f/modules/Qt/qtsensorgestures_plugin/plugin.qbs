import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qtsensorgestures_plugin"
    Depends { name: "Qt"; submodules: []}

    className: "QtSensorGesturePlugin"
    staticLibsDebug: []
    staticLibsRelease: ["C:/Build/qt5/qtbase/lib/Qt5Sensors.lib", "C:/Build/qt5/qtbase/lib/Qt5Core.lib", "kernel32.lib", "user32.lib", "shell32.lib", "uuid.lib", "ole32.lib", "advapi32.lib", "ws2_32.lib", "mpr.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qtsensorgestures_plugind.lib"
    libNameForLinkerRelease: "qtsensorgestures_plugin.lib"
    libFilePathDebug: ""
    libFilePathRelease: "E:/Build/Qt5.4.1.static/qtbase/plugins/sensorgestures/qtsensorgestures_plugin.lib"
    cpp.libraryPaths: ["C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

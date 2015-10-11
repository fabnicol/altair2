import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qmldbg_local"
    Depends { name: "Qt"; submodules: []}

    className: "QLocalClientConnection"
    staticLibsDebug: []
    staticLibsRelease: ["c:/Build/qt5/qtbase/lib/Qt5Qml.lib", "c:/Build/qt5/qtbase/lib/Qt5Network.lib", "dnsapi.lib", "c:/Build/qt5/qtbase/lib/Qt5Core.lib", "kernel32.lib", "user32.lib", "shell32.lib", "uuid.lib", "ole32.lib", "advapi32.lib", "ws2_32.lib", "mpr.lib", "C:/Build/qt5/qtbase/lib/qtpcre.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qmldbg_locald.lib"
    libNameForLinkerRelease: "qmldbg_local.lib"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Build/qt5/qtbase/plugins/qmltooling/qmldbg_local.lib"
    cpp.libraryPaths: ["c:/Build/qt5/qtbase/lib", "C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

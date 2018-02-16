import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Network"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Networkd"
    libNameForLinkerRelease: "Qt5Network"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_NETWORK_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.6/qtbase/include", "C:/Users/Public/Dev/qt-5.6/qtbase/include/QtNetwork"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

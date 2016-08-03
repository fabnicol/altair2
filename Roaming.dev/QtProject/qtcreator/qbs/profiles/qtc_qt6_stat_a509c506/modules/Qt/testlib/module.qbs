import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Test"
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
    libNameForLinkerDebug: "Qt5Testd"
    libNameForLinkerRelease: "Qt5Test"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_TESTLIB_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.6/qtbase/include", "C:/Users/Public/Dev/qt-5.6/qtbase/include/QtTest"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

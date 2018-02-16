import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Concurrent"
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
    libNameForLinkerDebug: "Qt5Concurrentd"
    libNameForLinkerRelease: "Qt5Concurrent"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_CONCURRENT_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.6/qtbase/include", "C:/Users/Public/Dev/qt-5.6/qtbase/include/QtConcurrent"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "AxServer"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "axbase"]}

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
    libNameForLinkerDebug: "Qt5AxServerd"
    libNameForLinkerRelease: "Qt5AxServer"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_AXSERVER_LIB", "QAXSERVER"]
    cpp.includePaths: []
    cpp.libraryPaths: []
    isStaticLibrary: true
}

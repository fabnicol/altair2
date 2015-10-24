import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "AxContainer"
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
    libNameForLinkerDebug: "Qt5AxContainerd"
    libNameForLinkerRelease: "Qt5AxContainer"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_AXCONTAINER_LIB"]
    cpp.includePaths: []
    cpp.libraryPaths: []
    isStaticLibrary: true
}

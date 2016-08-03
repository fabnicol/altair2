import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Help"
    Depends { name: "Qt"; submodules: ["help"]}

    hasLibrary: false
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
    libNameForLinkerDebug: ""
    libNameForLinkerRelease: ""
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: []
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/include/QtHelp/5.5.0", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/include/QtHelp/5.5.0/QtHelp"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

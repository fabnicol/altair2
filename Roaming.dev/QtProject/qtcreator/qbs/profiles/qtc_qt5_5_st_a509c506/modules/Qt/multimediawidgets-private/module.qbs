import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "MultimediaWidgets"
    Depends { name: "Qt"; submodules: ["multimediawidgets"]}

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
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/include/QtMultimediaWidgets/5.5.0", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/include/QtMultimediaWidgets/5.5.0/QtMultimediaWidgets"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

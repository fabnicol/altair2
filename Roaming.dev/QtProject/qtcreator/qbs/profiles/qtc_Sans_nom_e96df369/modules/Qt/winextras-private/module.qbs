import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "WinExtras"
    Depends { name: "Qt"; submodules: ["winextras"]}

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
    cpp.includePaths: ["C:/msys64/mingw64/qt5-static/include/QtWinExtras/5.5.1", "C:/msys64/mingw64/qt5-static/include/QtWinExtras/5.5.1/QtWinExtras"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

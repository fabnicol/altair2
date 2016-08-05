import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "3DCollision"
    Depends { name: "Qt"; submodules: ["3dcollision"]}

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
    cpp.includePaths: ["C:/msys64/mingw64/qt5-static/include/Qt3DCollision/5.5.1", "C:/msys64/mingw64/qt5-static/include/Qt3DCollision/5.5.1/Qt3DCollision"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

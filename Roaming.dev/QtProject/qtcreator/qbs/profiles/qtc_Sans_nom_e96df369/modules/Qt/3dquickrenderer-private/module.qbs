import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "3DQuickRenderer"
    Depends { name: "Qt"; submodules: ["3dquickrenderer"]}

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
    cpp.includePaths: ["C:/msys64/mingw64/qt5-static/include/Qt3DQuickRenderer/5.5.1", "C:/msys64/mingw64/qt5-static/include/Qt3DQuickRenderer/5.5.1/Qt3DQuickRenderer"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

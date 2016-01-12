import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "3DRenderer"
    Depends { name: "Qt"; submodules: ["3drenderer"]}

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
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/Qt3DRenderer/5.5.0", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/Qt3DRenderer/5.5.0/Qt3DRenderer"]
    cpp.libraryPaths: []
    
}

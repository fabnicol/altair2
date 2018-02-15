import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "3DQuickRenderer"
    Depends { name: "Qt"; submodules: ["core", "gui", "qml", "3dcore", "3drenderer", "3dquick"]}

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
    libNameForLinkerDebug: "Qt53DQuickRendererd"
    libNameForLinkerRelease: "Qt53DQuickRenderer"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_3DQUICKRENDERER_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/Qt3DQuickRenderer"]
    cpp.libraryPaths: []
    
}

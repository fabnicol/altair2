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
    libNameForLinkerDebug: "Qt53DQuickRendererd.lib"
    libNameForLinkerRelease: "Qt53DQuickRenderer.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt53DQuickRendererd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt53DQuickRenderer.lib"
    cpp.defines: ["QT_3DQUICKRENDERER_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/Qt3DQuickRenderer"]
    cpp.libraryPaths: []
    
}

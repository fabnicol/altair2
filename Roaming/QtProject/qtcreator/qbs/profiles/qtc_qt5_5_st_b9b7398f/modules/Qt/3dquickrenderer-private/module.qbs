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
    cpp.includePaths: ["E:/Build/Qt5.4.1.static/qtbase/include/Qt3DQuickRenderer/5.5.0", "E:/Build/Qt5.4.1.static/qtbase/include/Qt3DQuickRenderer/5.5.0/Qt3DQuickRenderer"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

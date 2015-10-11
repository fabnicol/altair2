import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "3DQuick"
    Depends { name: "Qt"; submodules: ["core", "gui", "qml", "3dcore"]}

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
    libNameForLinkerDebug: "Qt53DQuickd.lib"
    libNameForLinkerRelease: "Qt53DQuick.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt53DQuickd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt53DQuick.lib"
    cpp.defines: ["QT_3DQUICK_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/Qt3DQuick"]
    cpp.libraryPaths: []
    
}

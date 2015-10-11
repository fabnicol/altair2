import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Designer"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "xml", "uiplugin"]}

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
    libNameForLinkerDebug: "Qt5Designerd.lib"
    libNameForLinkerRelease: "Qt5Designer.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5Designerd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5Designer.lib"
    cpp.defines: ["QT_DESIGNER_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtDesigner"]
    cpp.libraryPaths: []
    
}

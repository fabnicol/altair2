import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "CLucene"
    Depends { name: "Qt"; submodules: ["core"]}

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
    libNameForLinkerDebug: "Qt5CLucened.lib"
    libNameForLinkerRelease: "Qt5CLucene.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5CLucened.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5CLucene.lib"
    cpp.defines: ["QT_CLUCENE_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtCLucene", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtCLucene/5.5.0", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtCLucene/5.5.0/QtCLucene"]
    cpp.libraryPaths: []
    
}

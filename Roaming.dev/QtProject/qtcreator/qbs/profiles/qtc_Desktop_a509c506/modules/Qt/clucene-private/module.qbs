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
    libNameForLinkerDebug: "Qt5CLucened"
    libNameForLinkerRelease: "Qt5CLucene"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_CLUCENE_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtCLucene", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtCLucene/5.5.0", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtCLucene/5.5.0/QtCLucene"]
    cpp.libraryPaths: []
    
}

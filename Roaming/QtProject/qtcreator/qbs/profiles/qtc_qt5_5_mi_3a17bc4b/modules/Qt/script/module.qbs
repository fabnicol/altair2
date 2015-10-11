import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Script"
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
    libNameForLinkerDebug: "Qt5Scriptd"
    libNameForLinkerRelease: "Qt5Script"
    libFilePathDebug: "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Scriptd.a"
    libFilePathRelease: "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Script.a"
    cpp.defines: ["QT_SCRIPT_LIB"]
    cpp.includePaths: ["C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtScript"]
    cpp.libraryPaths: []
    
}

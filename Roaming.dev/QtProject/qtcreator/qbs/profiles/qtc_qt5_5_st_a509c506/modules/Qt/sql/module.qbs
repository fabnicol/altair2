import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Sql"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "z", "qtpcre"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Sqld"
    libNameForLinkerRelease: "Qt5Sql"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5Sql.a"
    cpp.defines: ["QT_SQL_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/include", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/include/QtSql"]
    cpp.libraryPaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib"]
    isStaticLibrary: true
}

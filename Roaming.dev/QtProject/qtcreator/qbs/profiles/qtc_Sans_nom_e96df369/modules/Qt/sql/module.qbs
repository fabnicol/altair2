import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Sql"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: ["C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred"]
    staticLibsRelease: ["C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre"]
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
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/lib/libQt5Sqld.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/lib/libQt5Sql.a"
    cpp.defines: ["QT_SQL_LIB"]
    cpp.includePaths: ["C:/msys64/mingw64/qt5-static/include", "C:/msys64/mingw64/qt5-static/include/QtSql"]
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

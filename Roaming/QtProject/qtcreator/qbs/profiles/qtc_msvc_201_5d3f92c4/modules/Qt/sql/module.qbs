import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Sql"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["c:/Build/qt5/qtbase/lib/Qt5Core.lib", "kernel32.lib", "user32.lib", "shell32.lib", "uuid.lib", "ole32.lib", "advapi32.lib", "ws2_32.lib", "mpr.lib", "C:/Build/qt5/qtbase/lib/qtpcre.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Sqld.lib"
    libNameForLinkerRelease: "Qt5Sql.lib"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Build/qt5/qtbase/lib/Qt5Sql.lib"
    cpp.defines: ["QT_SQL_LIB"]
    cpp.includePaths: ["C:/Build/qt5/qtbase/include", "C:/Build/qt5/qtbase/include/QtSql"]
    cpp.libraryPaths: ["c:/Build/qt5/qtbase/lib", "C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

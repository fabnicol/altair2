import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "PrintSupport"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"]}

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
    libNameForLinkerDebug: "Qt5PrintSupportd"
    libNameForLinkerRelease: "Qt5PrintSupport"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_PRINTSUPPORT_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.6/qtbase/include", "C:/Users/Public/Dev/qt-5.6/qtbase/include/QtPrintSupport"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

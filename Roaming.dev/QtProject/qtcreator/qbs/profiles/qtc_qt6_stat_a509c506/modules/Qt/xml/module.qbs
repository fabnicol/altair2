import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Xml"
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
    libNameForLinkerDebug: "Qt5Xmld"
    libNameForLinkerRelease: "Qt5Xml"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_XML_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.6/qtbase/include", "C:/Users/Public/Dev/qt-5.6/qtbase/include/QtXml"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

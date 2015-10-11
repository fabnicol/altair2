import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Xml"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["C:/Build/qt5/qtbase/lib/Qt5Core.lib", "kernel32.lib", "user32.lib", "shell32.lib", "uuid.lib", "ole32.lib", "advapi32.lib", "ws2_32.lib", "mpr.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Xmld.lib"
    libNameForLinkerRelease: "Qt5Xml.lib"
    libFilePathDebug: ""
    libFilePathRelease: "E:/Build/Qt5.4.1.static/qtbase/lib/Qt5Xml.lib"
    cpp.defines: ["QT_XML_LIB"]
    cpp.includePaths: ["E:/Build/Qt5.4.1.static/qtbase/include", "E:/Build/Qt5.4.1.static/qtbase/include/QtXml"]
    cpp.libraryPaths: ["C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "CLucene"
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
    libNameForLinkerDebug: "Qt5CLucened.lib"
    libNameForLinkerRelease: "Qt5CLucene.lib"
    libFilePathDebug: ""
    libFilePathRelease: "E:/Build/Qt5.4.1.static/qtbase/lib/Qt5CLucene.lib"
    cpp.defines: ["QT_CLUCENE_LIB"]
    cpp.includePaths: ["E:/Build/Qt5.4.1.static/qtbase/include", "E:/Build/Qt5.4.1.static/qtbase/include/QtCLucene", "E:/Build/Qt5.4.1.static/qtbase/include/QtCLucene/5.5.0", "E:/Build/Qt5.4.1.static/qtbase/include/QtCLucene/5.5.0/QtCLucene"]
    cpp.libraryPaths: ["C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

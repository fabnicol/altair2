import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Network"
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
    libNameForLinkerDebug: "Qt5Networkd.lib"
    libNameForLinkerRelease: "Qt5Network.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5Networkd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5Network.lib"
    cpp.defines: ["QT_NETWORK_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtNetwork"]
    cpp.libraryPaths: []
    
}

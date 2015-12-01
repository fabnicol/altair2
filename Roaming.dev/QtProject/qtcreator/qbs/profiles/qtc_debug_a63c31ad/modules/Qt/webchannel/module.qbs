import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "WebChannel"
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
    libNameForLinkerDebug: "Qt5WebChanneld"
    libNameForLinkerRelease: "Qt5WebChannel"
    libFilePathDebug: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5WebChanneld.a"
    libFilePathRelease: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5WebChannel.a"
    cpp.defines: ["QT_WEBCHANNEL_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtWebChannel"]
    cpp.libraryPaths: []
    
}

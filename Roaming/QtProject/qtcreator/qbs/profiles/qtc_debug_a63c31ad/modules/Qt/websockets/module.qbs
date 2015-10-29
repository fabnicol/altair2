import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "WebSockets"
    Depends { name: "Qt"; submodules: ["core", "network"]}

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
    libNameForLinkerDebug: "Qt5WebSocketsd"
    libNameForLinkerRelease: "Qt5WebSockets"
    libFilePathDebug: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5WebSocketsd.a"
    libFilePathRelease: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5WebSockets.a"
    cpp.defines: ["QT_WEBSOCKETS_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtWebSockets"]
    cpp.libraryPaths: []
    
}

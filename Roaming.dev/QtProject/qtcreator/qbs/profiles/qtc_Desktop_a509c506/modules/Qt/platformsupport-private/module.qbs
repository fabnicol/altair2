import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "PlatformSupport"
    Depends { name: "Qt"; submodules: ["core-private", "gui-private"]}

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
    libNameForLinkerDebug: "Qt5PlatformSupportd"
    libNameForLinkerRelease: "Qt5PlatformSupport"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: ["QT_PLATFORMSUPPORT_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtPlatformSupport", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtPlatformSupport/5.5.0", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtPlatformSupport/5.5.0/QtPlatformSupport"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

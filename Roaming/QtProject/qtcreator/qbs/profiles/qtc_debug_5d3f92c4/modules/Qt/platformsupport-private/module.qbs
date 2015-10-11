import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "PlatformSupport"
    Depends { name: "Qt"; submodules: ["core-private", "gui-private"]}

    hasLibrary: true
    staticLibsDebug: ["$$[QT_INSTALL_LIBS]/Qt5Guid.lib", "$$[QT_INSTALL_LIBS]/Qt5DBusd.lib", "$$[QT_INSTALL_LIBS]/Qt5Cored.lib", "$$[QT_INSTALL_LIBS]/qtfreetyped.lib"]
    staticLibsRelease: ["$$[QT_INSTALL_LIBS]/Qt5Gui.lib", "$$[QT_INSTALL_LIBS]/Qt5DBus.lib", "$$[QT_INSTALL_LIBS]/Qt5Core.lib", "$$[QT_INSTALL_LIBS]/qtfreetype.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5PlatformSupportd.lib"
    libNameForLinkerRelease: "Qt5PlatformSupport.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5PlatformSupportd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5PlatformSupport.lib"
    cpp.defines: ["QT_PLATFORMSUPPORT_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtPlatformSupport", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtPlatformSupport/5.5.0", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtPlatformSupport/5.5.0/QtPlatformSupport"]
    cpp.libraryPaths: ["$$[QT_INSTALL_LIBS]", "$$[QT_INSTALL_LIBS]"]
    isStaticLibrary: true
}

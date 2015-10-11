import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "AxBase"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"]}

    hasLibrary: true
    staticLibsDebug: ["$$[QT_INSTALL_LIBS]/Qt5Widgetsd.lib", "$$[QT_INSTALL_LIBS]/Qt5Guid.lib", "$$[QT_INSTALL_LIBS]/Qt5Cored.lib", "ole32.lib", "oleaut32.lib", "user32.lib", "gdi32.lib", "advapi32.lib"]
    staticLibsRelease: ["$$[QT_INSTALL_LIBS]/Qt5Widgets.lib", "$$[QT_INSTALL_LIBS]/Qt5Gui.lib", "$$[QT_INSTALL_LIBS]/Qt5Core.lib", "ole32.lib", "oleaut32.lib", "user32.lib", "gdi32.lib", "advapi32.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5AxBased.lib"
    libNameForLinkerRelease: "Qt5AxBase.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5AxBased.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5AxBase.lib"
    cpp.defines: ["QT_AXBASE_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/ActiveQt"]
    cpp.libraryPaths: ["$$[QT_INSTALL_LIBS]", "$$[QT_INSTALL_LIBS]"]
    isStaticLibrary: true
}

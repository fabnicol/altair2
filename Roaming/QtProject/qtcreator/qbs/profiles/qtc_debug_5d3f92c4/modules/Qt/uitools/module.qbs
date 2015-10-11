import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "UiTools"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "uiplugin"]}

    hasLibrary: true
    staticLibsDebug: ["$$[QT_INSTALL_LIBS]/Qt5Widgetsd.lib", "$$[QT_INSTALL_LIBS]/Qt5Guid.lib", "$$[QT_INSTALL_LIBS]/Qt5Cored.lib"]
    staticLibsRelease: ["$$[QT_INSTALL_LIBS]/Qt5Widgets.lib", "$$[QT_INSTALL_LIBS]/Qt5Gui.lib", "$$[QT_INSTALL_LIBS]/Qt5Core.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5UiToolsd.lib"
    libNameForLinkerRelease: "Qt5UiTools.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5UiToolsd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5UiTools.lib"
    cpp.defines: ["QT_UITOOLS_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtUiTools"]
    cpp.libraryPaths: ["$$[QT_INSTALL_LIBS]", "$$[QT_INSTALL_LIBS]"]
    isStaticLibrary: true
}

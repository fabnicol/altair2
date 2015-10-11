import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "AxContainer"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "axbase"]}

    hasLibrary: true
    staticLibsDebug: ["$$[QT_INSTALL_LIBS]/Qt5AxBased.lib", "ole32.lib", "oleaut32.lib", "user32.lib", "gdi32.lib", "advapi32.lib", "$$[QT_INSTALL_LIBS]/Qt5Widgetsd.lib", "$$[QT_INSTALL_LIBS]/Qt5Guid.lib", "$$[QT_INSTALL_LIBS]/Qt5Cored.lib"]
    staticLibsRelease: ["$$[QT_INSTALL_LIBS]/Qt5AxBase.lib", "ole32.lib", "oleaut32.lib", "user32.lib", "gdi32.lib", "advapi32.lib", "$$[QT_INSTALL_LIBS]/Qt5Widgets.lib", "$$[QT_INSTALL_LIBS]/Qt5Gui.lib", "$$[QT_INSTALL_LIBS]/Qt5Core.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5AxContainerd.lib"
    libNameForLinkerRelease: "Qt5AxContainer.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5AxContainerd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5AxContainer.lib"
    cpp.defines: ["QT_AXCONTAINER_LIB"]
    cpp.includePaths: []
    cpp.libraryPaths: ["$$[QT_INSTALL_LIBS]", "$$[QT_INSTALL_LIBS]", "$$[QT_INSTALL_LIBS]", "$$[QT_INSTALL_LIBS]"]
    isStaticLibrary: true
}

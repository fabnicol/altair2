import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "OpenGLExtensions"
    Depends { name: "Qt"; submodules: ["core", "gui"]}

    hasLibrary: true
    staticLibsDebug: ["$$[QT_INSTALL_LIBS]/Qt5Guid.lib", "$$[QT_INSTALL_LIBS]/Qt5Cored.lib"]
    staticLibsRelease: ["$$[QT_INSTALL_LIBS]/Qt5Gui.lib", "$$[QT_INSTALL_LIBS]/Qt5Core.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5OpenGLExtensionsd.lib"
    libNameForLinkerRelease: "Qt5OpenGLExtensions.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5OpenGLExtensionsd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5OpenGLExtensions.lib"
    cpp.defines: ["QT_OPENGLEXTENSIONS_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtOpenGLExtensions"]
    cpp.libraryPaths: ["$$[QT_INSTALL_LIBS]", "$$[QT_INSTALL_LIBS]"]
    isStaticLibrary: true
}

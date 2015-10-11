import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "OpenGLExtensions"
    Depends { name: "Qt"; submodules: ["core", "gui"]}

    hasLibrary: true
    staticLibsDebug: ["C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Guid.a", "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Cored.a"]
    staticLibsRelease: ["C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Gui.a", "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Core.a"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5OpenGLExtensionsd"
    libNameForLinkerRelease: "Qt5OpenGLExtensions"
    libFilePathDebug: "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5OpenGLExtensionsd.a"
    libFilePathRelease: "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5OpenGLExtensions.a"
    cpp.defines: ["QT_OPENGLEXTENSIONS_LIB"]
    cpp.includePaths: ["C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtOpenGLExtensions"]
    cpp.libraryPaths: ["C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib", "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib"]
    isStaticLibrary: true
}

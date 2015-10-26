import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "MultimediaWidgets"
    Depends { name: "Qt"; submodules: ["core", "gui", "multimedia", "widgets"]}

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
    libNameForLinkerDebug: "Qt5MultimediaWidgetsd"
    libNameForLinkerRelease: "Qt5MultimediaWidgets"
    libFilePathDebug: "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5MultimediaWidgetsd.a"
    libFilePathRelease: "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5MultimediaWidgets.a"
    cpp.defines: ["QT_MULTIMEDIAWIDGETS_LIB"]
    cpp.includePaths: ["C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtMultimediaWidgets"]
    cpp.libraryPaths: []
    
}

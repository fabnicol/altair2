import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Widgets"
    Depends { name: "Qt"; submodules: ["core", "gui"]}

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
    libNameForLinkerDebug: "Qt5Widgetsd"
    libNameForLinkerRelease: "Qt5Widgets"
    libFilePathDebug: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Widgetsd.a"
    libFilePathRelease: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Widgets.a"
    cpp.defines: ["QT_WIDGETS_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtWidgets"]
    cpp.libraryPaths: []
    
}

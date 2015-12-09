import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "QmlDevTools"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Core.a"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5QmlDevToolsd"
    libNameForLinkerRelease: "Qt5QmlDevTools"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5QmlDevTools.a"
    cpp.defines: ["QT_QMLDEVTOOLS_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtQmlDevTools", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtQmlDevTools/5.5.0", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtQmlDevTools/5.5.0/QtQmlDevTools"]
    cpp.libraryPaths: ["$$[QT_HOST_LIBS]"]
    isStaticLibrary: true
}

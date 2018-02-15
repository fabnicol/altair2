import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "QmlDevTools"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre"]
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
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/lib/libQt5QmlDevTools.a"
    cpp.defines: ["QT_QMLDEVTOOLS_LIB"]
    cpp.includePaths: ["C:/msys64/mingw64/qt5-static/include", "C:/msys64/mingw64/qt5-static/include/QtQmlDevTools", "C:/msys64/mingw64/qt5-static/include/QtQmlDevTools/5.5.1", "C:/msys64/mingw64/qt5-static/include/QtQmlDevTools/5.5.1/QtQmlDevTools"]
    cpp.libraryPaths: ["$$[QT_HOST_LIBS]"]
    isStaticLibrary: true
}

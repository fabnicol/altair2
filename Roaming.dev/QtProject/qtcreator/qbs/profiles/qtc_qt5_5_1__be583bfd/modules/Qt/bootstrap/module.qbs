import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Bootstrap"
    Depends { name: "Qt"; submodules: []}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["user32", "ole32", "advapi32"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Bootstrapd"
    libNameForLinkerRelease: "Qt5Bootstrap"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Bootstrap.a"
    cpp.defines: ["QT_BOOTSTRAP_LIB", "QT_BOOTSTRAPPED", "QT_LITE_UNICODE", "QT_NO_CAST_TO_ASCII", "QT_NO_CODECS", "QT_NO_DATASTREAM", "QT_NO_LIBRARY", "QT_NO_QOBJECT", "QT_NO_SYSTEMLOCALE", "QT_NO_THREAD", "QT_NO_UNICODETABLES", "QT_NO_USING_NAMESPACE", "QT_NO_DEPRECATED", "QT_NO_TRANSLATION", "QT_QMAKE_LOCATION=\\\"C:/Users/Public/Dev/msys64/home/Fab/Qt-builds/work/build-x86_64-s/qt-5.1.1-angle/qtbase/bin/qmake.exe\\\""]
    cpp.includePaths: []
    cpp.libraryPaths: ["C:/Users/Public/Dev/msys64/home/Fab/Qt-builds/toolchains_win32/mingw64/x86_64-w64-mingw32/lib", "C:/Users/Public/Dev/qt5-5.5.1-static/prerequisites-x86_64-s/lib"]
    isStaticLibrary: true
}

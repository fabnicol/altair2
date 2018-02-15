import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Bootstrap"
    Depends { name: "Qt"; submodules: []}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["user32", "ole32", "advapi32", "shell32"]
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
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/lib/libQt5Bootstrap.a"
    cpp.defines: ["QT_BOOTSTRAP_LIB", "QT_BOOTSTRAPPED", "QT_LITE_UNICODE", "QT_NO_CAST_TO_ASCII", "QT_NO_CODECS", "QT_NO_DATASTREAM", "QT_NO_LIBRARY", "QT_NO_QOBJECT", "QT_NO_SYSTEMLOCALE", "QT_NO_THREAD", "QT_NO_UNICODETABLES", "QT_NO_USING_NAMESPACE", "QT_NO_DEPRECATED", "QT_NO_TRANSLATION"]
    cpp.includePaths: ["C:/msys64/mingw64/qt5-static/include", "C:/msys64/mingw64/qt5-static/include/QtCore", "C:/msys64/mingw64/qt5-static/include/QtXml", "C:/msys64/mingw64/qt5-static/include/QtCore/5.5.1", "C:/msys64/mingw64/qt5-static/include/QtCore/5.5.1/QtCore", "C:/msys64/mingw64/qt5-static/include/QtXml/5.5.1", "C:/msys64/mingw64/qt5-static/include/QtXml/5.5.1/QtXml"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

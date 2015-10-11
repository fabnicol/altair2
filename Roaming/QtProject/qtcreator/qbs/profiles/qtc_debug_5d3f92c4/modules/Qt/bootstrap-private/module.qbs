import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Bootstrap"
    Depends { name: "Qt"; submodules: []}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["user32.lib", "ole32.lib", "advapi32.lib", "shell32.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Bootstrapd.lib"
    libNameForLinkerRelease: "Qt5Bootstrap.lib"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5Bootstrap.lib"
    cpp.defines: ["QT_BOOTSTRAP_LIB", "QT_BOOTSTRAPPED", "QT_LITE_UNICODE", "QT_NO_CAST_TO_ASCII", "QT_NO_CODECS", "QT_NO_DATASTREAM", "QT_NO_LIBRARY", "QT_NO_QOBJECT", "QT_NO_SYSTEMLOCALE", "QT_NO_THREAD", "QT_NO_UNICODETABLES", "QT_NO_USING_NAMESPACE", "QT_NO_DEPRECATED", "QT_NO_TRANSLATION"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtCore", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtXml", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtCore/5.5.0", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtCore/5.5.0/QtCore", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtXml/5.5.0", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtXml/5.5.0/QtXml"]
    cpp.libraryPaths: []
    isStaticLibrary: true
}

import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Declarative"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "script"]}

    hasLibrary: true
    staticLibsDebug: ["C:/msys64/mingw64/qt5-static/lib/libQt5Widgetsd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Guid.a", "comdlg32", "oleaut32", "imm32", "glu32", "opengl32", "qtharfbuzzngd", "C:/msys64/mingw64/qt5-static/lib/libQt5Scriptd.a", "winmm", "C:/msys64/mingw64/qt5-static/lib/libQt5Sqld.a", "C:/msys64/mingw64/qt5-static/lib/libQt5XmlPatternsd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Networkd.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred", "C:/msys64/mingw64/qt5-static/lib/libQt5XmlPatternsd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Networkd.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred"]
    staticLibsRelease: ["C:/msys64/mingw64/qt5-static/lib/libQt5Widgets.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "glu32", "opengl32", "qtharfbuzzng", "C:/msys64/mingw64/qt5-static/lib/libQt5Script.a", "winmm", "C:/msys64/mingw64/qt5-static/lib/libQt5Sql.a", "C:/msys64/mingw64/qt5-static/lib/libQt5XmlPatterns.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Network.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre", "C:/msys64/mingw64/qt5-static/lib/libQt5XmlPatterns.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Network.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Declaratived"
    libNameForLinkerRelease: "Qt5Declarative"
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/lib/libQt5Declaratived.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/lib/libQt5Declarative.a"
    cpp.defines: {
        var result = ["QT_DECLARATIVE_LIB"];
        if (qmlDebugging)
            result.push("QT_DECLARATIVE_DEBUG");
        return result;
    }
    cpp.includePaths: ["C:/msys64/mingw64/qt5-static/include", "C:/msys64/mingw64/qt5-static/include/QtDeclarative"]
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    property bool qmlDebugging: false
    property string qmlPath
    property string qmlImportsPath: ""
    isStaticLibrary: true
}

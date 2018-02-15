import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qdeclarativeview"
    Depends { name: "Qt"; submodules: []}

    className: "QDeclarativeViewPlugin"
    staticLibsDebug: []
    staticLibsRelease: ["C:/msys64/mingw64/qt5-static/lib/libQt5Declarative.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Sql.a", "C:/msys64/mingw64/qt5-static/lib/libQt5XmlPatterns.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Network.a", "dnsapi", "ssl", "crypto", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Widgets.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "glu32", "opengl32", "gdi32", "qtharfbuzzng", "C:/msys64/mingw64/qt5-static/lib/libQt5Script.a", "winmm", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qdeclarativeviewd"
    libNameForLinkerRelease: "qdeclarativeview"
    libFilePathDebug: ""
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/designer/libqdeclarativeview.a"
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

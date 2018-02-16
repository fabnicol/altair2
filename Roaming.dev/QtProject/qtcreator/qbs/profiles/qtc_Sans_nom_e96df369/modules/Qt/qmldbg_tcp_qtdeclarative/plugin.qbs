import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qmldbg_tcp_qtdeclarative"
    Depends { name: "Qt"; submodules: []}

    className: "QTcpServerConnection"
    staticLibsDebug: ["C:/msys64/mingw64/qt5-static/lib/libQt5Declaratived.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Sqld.a", "C:/msys64/mingw64/qt5-static/lib/libQt5XmlPatternsd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Widgetsd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Guid.a", "comdlg32", "oleaut32", "imm32", "glu32", "opengl32", "qtharfbuzzngd", "C:/msys64/mingw64/qt5-static/lib/libQt5Scriptd.a", "winmm", "C:/msys64/mingw64/qt5-static/lib/libQt5Networkd.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred"]
    staticLibsRelease: ["C:/msys64/mingw64/qt5-static/lib/libQt5Declarative.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Sql.a", "C:/msys64/mingw64/qt5-static/lib/libQt5XmlPatterns.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Widgets.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "glu32", "opengl32", "qtharfbuzzng", "C:/msys64/mingw64/qt5-static/lib/libQt5Script.a", "winmm", "C:/msys64/mingw64/qt5-static/lib/libQt5Network.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qmldbg_tcp_qtdeclaratived"
    libNameForLinkerRelease: "qmldbg_tcp_qtdeclarative"
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/qml1tooling/libqmldbg_tcp_qtdeclaratived.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/qml1tooling/libqmldbg_tcp_qtdeclarative.a"
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

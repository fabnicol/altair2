import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qmldbg_qtquick2"
    Depends { name: "Qt"; submodules: []}

    className: "QtQuick2Plugin"
    staticLibsDebug: ["C:/msys64/mingw64/qt5-static/lib/libQt5Quickd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Guid.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "qtharfbuzzngd", "C:/msys64/mingw64/qt5-static/lib/libQt5Qmld.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Networkd.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred"]
    staticLibsRelease: ["C:/msys64/mingw64/qt5-static/lib/libQt5Quick.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "qtharfbuzzng", "C:/msys64/mingw64/qt5-static/lib/libQt5Qml.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Network.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qmldbg_qtquick2d"
    libNameForLinkerRelease: "qmldbg_qtquick2"
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/qmltooling/libqmldbg_qtquick2d.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/qmltooling/libqmldbg_qtquick2.a"
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

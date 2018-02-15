import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qwindows"
    Depends { name: "Qt"; submodules: []}

    className: "QWindowsIntegrationPlugin"
    staticLibsDebug: ["winspool", "shlwapi", "C:/msys64/mingw64/qt5-static/lib/libQt5PlatformSupportd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5DBusd.a", "netapi32", "qtfreetyped", "C:/msys64/mingw64/qt5-static/lib/libQt5Guid.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "qtharfbuzzngd", "C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred", "qtfreetyped"]
    staticLibsRelease: ["winspool", "shlwapi", "C:/msys64/mingw64/qt5-static/lib/libQt5PlatformSupport.a", "C:/msys64/mingw64/qt5-static/lib/libQt5DBus.a", "netapi32", "qtfreetype", "C:/msys64/mingw64/qt5-static/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "qtharfbuzzng", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre", "qtfreetype"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qwindowsd"
    libNameForLinkerRelease: "qwindows"
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/platforms/libqwindowsd.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/platforms/libqwindows.a"
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

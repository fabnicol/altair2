import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "PlatformSupport"
    Depends { name: "Qt"; submodules: ["core-private", "gui-private"]}

    hasLibrary: true
    staticLibsDebug: ["C:/msys64/mingw64/qt5-static/lib/libQt5Guid.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "qtharfbuzzngd", "C:/msys64/mingw64/qt5-static/lib/libQt5DBusd.a", "netapi32", "C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred", "qtfreetyped"]
    staticLibsRelease: ["C:/msys64/mingw64/qt5-static/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "qtharfbuzzng", "C:/msys64/mingw64/qt5-static/lib/libQt5DBus.a", "netapi32", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre", "qtfreetype"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5PlatformSupportd"
    libNameForLinkerRelease: "Qt5PlatformSupport"
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/lib/libQt5PlatformSupportd.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/lib/libQt5PlatformSupport.a"
    cpp.defines: ["QT_PLATFORMSUPPORT_LIB"]
    cpp.includePaths: ["C:/msys64/mingw64/qt5-static/include", "C:/msys64/mingw64/qt5-static/include/QtPlatformSupport", "C:/msys64/mingw64/qt5-static/include/QtPlatformSupport/5.5.1", "C:/msys64/mingw64/qt5-static/include/QtPlatformSupport/5.5.1/QtPlatformSupport"]
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Test"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: ["C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32"]
    staticLibsRelease: ["C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Testd"
    libNameForLinkerRelease: "Qt5Test"
    libFilePathDebug: "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Testd.a"
    libFilePathRelease: "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Test.a"
    cpp.defines: ["QT_TESTLIB_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/include", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/include/QtTest"]
    cpp.libraryPaths: ["C:/Users/Public/Dev/msys64/home/Fab/Qt-builds/toolchains_win32/mingw64/x86_64-w64-mingw32/lib", "C:/Users/Public/Dev/qt5-5.5.1-static/prerequisites-x86_64-s/lib", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib", "C:/Users/Public/Dev/msys64/home/Fab/Qt-builds/toolchains_win32/mingw64/x86_64-w64-mingw32/lib", "C:/Users/Public/Dev/qt5-5.5.1-static/prerequisites-x86_64-s/lib", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib"]
    isStaticLibrary: true
}

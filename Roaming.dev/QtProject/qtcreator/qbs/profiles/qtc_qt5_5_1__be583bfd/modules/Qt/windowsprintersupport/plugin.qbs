import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "windowsprintersupport"
    Depends { name: "Qt"; submodules: []}

    className: "QWindowsPrinterSupportPlugin"
    staticLibsDebug: ["C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5PrintSupportd.a", "winspool", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Widgetsd.a", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Guid.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32"]
    staticLibsRelease: ["C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5PrintSupport.a", "winspool", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Widgets.a", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "windowsprintersupportd"
    libNameForLinkerRelease: "windowsprintersupport"
    libFilePathDebug: "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/plugins/printsupport/libwindowsprintersupportd.a"
    libFilePathRelease: "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/plugins/printsupport/libwindowsprintersupport.a"
    cpp.libraryPaths: ["C:/Users/Public/Dev/msys64/home/Fab/Qt-builds/toolchains_win32/mingw64/x86_64-w64-mingw32/lib", "C:/Users/Public/Dev/qt5-5.5.1-static/prerequisites-x86_64-s/lib", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib", "C:/Users/Public/Dev/msys64/home/Fab/Qt-builds/toolchains_win32/mingw64/x86_64-w64-mingw32/lib", "C:/Users/Public/Dev/qt5-5.5.1-static/prerequisites-x86_64-s/lib", "C:/Users/Public/Dev/qt5-5.5.1-static/Qt-5.1.1-s-x86_64-angle/lib"]
    isStaticLibrary: true
}

import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qminimal"
    Depends { name: "Qt"; submodules: []}

    className: "QMinimalIntegrationPlugin"
    staticLibsDebug: ["C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5PlatformSupportd.a", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5Guid.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32"]
    staticLibsRelease: ["C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5PlatformSupport.a", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qminimald"
    libNameForLinkerRelease: "qminimal"
    libFilePathDebug: "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/plugins/platforms/libqminimald.a"
    libFilePathRelease: "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/plugins/platforms/libqminimal.a"
    cpp.libraryPaths: ["C:/msys64/home/Fab/Qt-builds/toolchains_win32/mingw64/x86_64-w64-mingw32/lib", "C:/Users/Public/Dev/Qt5.5-builds/prerequisites-x86_64-s/lib", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib", "C:/msys64/home/Fab/Qt-builds/toolchains_win32/mingw64/x86_64-w64-mingw32/lib", "C:/Users/Public/Dev/Qt5.5-builds/prerequisites-x86_64-s/lib", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib"]
    isStaticLibrary: true
}

import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "AxContainer"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "axbase"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5AxBase.a", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5Widgets.a", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5Gui.a", "gdi32", "comdlg32", "oleaut32", "imm32", "winmm", "qtharfbuzzng", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "z", "qtpcre"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5AxContainerd"
    libNameForLinkerRelease: "Qt5AxContainer"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5AxContainer.a"
    cpp.defines: ["QT_AXCONTAINER_LIB"]
    cpp.includePaths: []
    cpp.libraryPaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib"]
    isStaticLibrary: true
}

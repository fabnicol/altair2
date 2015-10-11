import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "AxContainer"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "axbase"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["c:/Build/qt5/qtbase/lib/Qt5AxBase.lib", "c:/Build/qt5/qtbase/lib/Qt5Widgets.lib", "c:/Build/qt5/qtbase/lib/Qt5Gui.lib", "gdi32.lib", "comdlg32.lib", "oleaut32.lib", "imm32.lib", "winmm.lib", "c:/Build/qt5/qtbase/lib/qtharfbuzzng.lib", "c:/Build/qt5/qtbase/lib/Qt5Core.lib", "kernel32.lib", "user32.lib", "shell32.lib", "uuid.lib", "ole32.lib", "advapi32.lib", "ws2_32.lib", "mpr.lib", "C:/Build/qt5/qtbase/lib/qtpcre.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5AxContainerd.lib"
    libNameForLinkerRelease: "Qt5AxContainer.lib"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Build/qt5/qtbase/lib/Qt5AxContainer.lib"
    cpp.defines: ["QT_AXCONTAINER_LIB"]
    cpp.includePaths: []
    cpp.libraryPaths: ["c:/Build/qt5/qtbase/lib", "C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

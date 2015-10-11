import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qjp2"
    Depends { name: "Qt"; submodules: []}

    className: "QJp2Plugin"
    staticLibsDebug: []
    staticLibsRelease: ["c:/Build/qt5/qtbase/lib/Qt5Gui.lib", "gdi32.lib", "comdlg32.lib", "oleaut32.lib", "imm32.lib", "winmm.lib", "c:/Build/qt5/qtbase/lib/qtharfbuzzng.lib", "c:/Build/qt5/qtbase/lib/Qt5Core.lib", "kernel32.lib", "user32.lib", "shell32.lib", "uuid.lib", "ole32.lib", "advapi32.lib", "ws2_32.lib", "mpr.lib", "C:/Build/qt5/qtbase/lib/qtpcre.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qjp2d.lib"
    libNameForLinkerRelease: "qjp2.lib"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Build/qt5/qtbase/plugins/imageformats/qjp2.lib"
    cpp.libraryPaths: ["c:/Build/qt5/qtbase/lib", "C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

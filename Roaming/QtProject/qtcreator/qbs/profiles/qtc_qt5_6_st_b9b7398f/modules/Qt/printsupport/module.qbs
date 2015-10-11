import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "PrintSupport"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["c:/Build/qt5/qtbase/lib/Qt5Widgets.lib", "c:/Build/qt5/qtbase/lib/Qt5Gui.lib", "gdi32.lib", "comdlg32.lib", "oleaut32.lib", "imm32.lib", "winmm.lib", "c:/Build/qt5/qtbase/lib/qtharfbuzzng.lib", "c:/Build/qt5/qtbase/lib/Qt5Core.lib", "kernel32.lib", "user32.lib", "shell32.lib", "uuid.lib", "ole32.lib", "advapi32.lib", "ws2_32.lib", "mpr.lib", "C:/Build/qt5/qtbase/lib/qtpcre.lib", "winspool.lib", "comdlg32.lib", "gdi32.lib", "user32.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5PrintSupportd.lib"
    libNameForLinkerRelease: "Qt5PrintSupport.lib"
    libFilePathDebug: ""
    libFilePathRelease: "C:/Build/qt5/qtbase/lib/Qt5PrintSupport.lib"
    cpp.defines: ["QT_PRINTSUPPORT_LIB"]
    cpp.includePaths: ["C:/Build/qt5/qtbase/include", "C:/Build/qt5/qtbase/include/QtPrintSupport"]
    cpp.libraryPaths: ["c:/Build/qt5/qtbase/lib", "C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "ScriptTools"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["C:/Build/qt5/qtbase/lib/Qt5Core.lib", "mpr.lib", "libEGL.lib", "C:/Build/qt5/qtbase/lib/libGLESv2.lib", "libGLESv2.lib", "kernel32.lib", "shell32.lib", "uuid.lib", "comdlg32.lib", "oleaut32.lib", "imm32.lib", "winmm.lib", "ws2_32.lib", "ole32.lib", "advapi32.lib", "d3d9.lib", "dxguid.lib", "C:/Build/qt5/qtbase/lib/translator.lib", "C:/Build/qt5/qtbase/lib/preprocessor.lib", "gdi32.lib", "user32.lib", "C:/Build/qt5/qtbase/lib/Qt5Widgets.lib", "C:/Build/qt5/qtbase/lib/Qt5Gui.lib", "libEGL.lib", "C:/Build/qt5/qtbase/lib/libGLESv2.lib", "libGLESv2.lib", "comdlg32.lib", "oleaut32.lib", "imm32.lib", "d3d9.lib", "dxguid.lib", "C:/Build/qt5/qtbase/lib/translator.lib", "C:/Build/qt5/qtbase/lib/preprocessor.lib", "gdi32.lib", "C:/Build/qt5/qtbase/lib/qtharfbuzzng.lib", "C:/Build/qt5/qtbase/lib/Qt5Script.lib", "winmm.lib", "C:/Build/qt5/qtbase/lib/Qt5Core.lib", "kernel32.lib", "user32.lib", "shell32.lib", "uuid.lib", "ole32.lib", "advapi32.lib", "ws2_32.lib", "mpr.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5ScriptToolsd.lib"
    libNameForLinkerRelease: "Qt5ScriptTools.lib"
    libFilePathDebug: ""
    libFilePathRelease: "E:/Build/Qt5.4.1.static/qtbase/lib/Qt5ScriptTools.lib"
    cpp.defines: ["QT_SCRIPTTOOLS_LIB"]
    cpp.includePaths: ["E:/Build/Qt5.4.1.static/qtbase/include", "E:/Build/Qt5.4.1.static/qtbase/include/QtScriptTools"]
    cpp.libraryPaths: ["C:/Build/qt5/qtbase/lib", "C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

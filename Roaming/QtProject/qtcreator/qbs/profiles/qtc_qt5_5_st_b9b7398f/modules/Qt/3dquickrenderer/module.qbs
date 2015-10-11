import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "3DQuickRenderer"
    Depends { name: "Qt"; submodules: ["core", "gui", "qml", "3dcore", "3drenderer", "3dquick"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["C:/Build/qt5/qtbase/lib/Qt53DRenderer.lib", "C:/Build/qt5/qtbase/lib/Qt53DQuick.lib", "C:/Build/qt5/qtbase/lib/Qt53DCore.lib", "C:/Build/qt5/qtbase/lib/Qt5OpenGLExtensions.lib", "C:/Build/qt5/qtbase/lib/Qt5Gui.lib", "C:/Build/qt5/qtbase/lib/qtharfbuzzng.lib", "C:/Build/qt5/qtbase/lib/Qt5Qml.lib", "C:/Build/qt5/qtbase/lib/Qt5Network.lib", "dnsapi.lib", "C:/Build/qt5/qtbase/lib/Qt5Core.lib", "mpr.lib", "libEGL.lib", "C:/Build/qt5/qtbase/lib/libGLESv2.lib", "libGLESv2.lib", "kernel32.lib", "shell32.lib", "uuid.lib", "comdlg32.lib", "oleaut32.lib", "imm32.lib", "winmm.lib", "ws2_32.lib", "ole32.lib", "advapi32.lib", "d3d9.lib", "dxguid.lib", "C:/Build/qt5/qtbase/lib/translator.lib", "C:/Build/qt5/qtbase/lib/preprocessor.lib", "gdi32.lib", "user32.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt53DQuickRendererd.lib"
    libNameForLinkerRelease: "Qt53DQuickRenderer.lib"
    libFilePathDebug: ""
    libFilePathRelease: "E:/Build/Qt5.4.1.static/qtbase/lib/Qt53DQuickRenderer.lib"
    cpp.defines: ["QT_3DQUICKRENDERER_LIB"]
    cpp.includePaths: ["E:/Build/Qt5.4.1.static/qtbase/include", "E:/Build/Qt5.4.1.static/qtbase/include/Qt3DQuickRenderer"]
    cpp.libraryPaths: ["C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "QuickParticles"
    Depends { name: "Qt"; submodules: ["core-private", "gui-private", "qml-private", "quick-private"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: ["C:/Build/qt5/qtbase/lib/Qt5Quick.lib", "C:/Build/qt5/qtbase/lib/Qt5Gui.lib", "C:/Build/qt5/qtbase/lib/qtharfbuzzng.lib", "C:/Build/qt5/qtbase/lib/Qt5Qml.lib", "C:/Build/qt5/qtbase/lib/Qt5Network.lib", "dnsapi.lib", "C:/Build/qt5/qtbase/lib/Qt5Core.lib", "mpr.lib", "libEGL.lib", "C:/Build/qt5/qtbase/lib/libGLESv2.lib", "libGLESv2.lib", "kernel32.lib", "shell32.lib", "uuid.lib", "comdlg32.lib", "oleaut32.lib", "imm32.lib", "winmm.lib", "ws2_32.lib", "ole32.lib", "advapi32.lib", "d3d9.lib", "dxguid.lib", "C:/Build/qt5/qtbase/lib/translator.lib", "C:/Build/qt5/qtbase/lib/preprocessor.lib", "gdi32.lib", "user32.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5QuickParticlesd.lib"
    libNameForLinkerRelease: "Qt5QuickParticles.lib"
    libFilePathDebug: ""
    libFilePathRelease: "E:/Build/Qt5.4.1.static/qtbase/lib/Qt5QuickParticles.lib"
    cpp.defines: ["QT_QUICKPARTICLES_LIB"]
    cpp.includePaths: ["E:/Build/Qt5.4.1.static/qtbase/include", "E:/Build/Qt5.4.1.static/qtbase/include/QtQuickParticles", "E:/Build/Qt5.4.1.static/qtbase/include/QtQuickParticles/5.5.0", "E:/Build/Qt5.4.1.static/qtbase/include/QtQuickParticles/5.5.0/QtQuickParticles"]
    cpp.libraryPaths: ["C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

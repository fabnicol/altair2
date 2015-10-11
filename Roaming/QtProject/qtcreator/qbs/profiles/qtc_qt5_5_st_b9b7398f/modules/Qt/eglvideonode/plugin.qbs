import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "eglvideonode"
    Depends { name: "Qt"; submodules: []}

    className: "QSGVideoNodeFactory_EGL"
    staticLibsDebug: []
    staticLibsRelease: ["C:/Build/qt5/qtbase/lib/Qt5MultimediaQuick_p.lib", "C:/Build/qt5/qtbase/lib/Qt5Multimedia.lib", "C:/Build/qt5/qtbase/lib/Qt5Quick.lib", "C:/Build/qt5/qtbase/lib/Qt5Gui.lib", "C:/Build/qt5/qtbase/lib/qtharfbuzzng.lib", "C:/Build/qt5/qtbase/lib/Qt5Qml.lib", "C:/Build/qt5/qtbase/lib/Qt5Network.lib", "dnsapi.lib", "C:/Build/qt5/qtbase/lib/Qt5Core.lib", "mpr.lib", "libEGL.lib", "C:/Build/qt5/qtbase/lib/libGLESv2.lib", "libGLESv2.lib", "kernel32.lib", "shell32.lib", "uuid.lib", "comdlg32.lib", "oleaut32.lib", "imm32.lib", "winmm.lib", "ws2_32.lib", "ole32.lib", "advapi32.lib", "d3d9.lib", "dxguid.lib", "C:/Build/qt5/qtbase/lib/translator.lib", "C:/Build/qt5/qtbase/lib/preprocessor.lib", "gdi32.lib", "user32.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "eglvideonoded.lib"
    libNameForLinkerRelease: "eglvideonode.lib"
    libFilePathDebug: ""
    libFilePathRelease: "E:/Build/Qt5.4.1.static/qtbase/plugins/video/videonode/eglvideonode.lib"
    cpp.libraryPaths: ["C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

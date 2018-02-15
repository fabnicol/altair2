import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "dsengine"
    Depends { name: "Qt"; submodules: []}

    className: "DSServicePlugin"
    staticLibsDebug: ["strmiids", "dmoguids", "msdmo", "C:/msys64/mingw64/qt5-static/lib/libQt5MultimediaWidgetsd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5OpenGLd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Multimediad.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Widgetsd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Guid.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "qtharfbuzzngd", "C:/msys64/mingw64/qt5-static/lib/libQt5Networkd.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred"]
    staticLibsRelease: ["strmiids", "dmoguids", "msdmo", "C:/msys64/mingw64/qt5-static/lib/libQt5MultimediaWidgets.a", "C:/msys64/mingw64/qt5-static/lib/libQt5OpenGL.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Multimedia.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Widgets.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "qtharfbuzzng", "C:/msys64/mingw64/qt5-static/lib/libQt5Network.a", "dnsapi", "ssl", "crypto", "gdi32", "crypt32", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "dsengined"
    libNameForLinkerRelease: "dsengine"
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/mediaservice/libdsengined.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/mediaservice/libdsengine.a"
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

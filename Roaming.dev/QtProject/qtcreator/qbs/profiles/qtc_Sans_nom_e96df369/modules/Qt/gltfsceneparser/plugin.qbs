import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "gltfsceneparser"
    Depends { name: "Qt"; submodules: []}

    className: "GLTFParser"
    staticLibsDebug: ["C:/msys64/mingw64/qt5-static/lib/libQt53DRendererd.a", "C:/msys64/mingw64/qt5-static/lib/libQt53DCored.a", "C:/msys64/mingw64/qt5-static/lib/libQt5OpenGLExtensionsd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Guid.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "qtharfbuzzngd", "C:/msys64/mingw64/qt5-static/lib/libQt5Concurrentd.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcred"]
    staticLibsRelease: ["C:/msys64/mingw64/qt5-static/lib/libQt53DRenderer.a", "C:/msys64/mingw64/qt5-static/lib/libQt53DCore.a", "C:/msys64/mingw64/qt5-static/lib/libQt5OpenGLExtensions.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Gui.a", "comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "qtharfbuzzng", "C:/msys64/mingw64/qt5-static/lib/libQt5Concurrent.a", "C:/msys64/mingw64/qt5-static/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "qtpcre"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "gltfsceneparserd"
    libNameForLinkerRelease: "gltfsceneparser"
    libFilePathDebug: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/sceneparsers/libgltfsceneparserd.a"
    libFilePathRelease: "C:/msys64/mingw64/qt5-static/share/qt5/plugins/sceneparsers/libgltfsceneparser.a"
    cpp.libraryPaths: ["C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib", "C:/msys64/mingw64/qt5-static/lib"]
    isStaticLibrary: true
}

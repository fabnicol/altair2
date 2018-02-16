import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qtuiotouchplugin"
    Depends { name: "Qt"; submodules: []}

    className: "QTuioTouchPlugin"
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qtuiotouchplugind"
    libNameForLinkerRelease: "qtuiotouchplugin"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.libraryPaths: []
    isStaticLibrary: true
}

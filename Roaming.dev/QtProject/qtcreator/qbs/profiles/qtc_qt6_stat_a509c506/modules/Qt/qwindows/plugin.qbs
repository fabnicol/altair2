import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qwindows"
    Depends { name: "Qt"; submodules: []}

    className: "QWindowsIntegrationPlugin"
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
    libNameForLinkerDebug: "qwindowsd"
    libNameForLinkerRelease: "qwindows"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.libraryPaths: []
    isStaticLibrary: true
}

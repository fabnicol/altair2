import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qminimal"
    Depends { name: "Qt"; submodules: []}

    className: "QMinimalIntegrationPlugin"
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
    libNameForLinkerDebug: "qminimald"
    libNameForLinkerRelease: "qminimal"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.libraryPaths: []
    isStaticLibrary: true
}

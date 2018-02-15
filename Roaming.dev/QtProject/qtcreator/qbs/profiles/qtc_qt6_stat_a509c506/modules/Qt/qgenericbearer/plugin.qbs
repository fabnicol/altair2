import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qgenericbearer"
    Depends { name: "Qt"; submodules: []}

    className: "QGenericEnginePlugin"
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
    libNameForLinkerDebug: "qgenericbearerd"
    libNameForLinkerRelease: "qgenericbearer"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.libraryPaths: []
    isStaticLibrary: true
}

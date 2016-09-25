import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qnativewifibearer"
    Depends { name: "Qt"; submodules: []}

    className: "QNativeWifiEnginePlugin"
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
    libNameForLinkerDebug: "qnativewifibearerd"
    libNameForLinkerRelease: "qnativewifibearer"
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.libraryPaths: []
    isStaticLibrary: true
}

import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qnativewifibearer"
    Depends { name: "Qt"; submodules: []}

    className: "QNativeWifiEnginePlugin"
    staticLibsDebug: []
    staticLibsRelease: ["C:/Build/qt5/qtbase/lib/Qt5Network.lib", "dnsapi.lib", "C:/Build/qt5/qtbase/lib/Qt5Core.lib", "kernel32.lib", "user32.lib", "shell32.lib", "uuid.lib", "ole32.lib", "advapi32.lib", "ws2_32.lib", "mpr.lib"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "qnativewifibearerd.lib"
    libNameForLinkerRelease: "qnativewifibearer.lib"
    libFilePathDebug: ""
    libFilePathRelease: "E:/Build/Qt5.4.1.static/qtbase/plugins/bearer/qnativewifibearer.lib"
    cpp.libraryPaths: ["C:/Build/qt5/qtbase/lib"]
    isStaticLibrary: true
}

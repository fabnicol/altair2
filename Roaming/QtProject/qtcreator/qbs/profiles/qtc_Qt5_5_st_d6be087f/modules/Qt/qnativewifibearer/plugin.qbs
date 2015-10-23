import qbs 1.0
import '../QtPlugin.qbs' as QtPlugin

QtPlugin {
    qtModuleName: "qnativewifibearer"
    Depends { name: "Qt"; submodules: []}

    className: "QNativeWifiEnginePlugin"
    staticLibsDebug: []
    staticLibsRelease: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5Network.a", "dnsapi", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32", "mpr", "z", "qtpcre"]
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
    libFilePathRelease: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/plugins/bearer/libqnativewifibearer.a"
    cpp.libraryPaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw52-static-runtime/lib"]
    isStaticLibrary: true
}

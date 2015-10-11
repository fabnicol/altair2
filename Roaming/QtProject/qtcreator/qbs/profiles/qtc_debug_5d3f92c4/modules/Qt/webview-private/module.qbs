import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "WebView"
    Depends { name: "Qt"; submodules: ["core", "gui", "webengine", "webengine-private", "core-private", "gui-private", "qml-private", "quick-private", "quick", "qml"]}

    hasLibrary: true
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
    libNameForLinkerDebug: "Qt5WebViewd.lib"
    libNameForLinkerRelease: "Qt5WebView.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5WebViewd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5WebView.lib"
    cpp.defines: ["QT_WEBVIEW_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtWebView", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtWebView/5.5.0", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtWebView/5.5.0/QtWebView"]
    cpp.libraryPaths: []
    
}

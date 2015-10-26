import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Designer"
    Depends { name: "Qt"; submodules: ["designer"]}

    hasLibrary: false
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
    libNameForLinkerDebug: ""
    libNameForLinkerRelease: ""
    libFilePathDebug: ""
    libFilePathRelease: ""
    cpp.defines: []
    cpp.includePaths: ["C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtDesigner/5.5.0", "C:/Qt/qt-5.5.0-x64-mingw510r0-seh-rev0/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtDesigner/5.5.0/QtDesigner"]
    cpp.libraryPaths: []
    
}

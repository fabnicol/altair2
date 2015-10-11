import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "QuickParticles"
    Depends { name: "Qt"; submodules: ["core-private", "gui-private", "qml-private", "quick-private"]}

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
    libNameForLinkerDebug: "Qt5QuickParticlesd.lib"
    libNameForLinkerRelease: "Qt5QuickParticles.lib"
    libFilePathDebug: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5QuickParticlesd.lib"
    libFilePathRelease: "C:/Qt/Qt5.5.0/5.5/msvc2013_64/lib/Qt5QuickParticles.lib"
    cpp.defines: ["QT_QUICKPARTICLES_LIB"]
    cpp.includePaths: ["C:/Qt/Qt5.5.0/5.5/msvc2013_64/include", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtQuickParticles", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtQuickParticles/5.5.0", "C:/Qt/Qt5.5.0/5.5/msvc2013_64/include/QtQuickParticles/5.5.0/QtQuickParticles"]
    cpp.libraryPaths: []
    
}

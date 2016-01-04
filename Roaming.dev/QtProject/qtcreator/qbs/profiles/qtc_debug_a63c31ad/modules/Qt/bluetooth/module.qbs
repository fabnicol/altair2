import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Bluetooth"
    Depends { name: "Qt"; submodules: ["core"]}

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
    libNameForLinkerDebug: "Qt5Bluetoothd"
    libNameForLinkerRelease: "Qt5Bluetooth"
    libFilePathDebug: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Bluetoothd.a"
    libFilePathRelease: "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/lib/libQt5Bluetooth.a"
    cpp.defines: ["QT_BLUETOOTH_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include", "C:/Users/Public/Dev/qt-5.5.0-x64-mingw510r0-seh-rev0/include/QtBluetooth"]
    cpp.libraryPaths: []
    
}

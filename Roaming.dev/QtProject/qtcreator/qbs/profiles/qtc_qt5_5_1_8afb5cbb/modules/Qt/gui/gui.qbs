import qbs 1.0
import qbs.FileInfo
import qbs.ModUtils
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Gui"

    property string uicName: "uic"

    FileTagger {
        patterns: ["*.ui"]
        fileTags: ["ui"]
    }

    Rule {
        inputs: ["ui"]

        Artifact {
            filePath: 'GeneratedFiles/ui_' + input.completeBaseName + '.h'
            fileTags: ["hpp"]
        }

        prepare: {
            var cmd = new Command(ModUtils.moduleProperty(product, "binPath") + '/'
                                  + ModUtils.moduleProperty(product, "uicName"),
                                  [input.filePath, '-o', output.filePath])
            cmd.description = 'uic ' + input.fileName;
            cmd.highlight = 'codegen';
            return cmd;
        }
    }

    property string defaultQpaPlugin: "qwindows"
    staticLibsDebug: ["comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5Cored.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32"]
    staticLibsRelease: ["comdlg32", "oleaut32", "imm32", "winmm", "glu32", "opengl32", "gdi32", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5Core.a", "ole32", "uuid", "ws2_32", "advapi32", "shell32", "user32", "kernel32"]
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Guid"
    libNameForLinkerRelease: "Qt5Gui"
    libFilePathDebug: "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5Guid.a"
    libFilePathRelease: "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib/libQt5Gui.a"

    cpp.defines: ["QT_GUI_LIB"]
    cpp.includePaths: ["C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/include", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/include/QtGui"]
    cpp.libraryPaths: ["C:/msys64/home/Fab/Qt-builds/toolchains_win32/mingw64/x86_64-w64-mingw32/lib", "C:/Users/Public/Dev/Qt5.5-builds/prerequisites-x86_64-s/lib", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib", "C:/msys64/home/Fab/Qt-builds/toolchains_win32/mingw64/x86_64-w64-mingw32/lib", "C:/Users/Public/Dev/Qt5.5-builds/prerequisites-x86_64-s/lib", "C:/Users/Public/Dev/Qt5.5-builds/Qt-5.1.1-s-x86_64-angle/lib"]

    Properties {
        condition: Qt.core.staticBuild && qbs.targetOS.contains("ios")
        cpp.frameworks: base.concat(["UIKit", "QuartzCore", "CoreText", "CoreGraphics",
                                     "Foundation", "CoreFoundation"])
    }
    cpp.frameworks: base
    isStaticLibrary: true
}


/****************************************************************************
** Meta object code from reading C++ file 'altair-gui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/altair-gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'altair-gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[15];
    char stringdata0[252];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 10), // "exitSignal"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 36), // "on_displayFileTreeViewButton_..."
QT_MOC_LITERAL(4, 60, 34), // "on_openManagerWidgetButton_cl..."
QT_MOC_LITERAL(5, 95, 28), // "on_editProjectButton_clicked"
QT_MOC_LITERAL(6, 124, 24), // "on_optionsButton_clicked"
QT_MOC_LITERAL(7, 149, 14), // "showMainWidget"
QT_MOC_LITERAL(8, 164, 9), // "configure"
QT_MOC_LITERAL(9, 174, 16), // "configureOptions"
QT_MOC_LITERAL(10, 191, 13), // "exportProject"
QT_MOC_LITERAL(11, 205, 3), // "str"
QT_MOC_LITERAL(12, 209, 14), // "archiveProject"
QT_MOC_LITERAL(13, 224, 14), // "restoreProject"
QT_MOC_LITERAL(14, 239, 12) // "resetCounter"

    },
    "MainWindow\0exitSignal\0\0"
    "on_displayFileTreeViewButton_clicked\0"
    "on_openManagerWidgetButton_clicked\0"
    "on_editProjectButton_clicked\0"
    "on_optionsButton_clicked\0showMainWidget\0"
    "configure\0configureOptions\0exportProject\0"
    "str\0archiveProject\0restoreProject\0"
    "resetCounter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  100,    2, 0x08 /* Private */,
       4,    0,  101,    2, 0x08 /* Private */,
       3,    1,  102,    2, 0x08 /* Private */,
       4,    1,  105,    2, 0x08 /* Private */,
       5,    0,  108,    2, 0x08 /* Private */,
       6,    0,  109,    2, 0x08 /* Private */,
       7,    0,  110,    2, 0x08 /* Private */,
       7,    1,  111,    2, 0x08 /* Private */,
       8,    0,  114,    2, 0x08 /* Private */,
       9,    0,  115,    2, 0x08 /* Private */,
      10,    1,  116,    2, 0x08 /* Private */,
      10,    0,  119,    2, 0x28 /* Private | MethodCloned */,
      12,    0,  120,    2, 0x08 /* Private */,
      13,    1,  121,    2, 0x08 /* Private */,
      13,    0,  124,    2, 0x28 /* Private | MethodCloned */,
      14,    0,  125,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,   11,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::QString,   11,
    QMetaType::Bool,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->exitSignal(); break;
        case 1: _t->on_displayFileTreeViewButton_clicked(); break;
        case 2: _t->on_openManagerWidgetButton_clicked(); break;
        case 3: _t->on_displayFileTreeViewButton_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_openManagerWidgetButton_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_editProjectButton_clicked(); break;
        case 6: _t->on_optionsButton_clicked(); break;
        case 7: _t->showMainWidget(); break;
        case 8: _t->showMainWidget((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->configure(); break;
        case 10: _t->configureOptions(); break;
        case 11: { bool _r = _t->exportProject((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->exportProject();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->archiveProject();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: { bool _r = _t->restoreProject((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->restoreProject();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: _t->resetCounter(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::exitSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::exitSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

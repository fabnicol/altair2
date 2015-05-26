/****************************************************************************
** Meta object code from reading C++ file 'altair.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/altair.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'altair.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Altair_t {
    QByteArrayData data[22];
    char stringdata0[317];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Altair_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Altair_t qt_meta_stringdata_Altair = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Altair"
QT_MOC_LITERAL(1, 7, 21), // "hasIndexChangedSignal"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 13), // "updateProject"
QT_MOC_LITERAL(4, 44, 28), // "on_openProjectButton_clicked"
QT_MOC_LITERAL(5, 73, 27), // "on_newProjectButton_clicked"
QT_MOC_LITERAL(6, 101, 21), // "on_deleteItem_clicked"
QT_MOC_LITERAL(7, 123, 3), // "run"
QT_MOC_LITERAL(8, 127, 10), // "runRAltair"
QT_MOC_LITERAL(9, 138, 15), // "processFinished"
QT_MOC_LITERAL(10, 154, 4), // "code"
QT_MOC_LITERAL(11, 159, 8), // "exitCode"
QT_MOC_LITERAL(12, 168, 8), // "addGroup"
QT_MOC_LITERAL(13, 177, 11), // "deleteGroup"
QT_MOC_LITERAL(14, 189, 11), // "killProcess"
QT_MOC_LITERAL(15, 201, 21), // "on_helpButton_clicked"
QT_MOC_LITERAL(16, 223, 18), // "requestSaveProject"
QT_MOC_LITERAL(17, 242, 16), // "writeProjectFile"
QT_MOC_LITERAL(18, 259, 16), // "assignGroupFiles"
QT_MOC_LITERAL(19, 276, 11), // "group_index"
QT_MOC_LITERAL(20, 288, 15), // "openProjectFile"
QT_MOC_LITERAL(21, 304, 12) // "closeProject"

    },
    "Altair\0hasIndexChangedSignal\0\0"
    "updateProject\0on_openProjectButton_clicked\0"
    "on_newProjectButton_clicked\0"
    "on_deleteItem_clicked\0run\0runRAltair\0"
    "processFinished\0code\0exitCode\0addGroup\0"
    "deleteGroup\0killProcess\0on_helpButton_clicked\0"
    "requestSaveProject\0writeProjectFile\0"
    "assignGroupFiles\0group_index\0"
    "openProjectFile\0closeProject"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Altair[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,  110,    2, 0x0a /* Public */,
       3,    0,  113,    2, 0x2a /* Public | MethodCloned */,
       4,    0,  114,    2, 0x0a /* Public */,
       5,    0,  115,    2, 0x0a /* Public */,
       6,    0,  116,    2, 0x0a /* Public */,
       7,    0,  117,    2, 0x08 /* Private */,
       8,    0,  118,    2, 0x08 /* Private */,
       9,    1,  119,    2, 0x08 /* Private */,
       9,    1,  122,    2, 0x08 /* Private */,
      12,    0,  125,    2, 0x08 /* Private */,
      13,    0,  126,    2, 0x08 /* Private */,
      14,    0,  127,    2, 0x08 /* Private */,
      15,    0,  128,    2, 0x08 /* Private */,
      16,    0,  129,    2, 0x08 /* Private */,
      17,    0,  130,    2, 0x08 /* Private */,
      18,    1,  131,    2, 0x08 /* Private */,
      20,    0,  134,    2, 0x08 /* Private */,
      21,    0,  135,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool, QMetaType::Bool,    2,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Altair::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Altair *_t = static_cast<Altair *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->hasIndexChangedSignal(); break;
        case 1: { bool _r = _t->updateProject((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->updateProject();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->on_openProjectButton_clicked(); break;
        case 4: _t->on_newProjectButton_clicked(); break;
        case 5: _t->on_deleteItem_clicked(); break;
        case 6: _t->run(); break;
        case 7: _t->runRAltair(); break;
        case 8: _t->processFinished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->processFinished((*reinterpret_cast< exitCode(*)>(_a[1]))); break;
        case 10: _t->addGroup(); break;
        case 11: _t->deleteGroup(); break;
        case 12: _t->killProcess(); break;
        case 13: _t->on_helpButton_clicked(); break;
        case 14: _t->requestSaveProject(); break;
        case 15: _t->writeProjectFile(); break;
        case 16: _t->assignGroupFiles((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 17: _t->openProjectFile(); break;
        case 18: _t->closeProject(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Altair::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Altair::hasIndexChangedSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Altair::staticMetaObject = {
    { &common::staticMetaObject, qt_meta_stringdata_Altair.data,
      qt_meta_data_Altair,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Altair::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Altair::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Altair.stringdata0))
        return static_cast<void*>(const_cast< Altair*>(this));
    return common::qt_metacast(_clname);
}

int Altair::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = common::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void Altair::hasIndexChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_FProgressBar_t {
    QByteArrayData data[3];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FProgressBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FProgressBar_t qt_meta_stringdata_FProgressBar = {
    {
QT_MOC_LITERAL(0, 0, 12), // "FProgressBar"
QT_MOC_LITERAL(1, 13, 4), // "stop"
QT_MOC_LITERAL(2, 18, 0) // ""

    },
    "FProgressBar\0stop\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FProgressBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void FProgressBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FProgressBar *_t = static_cast<FProgressBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject FProgressBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FProgressBar.data,
      qt_meta_data_FProgressBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FProgressBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FProgressBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FProgressBar.stringdata0))
        return static_cast<void*>(const_cast< FProgressBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int FProgressBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'options.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/options.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'options.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_standardPage_t {
    QByteArrayData data[7];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_standardPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_standardPage_t qt_meta_stringdata_standardPage = {
    {
QT_MOC_LITERAL(0, 0, 12), // "standardPage"
QT_MOC_LITERAL(1, 13, 27), // "on_processTypeWidgetChanged"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 28), // "on_openBaseDirButton_clicked"
QT_MOC_LITERAL(4, 71, 16), // "selectBaseOutput"
QT_MOC_LITERAL(5, 88, 27), // "on_openLogDirButton_clicked"
QT_MOC_LITERAL(6, 116, 15) // "selectLogOutput"

    },
    "standardPage\0on_processTypeWidgetChanged\0"
    "\0on_openBaseDirButton_clicked\0"
    "selectBaseOutput\0on_openLogDirButton_clicked\0"
    "selectLogOutput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_standardPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       3,    0,   42,    2, 0x08 /* Private */,
       4,    0,   43,    2, 0x08 /* Private */,
       5,    0,   44,    2, 0x08 /* Private */,
       6,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void standardPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        standardPage *_t = static_cast<standardPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_processTypeWidgetChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_openBaseDirButton_clicked(); break;
        case 2: _t->selectBaseOutput(); break;
        case 3: _t->on_openLogDirButton_clicked(); break;
        case 4: _t->selectLogOutput(); break;
        default: ;
        }
    }
}

const QMetaObject standardPage::staticMetaObject = {
    { &common::staticMetaObject, qt_meta_stringdata_standardPage.data,
      qt_meta_data_standardPage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *standardPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *standardPage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_standardPage.stringdata0))
        return static_cast<void*>(const_cast< standardPage*>(this));
    return common::qt_metacast(_clname);
}

int standardPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = common::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
struct qt_meta_stringdata_options_t {
    QByteArrayData data[3];
    char stringdata0[22];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_options_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_options_t qt_meta_stringdata_options = {
    {
QT_MOC_LITERAL(0, 0, 7), // "options"
QT_MOC_LITERAL(1, 8, 12), // "defaultClick"
QT_MOC_LITERAL(2, 21, 0) // ""

    },
    "options\0defaultClick\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_options[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void options::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        options *_t = static_cast<options *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->defaultClick((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (options::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&options::defaultClick)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject options::staticMetaObject = {
    { &common::staticMetaObject, qt_meta_stringdata_options.data,
      qt_meta_data_options,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *options::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *options::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_options.stringdata0))
        return static_cast<void*>(const_cast< options*>(this));
    return common::qt_metacast(_clname);
}

int options::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = common::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void options::defaultClick(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

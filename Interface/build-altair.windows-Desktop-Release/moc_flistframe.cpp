/****************************************************************************
** Meta object code from reading C++ file 'flistframe.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/flistframe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flistframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FListFrame_t {
    QByteArrayData data[12];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FListFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FListFrame_t qt_meta_stringdata_FListFrame = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FListFrame"
QT_MOC_LITERAL(1, 11, 16), // "is_ntabs_changed"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 18), // "is_ntracks_changed"
QT_MOC_LITERAL(4, 48, 11), // "deleteGroup"
QT_MOC_LITERAL(5, 60, 1), // "r"
QT_MOC_LITERAL(6, 62, 21), // "on_deleteItem_clicked"
QT_MOC_LITERAL(7, 84, 15), // "setSlotListSize"
QT_MOC_LITERAL(8, 100, 1), // "s"
QT_MOC_LITERAL(9, 102, 29), // "on_importFromMainTree_clicked"
QT_MOC_LITERAL(10, 132, 14), // "on_xhl_display"
QT_MOC_LITERAL(11, 147, 3) // "xhl"

    },
    "FListFrame\0is_ntabs_changed\0\0"
    "is_ntracks_changed\0deleteGroup\0r\0"
    "on_deleteItem_clicked\0setSlotListSize\0"
    "s\0on_importFromMainTree_clicked\0"
    "on_xhl_display\0xhl"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FListFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       3,    1,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   65,    2, 0x0a /* Public */,
       4,    1,   66,    2, 0x0a /* Public */,
       6,    0,   69,    2, 0x0a /* Public */,
       7,    1,   70,    2, 0x0a /* Public */,
       9,    0,   73,    2, 0x09 /* Protected */,
      10,    1,   74,    2, 0x08 /* Private */,
      10,    0,   77,    2, 0x28 /* Private | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,

       0        // eod
};

void FListFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FListFrame *_t = static_cast<FListFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->is_ntabs_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->is_ntracks_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->deleteGroup(); break;
        case 3: _t->deleteGroup((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_deleteItem_clicked(); break;
        case 5: _t->setSlotListSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_importFromMainTree_clicked(); break;
        case 7: _t->on_xhl_display((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->on_xhl_display(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FListFrame::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FListFrame::is_ntabs_changed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FListFrame::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FListFrame::is_ntracks_changed)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject FListFrame::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FListFrame.data,
      qt_meta_data_FListFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FListFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FListFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FListFrame.stringdata0))
        return static_cast<void*>(const_cast< FListFrame*>(this));
    return QWidget::qt_metacast(_clname);
}

int FListFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void FListFrame::is_ntabs_changed(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FListFrame::is_ntracks_changed(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

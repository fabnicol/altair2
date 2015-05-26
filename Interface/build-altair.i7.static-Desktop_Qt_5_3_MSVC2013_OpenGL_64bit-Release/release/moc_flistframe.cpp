/****************************************************************************
** Meta object code from reading C++ file 'flistframe.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/flistframe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flistframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FListFrame_t {
    QByteArrayData data[15];
    char stringdata0[201];
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
QT_MOC_LITERAL(4, 48, 22), // "isControlButtonClicked"
QT_MOC_LITERAL(5, 71, 11), // "deleteGroup"
QT_MOC_LITERAL(6, 83, 1), // "r"
QT_MOC_LITERAL(7, 85, 1), // "R"
QT_MOC_LITERAL(8, 87, 21), // "on_deleteItem_clicked"
QT_MOC_LITERAL(9, 109, 20), // "on_clearList_clicked"
QT_MOC_LITERAL(10, 130, 12), // "currentIndex"
QT_MOC_LITERAL(11, 143, 15), // "setSlotListSize"
QT_MOC_LITERAL(12, 159, 1), // "s"
QT_MOC_LITERAL(13, 161, 9), // "addGroups"
QT_MOC_LITERAL(14, 171, 29) // "on_importFromMainTree_clicked"

    },
    "FListFrame\0is_ntabs_changed\0\0"
    "is_ntracks_changed\0isControlButtonClicked\0"
    "deleteGroup\0r\0R\0on_deleteItem_clicked\0"
    "on_clearList_clicked\0currentIndex\0"
    "setSlotListSize\0s\0addGroups\0"
    "on_importFromMainTree_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FListFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       3,    1,   72,    2, 0x06 /* Public */,
       4,    0,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   76,    2, 0x0a /* Public */,
       5,    2,   77,    2, 0x0a /* Public */,
       8,    0,   82,    2, 0x0a /* Public */,
       9,    1,   83,    2, 0x0a /* Public */,
       9,    0,   86,    2, 0x2a /* Public | MethodCloned */,
      11,    1,   87,    2, 0x0a /* Public */,
      13,    1,   90,    2, 0x0a /* Public */,
      14,    0,   93,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,    2,
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
        case 2: _t->isControlButtonClicked(); break;
        case 3: _t->deleteGroup(); break;
        case 4: _t->deleteGroup((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->on_deleteItem_clicked(); break;
        case 6: _t->on_clearList_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_clearList_clicked(); break;
        case 8: _t->setSlotListSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->addGroups((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_importFromMainTree_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FListFrame::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FListFrame::is_ntabs_changed)) {
                *result = 0;
            }
        }
        {
            typedef void (FListFrame::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FListFrame::is_ntracks_changed)) {
                *result = 1;
            }
        }
        {
            typedef void (FListFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FListFrame::isControlButtonClicked)) {
                *result = 2;
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
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
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

// SIGNAL 2
void FListFrame::isControlButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

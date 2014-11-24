/****************************************************************************
** Meta object code from reading C++ file 'flistframe.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/flistframe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flistframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FListFrame_t {
    QByteArrayData data[17];
    char stringdata[259];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FListFrame_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FListFrame_t qt_meta_stringdata_FListFrame = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 16),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 18),
QT_MOC_LITERAL(4, 48, 22),
QT_MOC_LITERAL(5, 71, 11),
QT_MOC_LITERAL(6, 83, 1),
QT_MOC_LITERAL(7, 85, 1),
QT_MOC_LITERAL(8, 87, 21),
QT_MOC_LITERAL(9, 109, 20),
QT_MOC_LITERAL(10, 130, 12),
QT_MOC_LITERAL(11, 143, 15),
QT_MOC_LITERAL(12, 159, 1),
QT_MOC_LITERAL(13, 161, 29),
QT_MOC_LITERAL(14, 191, 29),
QT_MOC_LITERAL(15, 221, 27),
QT_MOC_LITERAL(16, 249, 9)
    },
    "FListFrame\0is_ntabs_changed\0\0"
    "is_ntracks_changed\0isControlButtonClicked\0"
    "deleteGroup\0r\0R\0on_deleteItem_clicked\0"
    "on_clearList_clicked\0currentIndex\0"
    "setSlotListSize\0s\0on_importFromMainTree_clicked\0"
    "on_moveDownItemButton_clicked\0"
    "on_moveUpItemButton_clicked\0addGroups"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FListFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       3,    1,   82,    2, 0x06 /* Public */,
       4,    0,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   86,    2, 0x0a /* Public */,
       5,    2,   87,    2, 0x0a /* Public */,
       8,    0,   92,    2, 0x0a /* Public */,
       9,    1,   93,    2, 0x0a /* Public */,
       9,    0,   96,    2, 0x2a /* Public | MethodCloned */,
      11,    1,   97,    2, 0x0a /* Public */,
      13,    0,  100,    2, 0x09 /* Protected */,
      14,    0,  101,    2, 0x09 /* Protected */,
      15,    0,  102,    2, 0x09 /* Protected */,
      16,    1,  103,    2, 0x09 /* Protected */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void FListFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FListFrame *_t = static_cast<FListFrame *>(_o);
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
        case 9: _t->on_importFromMainTree_clicked(); break;
        case 10: _t->on_moveDownItemButton_clicked(); break;
        case 11: _t->on_moveUpItemButton_clicked(); break;
        case 12: _t->addGroups((*reinterpret_cast< int(*)>(_a[1]))); break;
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
      qt_meta_data_FListFrame,  qt_static_metacall, 0, 0}
};


const QMetaObject *FListFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FListFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FListFrame.stringdata))
        return static_cast<void*>(const_cast< FListFrame*>(this));
    return QWidget::qt_metacast(_clname);
}

int FListFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void FListFrame::is_ntabs_changed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FListFrame::is_ntracks_changed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FListFrame::isControlButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE

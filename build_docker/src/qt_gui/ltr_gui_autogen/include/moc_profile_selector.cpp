/****************************************************************************
** Meta object code from reading C++ file 'profile_selector.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/qt_gui/profile_selector.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'profile_selector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ProfileSelector_t {
    const uint offsetsAndSize[18];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ProfileSelector_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ProfileSelector_t qt_meta_stringdata_ProfileSelector = {
    {
QT_MOC_LITERAL(0, 15), // "ProfileSelector"
QT_MOC_LITERAL(16, 5), // "close"
QT_MOC_LITERAL(22, 0), // ""
QT_MOC_LITERAL(23, 20), // "profilesIndexChanged"
QT_MOC_LITERAL(44, 5), // "index"
QT_MOC_LITERAL(50, 26), // "on_CopyFromDefault_pressed"
QT_MOC_LITERAL(77, 24), // "on_ImportProfile_pressed"
QT_MOC_LITERAL(102, 24), // "on_ExportProfile_pressed"
QT_MOC_LITERAL(127, 21) // "on_NewProfile_pressed"

    },
    "ProfileSelector\0close\0\0profilesIndexChanged\0"
    "index\0on_CopyFromDefault_pressed\0"
    "on_ImportProfile_pressed\0"
    "on_ExportProfile_pressed\0on_NewProfile_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProfileSelector[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x0a,    1 /* Public */,
       3,    1,   51,    2, 0x08,    2 /* Private */,
       5,    0,   54,    2, 0x08,    4 /* Private */,
       6,    0,   55,    2, 0x08,    5 /* Private */,
       7,    0,   56,    2, 0x08,    6 /* Private */,
       8,    0,   57,    2, 0x08,    7 /* Private */,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ProfileSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ProfileSelector *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { bool _r = _t->close();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->profilesIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->on_CopyFromDefault_pressed(); break;
        case 3: _t->on_ImportProfile_pressed(); break;
        case 4: _t->on_ExportProfile_pressed(); break;
        case 5: _t->on_NewProfile_pressed(); break;
        default: ;
        }
    }
}

const QMetaObject ProfileSelector::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ProfileSelector.offsetsAndSize,
    qt_meta_data_ProfileSelector,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ProfileSelector_t
, QtPrivate::TypeAndForceComplete<ProfileSelector, std::true_type>
, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *ProfileSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProfileSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProfileSelector.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProfileSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

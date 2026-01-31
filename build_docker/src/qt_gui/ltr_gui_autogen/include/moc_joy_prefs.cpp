/****************************************************************************
** Meta object code from reading C++ file 'joy_prefs.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/qt_gui/joy_prefs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'joy_prefs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_JoyPrefs_t {
    const uint offsetsAndSize[26];
    char stringdata0[217];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_JoyPrefs_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_JoyPrefs_t qt_meta_stringdata_JoyPrefs = {
    {
QT_MOC_LITERAL(0, 8), // "JoyPrefs"
QT_MOC_LITERAL(9, 23), // "on_PitchCombo_activated"
QT_MOC_LITERAL(33, 0), // ""
QT_MOC_LITERAL(34, 5), // "index"
QT_MOC_LITERAL(40, 21), // "on_YawCombo_activated"
QT_MOC_LITERAL(62, 22), // "on_RollCombo_activated"
QT_MOC_LITERAL(85, 20), // "on_TXCombo_activated"
QT_MOC_LITERAL(106, 20), // "on_TYCombo_activated"
QT_MOC_LITERAL(127, 20), // "on_TZCombo_activated"
QT_MOC_LITERAL(148, 19), // "on_JsButton_pressed"
QT_MOC_LITERAL(168, 22), // "on_EvdevButton_pressed"
QT_MOC_LITERAL(191, 23), // "on_PPSFreq_valueChanged"
QT_MOC_LITERAL(215, 1) // "i"

    },
    "JoyPrefs\0on_PitchCombo_activated\0\0"
    "index\0on_YawCombo_activated\0"
    "on_RollCombo_activated\0on_TXCombo_activated\0"
    "on_TYCombo_activated\0on_TZCombo_activated\0"
    "on_JsButton_pressed\0on_EvdevButton_pressed\0"
    "on_PPSFreq_valueChanged\0i"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_JoyPrefs[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x08,    1 /* Private */,
       4,    1,   71,    2, 0x08,    3 /* Private */,
       5,    1,   74,    2, 0x08,    5 /* Private */,
       6,    1,   77,    2, 0x08,    7 /* Private */,
       7,    1,   80,    2, 0x08,    9 /* Private */,
       8,    1,   83,    2, 0x08,   11 /* Private */,
       9,    0,   86,    2, 0x08,   13 /* Private */,
      10,    0,   87,    2, 0x08,   14 /* Private */,
      11,    1,   88,    2, 0x08,   15 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,

       0        // eod
};

void JoyPrefs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<JoyPrefs *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_PitchCombo_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->on_YawCombo_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->on_RollCombo_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->on_TXCombo_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->on_TYCombo_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->on_TZCombo_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->on_JsButton_pressed(); break;
        case 7: _t->on_EvdevButton_pressed(); break;
        case 8: _t->on_PPSFreq_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject JoyPrefs::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_JoyPrefs.offsetsAndSize,
    qt_meta_data_JoyPrefs,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_JoyPrefs_t
, QtPrivate::TypeAndForceComplete<JoyPrefs, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>


>,
    nullptr
} };


const QMetaObject *JoyPrefs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *JoyPrefs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_JoyPrefs.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int JoyPrefs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

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
    const uint offsetsAndSize[22];
    char stringdata0[176];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ProfileSelector_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ProfileSelector_t qt_meta_stringdata_ProfileSelector = {
    {
QT_MOC_LITERAL(0, 15), // "ProfileSelector"
QT_MOC_LITERAL(16, 14), // "profileChanged"
QT_MOC_LITERAL(31, 0), // ""
QT_MOC_LITERAL(32, 11), // "profileName"
QT_MOC_LITERAL(44, 5), // "close"
QT_MOC_LITERAL(50, 20), // "profilesIndexChanged"
QT_MOC_LITERAL(71, 5), // "index"
QT_MOC_LITERAL(77, 26), // "on_CopyFromDefault_pressed"
QT_MOC_LITERAL(104, 24), // "on_ImportProfile_pressed"
QT_MOC_LITERAL(129, 24), // "on_ExportProfile_pressed"
QT_MOC_LITERAL(154, 21) // "on_NewProfile_pressed"

    },
    "ProfileSelector\0profileChanged\0\0"
    "profileName\0close\0profilesIndexChanged\0"
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
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   59,    2, 0x0a,    3 /* Public */,
       5,    1,   60,    2, 0x08,    4 /* Private */,
       7,    0,   63,    2, 0x08,    6 /* Private */,
       8,    0,   64,    2, 0x08,    7 /* Private */,
       9,    0,   65,    2, 0x08,    8 /* Private */,
      10,    0,   66,    2, 0x08,    9 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Int,    6,
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
        case 0: _t->profileChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: { bool _r = _t->close();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->profilesIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->on_CopyFromDefault_pressed(); break;
        case 4: _t->on_ImportProfile_pressed(); break;
        case 5: _t->on_ExportProfile_pressed(); break;
        case 6: _t->on_NewProfile_pressed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ProfileSelector::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProfileSelector::profileChanged)) {
                *result = 0;
                return;
            }
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
, QtPrivate::TypeAndForceComplete<ProfileSelector, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
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
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ProfileSelector::profileChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

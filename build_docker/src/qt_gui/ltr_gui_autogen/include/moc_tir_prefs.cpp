/****************************************************************************
** Meta object code from reading C++ file 'tir_prefs.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/qt_gui/tir_prefs.h"
#include <QtNetwork/QSslError>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tir_prefs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TirPrefs_t {
    const uint offsetsAndSize[30];
    char stringdata0[325];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_TirPrefs_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_TirPrefs_t qt_meta_stringdata_TirPrefs = {
    {
QT_MOC_LITERAL(0, 8), // "TirPrefs"
QT_MOC_LITERAL(9, 12), // "pressRefresh"
QT_MOC_LITERAL(22, 0), // ""
QT_MOC_LITERAL(23, 28), // "on_TirThreshold_valueChanged"
QT_MOC_LITERAL(52, 1), // "i"
QT_MOC_LITERAL(54, 26), // "on_TirMinBlob_valueChanged"
QT_MOC_LITERAL(81, 26), // "on_TirMaxBlob_valueChanged"
QT_MOC_LITERAL(108, 31), // "on_TirStatusBright_valueChanged"
QT_MOC_LITERAL(140, 27), // "on_TirIrBright_valueChanged"
QT_MOC_LITERAL(168, 34), // "on_TirSignalizeStatus_stateCh..."
QT_MOC_LITERAL(203, 5), // "state"
QT_MOC_LITERAL(209, 31), // "on_TirUseGrayscale_stateChanged"
QT_MOC_LITERAL(241, 31), // "on_TirVideoOnDelay_valueChanged"
QT_MOC_LITERAL(273, 29), // "on_TirInstallFirmware_pressed"
QT_MOC_LITERAL(303, 21) // "TirFirmwareDLFinished"

    },
    "TirPrefs\0pressRefresh\0\0"
    "on_TirThreshold_valueChanged\0i\0"
    "on_TirMinBlob_valueChanged\0"
    "on_TirMaxBlob_valueChanged\0"
    "on_TirStatusBright_valueChanged\0"
    "on_TirIrBright_valueChanged\0"
    "on_TirSignalizeStatus_stateChanged\0"
    "state\0on_TirUseGrayscale_stateChanged\0"
    "on_TirVideoOnDelay_valueChanged\0"
    "on_TirInstallFirmware_pressed\0"
    "TirFirmwareDLFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TirPrefs[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    1,   81,    2, 0x08,    2 /* Private */,
       5,    1,   84,    2, 0x08,    4 /* Private */,
       6,    1,   87,    2, 0x08,    6 /* Private */,
       7,    1,   90,    2, 0x08,    8 /* Private */,
       8,    1,   93,    2, 0x08,   10 /* Private */,
       9,    1,   96,    2, 0x08,   12 /* Private */,
      11,    1,   99,    2, 0x08,   14 /* Private */,
      12,    1,  102,    2, 0x08,   16 /* Private */,
      13,    0,  105,    2, 0x08,   18 /* Private */,
      14,    1,  106,    2, 0x08,   19 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,

       0        // eod
};

void TirPrefs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TirPrefs *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->pressRefresh(); break;
        case 1: _t->on_TirThreshold_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->on_TirMinBlob_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->on_TirMaxBlob_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->on_TirStatusBright_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->on_TirIrBright_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->on_TirSignalizeStatus_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->on_TirUseGrayscale_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->on_TirVideoOnDelay_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->on_TirInstallFirmware_pressed(); break;
        case 10: _t->TirFirmwareDLFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TirPrefs::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TirPrefs::pressRefresh)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject TirPrefs::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_TirPrefs.offsetsAndSize,
    qt_meta_data_TirPrefs,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_TirPrefs_t
, QtPrivate::TypeAndForceComplete<TirPrefs, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>


>,
    nullptr
} };


const QMetaObject *TirPrefs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TirPrefs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TirPrefs.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TirPrefs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void TirPrefs::pressRefresh()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

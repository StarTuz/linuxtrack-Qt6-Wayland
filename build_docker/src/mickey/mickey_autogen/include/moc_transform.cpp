/****************************************************************************
** Meta object code from reading C++ file 'transform.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/mickey/transform.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'transform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MickeyCurveShow_t {
    const uint offsetsAndSize[6];
    char stringdata0[25];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MickeyCurveShow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MickeyCurveShow_t qt_meta_stringdata_MickeyCurveShow = {
    {
QT_MOC_LITERAL(0, 15), // "MickeyCurveShow"
QT_MOC_LITERAL(16, 7), // "resized"
QT_MOC_LITERAL(24, 0) // ""

    },
    "MickeyCurveShow\0resized\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MickeyCurveShow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void MickeyCurveShow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MickeyCurveShow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->resized(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MickeyCurveShow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeyCurveShow::resized)) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject MickeyCurveShow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MickeyCurveShow.offsetsAndSize,
    qt_meta_data_MickeyCurveShow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MickeyCurveShow_t
, QtPrivate::TypeAndForceComplete<MickeyCurveShow, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>



>,
    nullptr
} };


const QMetaObject *MickeyCurveShow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MickeyCurveShow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MickeyCurveShow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MickeyCurveShow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MickeyCurveShow::resized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_MickeysAxis_t {
    const uint offsetsAndSize[10];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MickeysAxis_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MickeysAxis_t qt_meta_stringdata_MickeysAxis = {
    {
QT_MOC_LITERAL(0, 11), // "MickeysAxis"
QT_MOC_LITERAL(12, 11), // "newSettings"
QT_MOC_LITERAL(24, 0), // ""
QT_MOC_LITERAL(25, 11), // "axisChanged"
QT_MOC_LITERAL(37, 17) // "curveShow_resized"

    },
    "MickeysAxis\0newSettings\0\0axisChanged\0"
    "curveShow_resized"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MickeysAxis[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   33,    2, 0x08,    2 /* Private */,
       4,    0,   34,    2, 0x08,    3 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MickeysAxis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MickeysAxis *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newSettings(); break;
        case 1: _t->axisChanged(); break;
        case 2: _t->curveShow_resized(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MickeysAxis::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeysAxis::newSettings)) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject MickeysAxis::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MickeysAxis.offsetsAndSize,
    qt_meta_data_MickeysAxis,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MickeysAxis_t
, QtPrivate::TypeAndForceComplete<MickeysAxis, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MickeysAxis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MickeysAxis::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MickeysAxis.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MickeysAxis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MickeysAxis::newSettings()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_MickeyTransform_t {
    const uint offsetsAndSize[2];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MickeyTransform_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MickeyTransform_t qt_meta_stringdata_MickeyTransform = {
    {
QT_MOC_LITERAL(0, 15) // "MickeyTransform"

    },
    "MickeyTransform"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MickeyTransform[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MickeyTransform::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject MickeyTransform::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MickeyTransform.offsetsAndSize,
    qt_meta_data_MickeyTransform,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MickeyTransform_t
, QtPrivate::TypeAndForceComplete<MickeyTransform, std::true_type>



>,
    nullptr
} };


const QMetaObject *MickeyTransform::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MickeyTransform::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MickeyTransform.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MickeyTransform::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

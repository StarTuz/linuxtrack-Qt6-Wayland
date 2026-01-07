/****************************************************************************
** Meta object code from reading C++ file 'mickey.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/mickey/mickey.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mickey.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MickeyApplyDialog_t {
    const uint offsetsAndSize[14];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MickeyApplyDialog_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MickeyApplyDialog_t qt_meta_stringdata_MickeyApplyDialog = {
    {
QT_MOC_LITERAL(0, 17), // "MickeyApplyDialog"
QT_MOC_LITERAL(18, 6), // "revert"
QT_MOC_LITERAL(25, 0), // ""
QT_MOC_LITERAL(26, 4), // "keep"
QT_MOC_LITERAL(31, 23), // "on_RevertButton_pressed"
QT_MOC_LITERAL(55, 21), // "on_KeepButton_pressed"
QT_MOC_LITERAL(77, 7) // "timeout"

    },
    "MickeyApplyDialog\0revert\0\0keep\0"
    "on_RevertButton_pressed\0on_KeepButton_pressed\0"
    "timeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MickeyApplyDialog[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x06,    1 /* Public */,
       3,    0,   45,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   46,    2, 0x08,    3 /* Private */,
       5,    0,   47,    2, 0x08,    4 /* Private */,
       6,    0,   48,    2, 0x08,    5 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MickeyApplyDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MickeyApplyDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->revert(); break;
        case 1: _t->keep(); break;
        case 2: _t->on_RevertButton_pressed(); break;
        case 3: _t->on_KeepButton_pressed(); break;
        case 4: _t->timeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MickeyApplyDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeyApplyDialog::revert)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MickeyApplyDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeyApplyDialog::keep)) {
                *result = 1;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject MickeyApplyDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_MickeyApplyDialog.offsetsAndSize,
    qt_meta_data_MickeyApplyDialog,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MickeyApplyDialog_t
, QtPrivate::TypeAndForceComplete<MickeyApplyDialog, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MickeyApplyDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MickeyApplyDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MickeyApplyDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MickeyApplyDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MickeyApplyDialog::revert()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MickeyApplyDialog::keep()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_MickeyCalibration_t {
    const uint offsetsAndSize[20];
    char stringdata0[123];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MickeyCalibration_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MickeyCalibration_t qt_meta_stringdata_MickeyCalibration = {
    {
QT_MOC_LITERAL(0, 17), // "MickeyCalibration"
QT_MOC_LITERAL(18, 11), // "recenterNow"
QT_MOC_LITERAL(30, 0), // ""
QT_MOC_LITERAL(31, 5), // "leave"
QT_MOC_LITERAL(37, 16), // "startCalibration"
QT_MOC_LITERAL(54, 17), // "finishCalibration"
QT_MOC_LITERAL(72, 17), // "cancelCalibration"
QT_MOC_LITERAL(90, 10), // "calStarted"
QT_MOC_LITERAL(101, 7), // "timeout"
QT_MOC_LITERAL(109, 13) // "cancelPressed"

    },
    "MickeyCalibration\0recenterNow\0\0leave\0"
    "startCalibration\0finishCalibration\0"
    "cancelCalibration\0calStarted\0timeout\0"
    "cancelPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MickeyCalibration[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       4,    0,   53,    2, 0x06,    3 /* Public */,
       5,    0,   54,    2, 0x06,    4 /* Public */,
       6,    1,   55,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   58,    2, 0x08,    7 /* Private */,
       9,    0,   59,    2, 0x08,    8 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MickeyCalibration::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MickeyCalibration *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->recenterNow((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->startCalibration(); break;
        case 2: _t->finishCalibration(); break;
        case 3: _t->cancelCalibration((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->timeout(); break;
        case 5: _t->cancelPressed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MickeyCalibration::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeyCalibration::recenterNow)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MickeyCalibration::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeyCalibration::startCalibration)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MickeyCalibration::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeyCalibration::finishCalibration)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MickeyCalibration::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeyCalibration::cancelCalibration)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject MickeyCalibration::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_MickeyCalibration.offsetsAndSize,
    qt_meta_data_MickeyCalibration,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MickeyCalibration_t
, QtPrivate::TypeAndForceComplete<MickeyCalibration, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MickeyCalibration::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MickeyCalibration::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MickeyCalibration.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MickeyCalibration::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void MickeyCalibration::recenterNow(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MickeyCalibration::startCalibration()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MickeyCalibration::finishCalibration()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MickeyCalibration::cancelCalibration(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_MickeyThread_t {
    const uint offsetsAndSize[12];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MickeyThread_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MickeyThread_t qt_meta_stringdata_MickeyThread = {
    {
QT_MOC_LITERAL(0, 12), // "MickeyThread"
QT_MOC_LITERAL(13, 7), // "clicked"
QT_MOC_LITERAL(21, 0), // ""
QT_MOC_LITERAL(22, 24), // "on_mouseHotKey_activated"
QT_MOC_LITERAL(47, 6), // "button"
QT_MOC_LITERAL(54, 7) // "pressed"

    },
    "MickeyThread\0clicked\0\0on_mouseHotKey_activated\0"
    "button\0pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MickeyThread[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    2,   27,    2, 0x0a,    2 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    4,    5,

       0        // eod
};

void MickeyThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MickeyThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->on_mouseHotKey_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MickeyThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeyThread::clicked)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MickeyThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_MickeyThread.offsetsAndSize,
    qt_meta_data_MickeyThread,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MickeyThread_t
, QtPrivate::TypeAndForceComplete<MickeyThread, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>


>,
    nullptr
} };


const QMetaObject *MickeyThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MickeyThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MickeyThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int MickeyThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MickeyThread::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_Mickey_t {
    const uint offsetsAndSize[36];
    char stringdata0[218];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Mickey_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Mickey_t qt_meta_stringdata_Mickey = {
    {
QT_MOC_LITERAL(0, 6), // "Mickey"
QT_MOC_LITERAL(7, 21), // "mouseHotKey_activated"
QT_MOC_LITERAL(29, 0), // ""
QT_MOC_LITERAL(30, 6), // "button"
QT_MOC_LITERAL(37, 7), // "pressed"
QT_MOC_LITERAL(45, 16), // "hotKey_activated"
QT_MOC_LITERAL(62, 2), // "id"
QT_MOC_LITERAL(65, 21), // "updateTimer_activated"
QT_MOC_LITERAL(87, 14), // "revertSettings"
QT_MOC_LITERAL(102, 12), // "keepSettings"
QT_MOC_LITERAL(115, 11), // "recenterNow"
QT_MOC_LITERAL(127, 5), // "leave"
QT_MOC_LITERAL(133, 16), // "startCalibration"
QT_MOC_LITERAL(150, 17), // "finishCalibration"
QT_MOC_LITERAL(168, 17), // "cancelCalibration"
QT_MOC_LITERAL(186, 10), // "calStarted"
QT_MOC_LITERAL(197, 13), // "screenResized"
QT_MOC_LITERAL(211, 6) // "screen"

    },
    "Mickey\0mouseHotKey_activated\0\0button\0"
    "pressed\0hotKey_activated\0id\0"
    "updateTimer_activated\0revertSettings\0"
    "keepSettings\0recenterNow\0leave\0"
    "startCalibration\0finishCalibration\0"
    "cancelCalibration\0calStarted\0screenResized\0"
    "screen"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mickey[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   74,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    2,   79,    2, 0x08,    4 /* Private */,
       7,    0,   84,    2, 0x08,    7 /* Private */,
       8,    0,   85,    2, 0x08,    8 /* Private */,
       9,    0,   86,    2, 0x08,    9 /* Private */,
      10,    1,   87,    2, 0x08,   10 /* Private */,
      12,    0,   90,    2, 0x08,   12 /* Private */,
      13,    0,   91,    2, 0x08,   13 /* Private */,
      14,    1,   92,    2, 0x08,   14 /* Private */,
      16,    1,   95,    2, 0x08,   16 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    6,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::Int,   17,

       0        // eod
};

void Mickey::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Mickey *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->mouseHotKey_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 1: _t->hotKey_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 2: _t->updateTimer_activated(); break;
        case 3: _t->revertSettings(); break;
        case 4: _t->keepSettings(); break;
        case 5: _t->recenterNow((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->startCalibration(); break;
        case 7: _t->finishCalibration(); break;
        case 8: _t->cancelCalibration((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->screenResized((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Mickey::*)(int , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Mickey::mouseHotKey_activated)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Mickey::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Mickey.offsetsAndSize,
    qt_meta_data_Mickey,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Mickey_t
, QtPrivate::TypeAndForceComplete<Mickey, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>


>,
    nullptr
} };


const QMetaObject *Mickey::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mickey::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Mickey.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Mickey::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Mickey::mouseHotKey_activated(int _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_MickeyGUI_t {
    const uint offsetsAndSize[50];
    char stringdata0[448];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MickeyGUI_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MickeyGUI_t qt_meta_stringdata_MickeyGUI = {
    {
QT_MOC_LITERAL(0, 9), // "MickeyGUI"
QT_MOC_LITERAL(10, 11), // "axisChanged"
QT_MOC_LITERAL(22, 0), // ""
QT_MOC_LITERAL(23, 4), // "show"
QT_MOC_LITERAL(28, 26), // "on_SensSlider_valueChanged"
QT_MOC_LITERAL(55, 3), // "val"
QT_MOC_LITERAL(59, 24), // "on_DZSlider_valueChanged"
QT_MOC_LITERAL(84, 27), // "on_CurveSlider_valueChanged"
QT_MOC_LITERAL(112, 31), // "on_SmoothingSlider_valueChanged"
QT_MOC_LITERAL(144, 21), // "on_RelativeCB_clicked"
QT_MOC_LITERAL(166, 7), // "checked"
QT_MOC_LITERAL(174, 21), // "on_AbsoluteCB_clicked"
QT_MOC_LITERAL(196, 24), // "on_StepOnly_stateChanged"
QT_MOC_LITERAL(221, 5), // "state"
QT_MOC_LITERAL(227, 22), // "on_ApplyButton_pressed"
QT_MOC_LITERAL(250, 26), // "on_CalibrateButton_pressed"
QT_MOC_LITERAL(277, 25), // "on_RecenterButton_pressed"
QT_MOC_LITERAL(303, 21), // "on_HelpButton_pressed"
QT_MOC_LITERAL(325, 34), // "on_CalibrationTimeout_valueCh..."
QT_MOC_LITERAL(360, 29), // "on_CenterTimeout_valueChanged"
QT_MOC_LITERAL(390, 28), // "on_MickeyTabs_currentChanged"
QT_MOC_LITERAL(419, 5), // "index"
QT_MOC_LITERAL(425, 12), // "updateHotKey"
QT_MOC_LITERAL(438, 6), // "prefId"
QT_MOC_LITERAL(445, 2) // "hk"

    },
    "MickeyGUI\0axisChanged\0\0show\0"
    "on_SensSlider_valueChanged\0val\0"
    "on_DZSlider_valueChanged\0"
    "on_CurveSlider_valueChanged\0"
    "on_SmoothingSlider_valueChanged\0"
    "on_RelativeCB_clicked\0checked\0"
    "on_AbsoluteCB_clicked\0on_StepOnly_stateChanged\0"
    "state\0on_ApplyButton_pressed\0"
    "on_CalibrateButton_pressed\0"
    "on_RecenterButton_pressed\0"
    "on_HelpButton_pressed\0"
    "on_CalibrationTimeout_valueChanged\0"
    "on_CenterTimeout_valueChanged\0"
    "on_MickeyTabs_currentChanged\0index\0"
    "updateHotKey\0prefId\0hk"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MickeyGUI[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  116,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,  117,    2, 0x0a,    2 /* Public */,
       4,    1,  118,    2, 0x08,    3 /* Private */,
       6,    1,  121,    2, 0x08,    5 /* Private */,
       7,    1,  124,    2, 0x08,    7 /* Private */,
       8,    1,  127,    2, 0x08,    9 /* Private */,
       9,    1,  130,    2, 0x08,   11 /* Private */,
      11,    1,  133,    2, 0x08,   13 /* Private */,
      12,    1,  136,    2, 0x08,   15 /* Private */,
      14,    0,  139,    2, 0x08,   17 /* Private */,
      15,    0,  140,    2, 0x08,   18 /* Private */,
      16,    0,  141,    2, 0x08,   19 /* Private */,
      17,    0,  142,    2, 0x08,   20 /* Private */,
      18,    1,  143,    2, 0x08,   21 /* Private */,
      19,    1,  146,    2, 0x08,   23 /* Private */,
      20,    1,  149,    2, 0x08,   25 /* Private */,
      22,    2,  152,    2, 0x08,   27 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   23,   24,

       0        // eod
};

void MickeyGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MickeyGUI *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->axisChanged(); break;
        case 1: _t->show(); break;
        case 2: _t->on_SensSlider_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->on_DZSlider_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->on_CurveSlider_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->on_SmoothingSlider_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->on_RelativeCB_clicked((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->on_AbsoluteCB_clicked((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->on_StepOnly_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->on_ApplyButton_pressed(); break;
        case 10: _t->on_CalibrateButton_pressed(); break;
        case 11: _t->on_RecenterButton_pressed(); break;
        case 12: _t->on_HelpButton_pressed(); break;
        case 13: _t->on_CalibrationTimeout_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->on_CenterTimeout_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->on_MickeyTabs_currentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->updateHotKey((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MickeyGUI::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MickeyGUI::axisChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MickeyGUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MickeyGUI.offsetsAndSize,
    qt_meta_data_MickeyGUI,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MickeyGUI_t
, QtPrivate::TypeAndForceComplete<MickeyGUI, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>


>,
    nullptr
} };


const QMetaObject *MickeyGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MickeyGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MickeyGUI.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MickeyGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void MickeyGUI::axisChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

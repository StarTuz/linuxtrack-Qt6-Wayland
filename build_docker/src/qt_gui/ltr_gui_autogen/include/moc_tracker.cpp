/****************************************************************************
** Meta object code from reading C++ file 'tracker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/qt_gui/tracker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tracker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Tracker_t {
    const uint offsetsAndSize[54];
    char stringdata0[248];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Tracker_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Tracker_t qt_meta_stringdata_Tracker = {
    {
QT_MOC_LITERAL(0, 7), // "Tracker"
QT_MOC_LITERAL(8, 12), // "stateChanged"
QT_MOC_LITERAL(21, 0), // ""
QT_MOC_LITERAL(22, 13), // "current_state"
QT_MOC_LITERAL(36, 8), // "newFrame"
QT_MOC_LITERAL(45, 11), // "frame_type*"
QT_MOC_LITERAL(57, 5), // "frame"
QT_MOC_LITERAL(63, 7), // "newPose"
QT_MOC_LITERAL(71, 23), // "linuxtrack_full_pose_t*"
QT_MOC_LITERAL(95, 8), // "raw_pose"
QT_MOC_LITERAL(104, 18), // "linuxtrack_pose_t*"
QT_MOC_LITERAL(123, 10), // "unfiltered"
QT_MOC_LITERAL(134, 4), // "pose"
QT_MOC_LITERAL(139, 11), // "axisChanged"
QT_MOC_LITERAL(151, 4), // "axis"
QT_MOC_LITERAL(156, 4), // "elem"
QT_MOC_LITERAL(161, 11), // "setCommonFF"
QT_MOC_LITERAL(173, 3), // "val"
QT_MOC_LITERAL(177, 8), // "initAxes"
QT_MOC_LITERAL(186, 11), // "quitRequest"
QT_MOC_LITERAL(198, 5), // "start"
QT_MOC_LITERAL(204, 8), // "QString&"
QT_MOC_LITERAL(213, 7), // "section"
QT_MOC_LITERAL(221, 5), // "pause"
QT_MOC_LITERAL(227, 6), // "wakeup"
QT_MOC_LITERAL(234, 8), // "recenter"
QT_MOC_LITERAL(243, 4) // "stop"

    },
    "Tracker\0stateChanged\0\0current_state\0"
    "newFrame\0frame_type*\0frame\0newPose\0"
    "linuxtrack_full_pose_t*\0raw_pose\0"
    "linuxtrack_pose_t*\0unfiltered\0pose\0"
    "axisChanged\0axis\0elem\0setCommonFF\0val\0"
    "initAxes\0quitRequest\0start\0QString&\0"
    "section\0pause\0wakeup\0recenter\0stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Tracker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   86,    2, 0x06,    1 /* Public */,
       4,    1,   89,    2, 0x06,    3 /* Public */,
       7,    3,   92,    2, 0x06,    5 /* Public */,
      13,    2,   99,    2, 0x06,    9 /* Public */,
      16,    1,  104,    2, 0x06,   12 /* Public */,
      18,    0,  107,    2, 0x06,   14 /* Public */,
      19,    0,  108,    2, 0x06,   15 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      20,    1,  109,    2, 0x0a,   16 /* Public */,
      23,    0,  112,    2, 0x0a,   18 /* Public */,
      24,    0,  113,    2, 0x0a,   19 /* Public */,
      25,    0,  114,    2, 0x0a,   20 /* Public */,
      26,    0,  115,    2, 0x0a,   21 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 10, 0x80000000 | 10,    9,   11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   14,   15,
    QMetaType::Void, QMetaType::Float,   17,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Tracker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Tracker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->newFrame((*reinterpret_cast< std::add_pointer_t<frame_type*>>(_a[1]))); break;
        case 2: _t->newPose((*reinterpret_cast< std::add_pointer_t<linuxtrack_full_pose_t*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<linuxtrack_pose_t*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<linuxtrack_pose_t*>>(_a[3]))); break;
        case 3: _t->axisChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->setCommonFF((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 5: _t->initAxes(); break;
        case 6: _t->quitRequest(); break;
        case 7: _t->start((*reinterpret_cast< std::add_pointer_t<QString&>>(_a[1]))); break;
        case 8: _t->pause(); break;
        case 9: _t->wakeup(); break;
        case 10: _t->recenter(); break;
        case 11: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Tracker::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Tracker::stateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Tracker::*)(frame_type * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Tracker::newFrame)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Tracker::*)(linuxtrack_full_pose_t * , linuxtrack_pose_t * , linuxtrack_pose_t * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Tracker::newPose)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Tracker::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Tracker::axisChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Tracker::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Tracker::setCommonFF)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Tracker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Tracker::initAxes)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Tracker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Tracker::quitRequest)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject Tracker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Tracker.offsetsAndSize,
    qt_meta_data_Tracker,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Tracker_t
, QtPrivate::TypeAndForceComplete<Tracker, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<frame_type *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<linuxtrack_full_pose_t *, std::false_type>, QtPrivate::TypeAndForceComplete<linuxtrack_pose_t *, std::false_type>, QtPrivate::TypeAndForceComplete<linuxtrack_pose_t *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Tracker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Tracker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Tracker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Tracker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Tracker::stateChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Tracker::newFrame(frame_type * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Tracker::newPose(linuxtrack_full_pose_t * _t1, linuxtrack_pose_t * _t2, linuxtrack_pose_t * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Tracker::axisChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Tracker::setCommonFF(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Tracker::initAxes()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Tracker::quitRequest()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

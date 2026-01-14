/****************************************************************************
** Meta object code from reading C++ file 'udp_bridge.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/qt_gui/udp_bridge.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udp_bridge.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UdpBridge_t {
    const uint offsetsAndSize[54];
    char stringdata0[239];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_UdpBridge_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_UdpBridge_t qt_meta_stringdata_UdpBridge = {
    {
QT_MOC_LITERAL(0, 9), // "UdpBridge"
QT_MOC_LITERAL(10, 13), // "statusChanged"
QT_MOC_LITERAL(24, 0), // ""
QT_MOC_LITERAL(25, 7), // "running"
QT_MOC_LITERAL(33, 5), // "error"
QT_MOC_LITERAL(39, 7), // "message"
QT_MOC_LITERAL(47, 15), // "commandReceived"
QT_MOC_LITERAL(63, 3), // "cmd"
QT_MOC_LITERAL(67, 5), // "start"
QT_MOC_LITERAL(73, 4), // "stop"
QT_MOC_LITERAL(78, 8), // "sendPose"
QT_MOC_LITERAL(87, 3), // "yaw"
QT_MOC_LITERAL(91, 5), // "pitch"
QT_MOC_LITERAL(97, 4), // "roll"
QT_MOC_LITERAL(102, 1), // "x"
QT_MOC_LITERAL(104, 1), // "y"
QT_MOC_LITERAL(106, 1), // "z"
QT_MOC_LITERAL(108, 11), // "sendCommand"
QT_MOC_LITERAL(120, 11), // "const char*"
QT_MOC_LITERAL(132, 18), // "onCommandReadyRead"
QT_MOC_LITERAL(151, 17), // "onProcessFinished"
QT_MOC_LITERAL(169, 8), // "exitCode"
QT_MOC_LITERAL(178, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(199, 10), // "exitStatus"
QT_MOC_LITERAL(210, 8), // "Protocol"
QT_MOC_LITERAL(219, 9), // "OpenTrack"
QT_MOC_LITERAL(229, 9) // "FreeTrack"

    },
    "UdpBridge\0statusChanged\0\0running\0error\0"
    "message\0commandReceived\0cmd\0start\0"
    "stop\0sendPose\0yaw\0pitch\0roll\0x\0y\0z\0"
    "sendCommand\0const char*\0onCommandReadyRead\0"
    "onProcessFinished\0exitCode\0"
    "QProcess::ExitStatus\0exitStatus\0"
    "Protocol\0OpenTrack\0FreeTrack"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UdpBridge[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       1,  101, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x06,    1 /* Public */,
       4,    1,   71,    2, 0x06,    3 /* Public */,
       6,    1,   74,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   77,    2, 0x0a,    7 /* Public */,
       9,    0,   78,    2, 0x0a,    8 /* Public */,
      10,    6,   79,    2, 0x0a,    9 /* Public */,
      17,    1,   92,    2, 0x0a,   16 /* Public */,
      19,    0,   95,    2, 0x08,   18 /* Private */,
      20,    2,   96,    2, 0x08,   19 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, 0x80000000 | 18,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 22,   21,   23,

 // enums: name, alias, flags, count, data
      24,   24, 0x0,    2,  106,

 // enum data: key, value
      25, uint(UdpBridge::OpenTrack),
      26, uint(UdpBridge::FreeTrack),

       0        // eod
};

void UdpBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UdpBridge *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->commandReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->start(); break;
        case 4: _t->stop(); break;
        case 5: _t->sendPose((*reinterpret_cast< std::add_pointer_t<float>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[6]))); break;
        case 6: _t->sendCommand((*reinterpret_cast< std::add_pointer_t<const char*>>(_a[1]))); break;
        case 7: _t->onCommandReadyRead(); break;
        case 8: _t->onProcessFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UdpBridge::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpBridge::statusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UdpBridge::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpBridge::error)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UdpBridge::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpBridge::commandReceived)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject UdpBridge::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UdpBridge.offsetsAndSize,
    qt_meta_data_UdpBridge,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_UdpBridge_t
, QtPrivate::TypeAndForceComplete<UdpBridge, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const char *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QProcess::ExitStatus, std::false_type>


>,
    nullptr
} };


const QMetaObject *UdpBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UdpBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UdpBridge.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UdpBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void UdpBridge::statusChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UdpBridge::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UdpBridge::commandReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

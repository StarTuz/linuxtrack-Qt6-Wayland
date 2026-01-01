/****************************************************************************
** Meta object code from reading C++ file 'tracker.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/qt_gui/tracker.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tracker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7TrackerE_t {};
} // unnamed namespace

template <> constexpr inline auto Tracker::qt_create_metaobjectdata<qt_meta_tag_ZN7TrackerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Tracker",
        "stateChanged",
        "",
        "current_state",
        "newFrame",
        "frame_type*",
        "frame",
        "newPose",
        "linuxtrack_full_pose_t*",
        "raw_pose",
        "linuxtrack_pose_t*",
        "unfiltered",
        "pose",
        "axisChanged",
        "axis",
        "elem",
        "setCommonFF",
        "val",
        "initAxes",
        "quitRequest",
        "start",
        "QString&",
        "section",
        "pause",
        "wakeup",
        "recenter",
        "stop"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'stateChanged'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'newFrame'
        QtMocHelpers::SignalData<void(frame_type *)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'newPose'
        QtMocHelpers::SignalData<void(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 }, { 0x80000000 | 10, 11 }, { 0x80000000 | 10, 12 },
        }}),
        // Signal 'axisChanged'
        QtMocHelpers::SignalData<void(int, int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 14 }, { QMetaType::Int, 15 },
        }}),
        // Signal 'setCommonFF'
        QtMocHelpers::SignalData<void(float)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 17 },
        }}),
        // Signal 'initAxes'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'quitRequest'
        QtMocHelpers::SignalData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'start'
        QtMocHelpers::SlotData<void(QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 21, 22 },
        }}),
        // Slot 'pause'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'wakeup'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'recenter'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'stop'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Tracker, qt_meta_tag_ZN7TrackerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Tracker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7TrackerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7TrackerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7TrackerE_t>.metaTypes,
    nullptr
} };

void Tracker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Tracker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->newFrame((*reinterpret_cast<std::add_pointer_t<frame_type*>>(_a[1]))); break;
        case 2: _t->newPose((*reinterpret_cast<std::add_pointer_t<linuxtrack_full_pose_t*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<linuxtrack_pose_t*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<linuxtrack_pose_t*>>(_a[3]))); break;
        case 3: _t->axisChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->setCommonFF((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 5: _t->initAxes(); break;
        case 6: _t->quitRequest(); break;
        case 7: _t->start((*reinterpret_cast<std::add_pointer_t<QString&>>(_a[1]))); break;
        case 8: _t->pause(); break;
        case 9: _t->wakeup(); break;
        case 10: _t->recenter(); break;
        case 11: _t->stop(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Tracker::*)(int )>(_a, &Tracker::stateChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Tracker::*)(frame_type * )>(_a, &Tracker::newFrame, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Tracker::*)(linuxtrack_full_pose_t * , linuxtrack_pose_t * , linuxtrack_pose_t * )>(_a, &Tracker::newPose, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Tracker::*)(int , int )>(_a, &Tracker::axisChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Tracker::*)(float )>(_a, &Tracker::setCommonFF, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Tracker::*)()>(_a, &Tracker::initAxes, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Tracker::*)()>(_a, &Tracker::quitRequest, 6))
            return;
    }
}

const QMetaObject *Tracker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Tracker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7TrackerE_t>.strings))
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
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Tracker::stateChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Tracker::newFrame(frame_type * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Tracker::newPose(linuxtrack_full_pose_t * _t1, linuxtrack_pose_t * _t2, linuxtrack_pose_t * _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3);
}

// SIGNAL 3
void Tracker::axisChanged(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void Tracker::setCommonFF(float _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
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

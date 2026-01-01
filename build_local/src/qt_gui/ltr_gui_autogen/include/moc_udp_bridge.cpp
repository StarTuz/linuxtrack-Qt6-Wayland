/****************************************************************************
** Meta object code from reading C++ file 'udp_bridge.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/qt_gui/udp_bridge.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udp_bridge.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9UdpBridgeE_t {};
} // unnamed namespace

template <> constexpr inline auto UdpBridge::qt_create_metaobjectdata<qt_meta_tag_ZN9UdpBridgeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "UdpBridge",
        "statusChanged",
        "",
        "running",
        "error",
        "message",
        "commandReceived",
        "cmd",
        "start",
        "stop",
        "sendPose",
        "yaw",
        "pitch",
        "roll",
        "x",
        "y",
        "z",
        "sendCommand",
        "const char*",
        "onCommandReadyRead",
        "Protocol",
        "OpenTrack",
        "FreeTrack"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'statusChanged'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'error'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'commandReceived'
        QtMocHelpers::SignalData<void(const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Slot 'start'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'stop'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendPose'
        QtMocHelpers::SlotData<void(float, float, float, float, float, float)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 11 }, { QMetaType::Float, 12 }, { QMetaType::Float, 13 }, { QMetaType::Float, 14 },
            { QMetaType::Float, 15 }, { QMetaType::Float, 16 },
        }}),
        // Slot 'sendCommand'
        QtMocHelpers::SlotData<void(const char *)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 7 },
        }}),
        // Slot 'onCommandReadyRead'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'Protocol'
        QtMocHelpers::EnumData<enum Protocol>(20, 20, QMC::EnumFlags{}).add({
            {   21, Protocol::OpenTrack },
            {   22, Protocol::FreeTrack },
        }),
    };
    return QtMocHelpers::metaObjectData<UdpBridge, qt_meta_tag_ZN9UdpBridgeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject UdpBridge::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UdpBridgeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UdpBridgeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9UdpBridgeE_t>.metaTypes,
    nullptr
} };

void UdpBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UdpBridge *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->error((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->commandReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->start(); break;
        case 4: _t->stop(); break;
        case 5: _t->sendPose((*reinterpret_cast<std::add_pointer_t<float>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[6]))); break;
        case 6: _t->sendCommand((*reinterpret_cast<std::add_pointer_t<const char*>>(_a[1]))); break;
        case 7: _t->onCommandReadyRead(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (UdpBridge::*)(bool )>(_a, &UdpBridge::statusChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (UdpBridge::*)(const QString & )>(_a, &UdpBridge::error, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (UdpBridge::*)(const QString & )>(_a, &UdpBridge::commandReceived, 2))
            return;
    }
}

const QMetaObject *UdpBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UdpBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UdpBridgeE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UdpBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void UdpBridge::statusChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void UdpBridge::error(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void UdpBridge::commandReceived(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP

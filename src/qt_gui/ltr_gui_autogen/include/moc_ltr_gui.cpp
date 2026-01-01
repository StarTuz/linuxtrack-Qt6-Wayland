/****************************************************************************
** Meta object code from reading C++ file 'ltr_gui.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ltr_gui.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ltr_gui.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13LinuxtrackGuiE_t {};
} // unnamed namespace

template <> constexpr inline auto LinuxtrackGui::qt_create_metaobjectdata<qt_meta_tag_ZN13LinuxtrackGuiE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "LinuxtrackGui",
        "show",
        "",
        "on_QuitButton_pressed",
        "on_SaveButton_pressed",
        "on_DefaultsButton_pressed",
        "on_DiscardChangesButton_pressed",
        "on_HelpButton_pressed",
        "on_LtrTab_currentChanged",
        "index",
        "trackerStateHandler",
        "linuxtrack_state_type",
        "current_state",
        "on_LegacyPose_stateChanged",
        "state",
        "on_LegacyRotation_stateChanged",
        "on_FocalLength_valueChanged",
        "val",
        "on_TransRotDisable_stateChanged",
        "on_ViewLogButton_pressed",
        "on_XplanePluginButton_pressed",
        "on_PackageLogsButton_pressed",
        "onLALClicked",
        "logsPackaged",
        "exitCode",
        "QProcess::ExitStatus",
        "exitStatus"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'show'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_QuitButton_pressed'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_SaveButton_pressed'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_DefaultsButton_pressed'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_DiscardChangesButton_pressed'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_HelpButton_pressed'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_LtrTab_currentChanged'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'trackerStateHandler'
        QtMocHelpers::SlotData<void(linuxtrack_state_type)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'on_LegacyPose_stateChanged'
        QtMocHelpers::SlotData<void(int)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
        // Slot 'on_LegacyRotation_stateChanged'
        QtMocHelpers::SlotData<void(int)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
        // Slot 'on_FocalLength_valueChanged'
        QtMocHelpers::SlotData<void(double)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 17 },
        }}),
        // Slot 'on_TransRotDisable_stateChanged'
        QtMocHelpers::SlotData<void(int)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
        // Slot 'on_ViewLogButton_pressed'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_XplanePluginButton_pressed'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_PackageLogsButton_pressed'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLALClicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'logsPackaged'
        QtMocHelpers::SlotData<void(int, QProcess::ExitStatus)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 24 }, { 0x80000000 | 25, 26 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<LinuxtrackGui, qt_meta_tag_ZN13LinuxtrackGuiE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject LinuxtrackGui::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13LinuxtrackGuiE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13LinuxtrackGuiE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13LinuxtrackGuiE_t>.metaTypes,
    nullptr
} };

void LinuxtrackGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LinuxtrackGui *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->show(); break;
        case 1: _t->on_QuitButton_pressed(); break;
        case 2: _t->on_SaveButton_pressed(); break;
        case 3: _t->on_DefaultsButton_pressed(); break;
        case 4: _t->on_DiscardChangesButton_pressed(); break;
        case 5: _t->on_HelpButton_pressed(); break;
        case 6: _t->on_LtrTab_currentChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->trackerStateHandler((*reinterpret_cast<std::add_pointer_t<linuxtrack_state_type>>(_a[1]))); break;
        case 8: _t->on_LegacyPose_stateChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->on_LegacyRotation_stateChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->on_FocalLength_valueChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 11: _t->on_TransRotDisable_stateChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->on_ViewLogButton_pressed(); break;
        case 13: _t->on_XplanePluginButton_pressed(); break;
        case 14: _t->on_PackageLogsButton_pressed(); break;
        case 15: _t->onLALClicked(); break;
        case 16: _t->logsPackaged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *LinuxtrackGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LinuxtrackGui::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13LinuxtrackGuiE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LinuxtrackGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP

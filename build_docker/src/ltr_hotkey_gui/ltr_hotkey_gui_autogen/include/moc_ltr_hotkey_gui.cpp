/****************************************************************************
** Meta object code from reading C++ file 'ltr_hotkey_gui.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/ltr_hotkey_gui/ltr_hotkey_gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ltr_hotkey_gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HotkeyGUI_t {
    const uint offsetsAndSize[20];
    char stringdata0[154];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_HotkeyGUI_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_HotkeyGUI_t qt_meta_stringdata_HotkeyGUI = {
    {
QT_MOC_LITERAL(0, 9), // "HotkeyGUI"
QT_MOC_LITERAL(10, 18), // "onRecenterRedefine"
QT_MOC_LITERAL(29, 0), // ""
QT_MOC_LITERAL(30, 15), // "onPauseRedefine"
QT_MOC_LITERAL(46, 13), // "onQuitClicked"
QT_MOC_LITERAL(60, 18), // "onStartStopClicked"
QT_MOC_LITERAL(79, 15), // "onTrayActivated"
QT_MOC_LITERAL(95, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(129, 6), // "reason"
QT_MOC_LITERAL(136, 17) // "checkDaemonStatus"

    },
    "HotkeyGUI\0onRecenterRedefine\0\0"
    "onPauseRedefine\0onQuitClicked\0"
    "onStartStopClicked\0onTrayActivated\0"
    "QSystemTrayIcon::ActivationReason\0"
    "reason\0checkDaemonStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HotkeyGUI[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x08,    1 /* Private */,
       3,    0,   51,    2, 0x08,    2 /* Private */,
       4,    0,   52,    2, 0x08,    3 /* Private */,
       5,    0,   53,    2, 0x08,    4 /* Private */,
       6,    1,   54,    2, 0x08,    5 /* Private */,
       9,    0,   57,    2, 0x08,    7 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

       0        // eod
};

void HotkeyGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HotkeyGUI *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onRecenterRedefine(); break;
        case 1: _t->onPauseRedefine(); break;
        case 2: _t->onQuitClicked(); break;
        case 3: _t->onStartStopClicked(); break;
        case 4: _t->onTrayActivated((*reinterpret_cast< std::add_pointer_t<QSystemTrayIcon::ActivationReason>>(_a[1]))); break;
        case 5: _t->checkDaemonStatus(); break;
        default: ;
        }
    }
}

const QMetaObject HotkeyGUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_HotkeyGUI.offsetsAndSize,
    qt_meta_data_HotkeyGUI,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_HotkeyGUI_t
, QtPrivate::TypeAndForceComplete<HotkeyGUI, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QSystemTrayIcon::ActivationReason, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *HotkeyGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HotkeyGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HotkeyGUI.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int HotkeyGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE

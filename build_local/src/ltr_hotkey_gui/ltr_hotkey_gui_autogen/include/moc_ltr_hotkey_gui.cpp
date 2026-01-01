/****************************************************************************
** Meta object code from reading C++ file 'ltr_hotkey_gui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/ltr_hotkey_gui/ltr_hotkey_gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ltr_hotkey_gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HotkeyGUI_t {
    QByteArrayData data[10];
    char stringdata0[154];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HotkeyGUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HotkeyGUI_t qt_meta_stringdata_HotkeyGUI = {
    {
QT_MOC_LITERAL(0, 0, 9), // "HotkeyGUI"
QT_MOC_LITERAL(1, 10, 18), // "onRecenterRedefine"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 15), // "onPauseRedefine"
QT_MOC_LITERAL(4, 46, 13), // "onQuitClicked"
QT_MOC_LITERAL(5, 60, 18), // "onStartStopClicked"
QT_MOC_LITERAL(6, 79, 15), // "onTrayActivated"
QT_MOC_LITERAL(7, 95, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(8, 129, 6), // "reason"
QT_MOC_LITERAL(9, 136, 17) // "checkDaemonStatus"

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
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    1,   48,    2, 0x08 /* Private */,
       9,    0,   51,    2, 0x08 /* Private */,

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
        case 4: _t->onTrayActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 5: _t->checkDaemonStatus(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HotkeyGUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_HotkeyGUI.data,
    qt_meta_data_HotkeyGUI,
    qt_static_metacall,
    nullptr,
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

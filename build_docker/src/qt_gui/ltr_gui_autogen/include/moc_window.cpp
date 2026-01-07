/****************************************************************************
** Meta object code from reading C++ file 'window.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/qt_gui/window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Window_t {
    const uint offsetsAndSize[20];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Window_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Window_t qt_meta_stringdata_Window = {
    {
QT_MOC_LITERAL(0, 6), // "Window"
QT_MOC_LITERAL(7, 10), // "update_pic"
QT_MOC_LITERAL(18, 0), // ""
QT_MOC_LITERAL(19, 12), // "start_widget"
QT_MOC_LITERAL(32, 7), // "newPose"
QT_MOC_LITERAL(40, 23), // "linuxtrack_full_pose_t*"
QT_MOC_LITERAL(64, 3), // "raw"
QT_MOC_LITERAL(68, 18), // "linuxtrack_pose_t*"
QT_MOC_LITERAL(87, 10), // "unfiltered"
QT_MOC_LITERAL(98, 9) // "processed"

    },
    "Window\0update_pic\0\0start_widget\0newPose\0"
    "linuxtrack_full_pose_t*\0raw\0"
    "linuxtrack_pose_t*\0unfiltered\0processed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Window[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x08,    1 /* Private */,
       3,    0,   33,    2, 0x08,    2 /* Private */,
       4,    3,   34,    2, 0x08,    3 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 7, 0x80000000 | 7,    6,    8,    9,

       0        // eod
};

void Window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Window *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->update_pic(); break;
        case 1: _t->start_widget(); break;
        case 2: _t->newPose((*reinterpret_cast< std::add_pointer_t<linuxtrack_full_pose_t*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<linuxtrack_pose_t*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<linuxtrack_pose_t*>>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject Window::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Window.offsetsAndSize,
    qt_meta_data_Window,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Window_t
, QtPrivate::TypeAndForceComplete<Window, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<linuxtrack_full_pose_t *, std::false_type>, QtPrivate::TypeAndForceComplete<linuxtrack_pose_t *, std::false_type>, QtPrivate::TypeAndForceComplete<linuxtrack_pose_t *, std::false_type>


>,
    nullptr
} };


const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Window.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE

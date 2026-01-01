/****************************************************************************
** Meta object code from reading C++ file 'glwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../glwidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12ReaderThreadE_t {};
} // unnamed namespace

template <> constexpr inline auto ReaderThread::qt_create_metaobjectdata<qt_meta_tag_ZN12ReaderThreadE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ReaderThread",
        "done",
        ""
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'done'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ReaderThread, qt_meta_tag_ZN12ReaderThreadE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ReaderThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ReaderThreadE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ReaderThreadE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ReaderThreadE_t>.metaTypes,
    nullptr
} };

void ReaderThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ReaderThread *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->done(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ReaderThread::*)()>(_a, &ReaderThread::done, 0))
            return;
    }
}

const QMetaObject *ReaderThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReaderThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ReaderThreadE_t>.strings))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int ReaderThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ReaderThread::done()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
namespace {
struct qt_meta_tag_ZN8GLWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto GLWidget::qt_create_metaobjectdata<qt_meta_tag_ZN8GLWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GLWidget",
        "ready",
        "",
        "setXRotation",
        "angle",
        "setYRotation",
        "setZRotation",
        "setXTrans",
        "pos",
        "setYTrans",
        "setZTrans",
        "objectsRead"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'ready'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setXRotation'
        QtMocHelpers::SlotData<void(float)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 4 },
        }}),
        // Slot 'setYRotation'
        QtMocHelpers::SlotData<void(float)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 4 },
        }}),
        // Slot 'setZRotation'
        QtMocHelpers::SlotData<void(float)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 4 },
        }}),
        // Slot 'setXTrans'
        QtMocHelpers::SlotData<void(float)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 8 },
        }}),
        // Slot 'setYTrans'
        QtMocHelpers::SlotData<void(float)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 8 },
        }}),
        // Slot 'setZTrans'
        QtMocHelpers::SlotData<void(float)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 8 },
        }}),
        // Slot 'objectsRead'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GLWidget, qt_meta_tag_ZN8GLWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GLWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QOpenGLWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8GLWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8GLWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8GLWidgetE_t>.metaTypes,
    nullptr
} };

void GLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GLWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ready(); break;
        case 1: _t->setXRotation((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 2: _t->setYRotation((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 3: _t->setZRotation((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 4: _t->setXTrans((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 5: _t->setYTrans((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 6: _t->setZTrans((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 7: _t->objectsRead(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GLWidget::*)()>(_a, &GLWidget::ready, 0))
            return;
    }
}

const QMetaObject *GLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8GLWidgetE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int GLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
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
void GLWidget::ready()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP

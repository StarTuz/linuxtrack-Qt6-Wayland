/****************************************************************************
** Meta object code from reading C++ file 'extractor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/qt_gui/extractor.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'extractor.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18TirFwExtractThreadE_t {};
} // unnamed namespace

template <> constexpr inline auto TirFwExtractThread::qt_create_metaobjectdata<qt_meta_tag_ZN18TirFwExtractThreadE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TirFwExtractThread",
        "progress",
        "",
        "msg"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'progress'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TirFwExtractThread, qt_meta_tag_ZN18TirFwExtractThreadE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TirFwExtractThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18TirFwExtractThreadE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18TirFwExtractThreadE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18TirFwExtractThreadE_t>.metaTypes,
    nullptr
} };

void TirFwExtractThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TirFwExtractThread *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->progress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TirFwExtractThread::*)(const QString & )>(_a, &TirFwExtractThread::progress, 0))
            return;
    }
}

const QMetaObject *TirFwExtractThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TirFwExtractThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18TirFwExtractThreadE_t>.strings))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int TirFwExtractThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void TirFwExtractThread::progress(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN9ExtractorE_t {};
} // unnamed namespace

template <> constexpr inline auto Extractor::qt_create_metaobjectdata<qt_meta_tag_ZN9ExtractorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Extractor",
        "finished",
        "",
        "result",
        "show",
        "on_BrowseInstaller_pressed",
        "on_BrowseDir_pressed",
        "on_AnalyzeSourceButton_pressed",
        "on_DownloadButton_pressed",
        "on_QuitButton_pressed",
        "on_HelpButton_pressed",
        "progress",
        "msg",
        "threadFinished",
        "wineFinished",
        "downloadDone",
        "ok",
        "fileName"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'finished'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Slot 'show'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_BrowseInstaller_pressed'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'on_BrowseDir_pressed'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'on_AnalyzeSourceButton_pressed'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'on_DownloadButton_pressed'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'on_QuitButton_pressed'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'on_HelpButton_pressed'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'progress'
        QtMocHelpers::SlotData<void(const QString &)>(11, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::QString, 12 },
        }}),
        // Slot 'threadFinished'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'wineFinished'
        QtMocHelpers::SlotData<void(bool)>(14, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Slot 'downloadDone'
        QtMocHelpers::SlotData<void(bool, QString)>(15, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::Bool, 16 }, { QMetaType::QString, 17 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Extractor, qt_meta_tag_ZN9ExtractorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Extractor::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ExtractorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ExtractorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9ExtractorE_t>.metaTypes,
    nullptr
} };

void Extractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Extractor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->show(); break;
        case 2: _t->on_BrowseInstaller_pressed(); break;
        case 3: _t->on_BrowseDir_pressed(); break;
        case 4: _t->on_AnalyzeSourceButton_pressed(); break;
        case 5: _t->on_DownloadButton_pressed(); break;
        case 6: _t->on_QuitButton_pressed(); break;
        case 7: _t->on_HelpButton_pressed(); break;
        case 8: _t->progress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->threadFinished(); break;
        case 10: _t->wineFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 11: _t->downloadDone((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Extractor::*)(bool )>(_a, &Extractor::finished, 0))
            return;
    }
}

const QMetaObject *Extractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Extractor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ExtractorE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Extractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void Extractor::finished(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN14TirFwExtractorE_t {};
} // unnamed namespace

template <> constexpr inline auto TirFwExtractor::qt_create_metaobjectdata<qt_meta_tag_ZN14TirFwExtractorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TirFwExtractor",
        "threadFinished",
        "",
        "wineFinished",
        "result",
        "on_AnalyzeSourceButton_pressed",
        "on_QuitButton_pressed"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'threadFinished'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'wineFinished'
        QtMocHelpers::SlotData<void(bool)>(3, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 4 },
        }}),
        // Slot 'on_AnalyzeSourceButton_pressed'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_QuitButton_pressed'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TirFwExtractor, qt_meta_tag_ZN14TirFwExtractorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TirFwExtractor::staticMetaObject = { {
    QMetaObject::SuperData::link<Extractor::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14TirFwExtractorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14TirFwExtractorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14TirFwExtractorE_t>.metaTypes,
    nullptr
} };

void TirFwExtractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TirFwExtractor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->threadFinished(); break;
        case 1: _t->wineFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->on_AnalyzeSourceButton_pressed(); break;
        case 3: _t->on_QuitButton_pressed(); break;
        default: ;
        }
    }
}

const QMetaObject *TirFwExtractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TirFwExtractor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14TirFwExtractorE_t>.strings))
        return static_cast<void*>(this);
    return Extractor::qt_metacast(_clname);
}

int TirFwExtractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Extractor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN15Mfc42uExtractorE_t {};
} // unnamed namespace

template <> constexpr inline auto Mfc42uExtractor::qt_create_metaobjectdata<qt_meta_tag_ZN15Mfc42uExtractorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Mfc42uExtractor",
        "wineFinished",
        "",
        "result",
        "cabextractFinished",
        "exitCode",
        "QProcess::ExitStatus",
        "status"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'wineFinished'
        QtMocHelpers::SlotData<void(bool)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Slot 'cabextractFinished'
        QtMocHelpers::SlotData<void(int, QProcess::ExitStatus)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 5 }, { 0x80000000 | 6, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Mfc42uExtractor, qt_meta_tag_ZN15Mfc42uExtractorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Mfc42uExtractor::staticMetaObject = { {
    QMetaObject::SuperData::link<Extractor::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15Mfc42uExtractorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15Mfc42uExtractorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15Mfc42uExtractorE_t>.metaTypes,
    nullptr
} };

void Mfc42uExtractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Mfc42uExtractor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->wineFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->cabextractFinished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *Mfc42uExtractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mfc42uExtractor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15Mfc42uExtractorE_t>.strings))
        return static_cast<void*>(this);
    return Extractor::qt_metacast(_clname);
}

int Mfc42uExtractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Extractor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP

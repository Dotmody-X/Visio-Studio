/****************************************************************************
** Meta object code from reading C++ file 'engine.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/core/engine.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'engine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN6EngineE_t {};
} // unnamed namespace

template <> constexpr inline auto Engine::qt_create_metaobjectdata<qt_meta_tag_ZN6EngineE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Engine",
        "processingStarted",
        "",
        "processingFinished",
        "result",
        "processingCanceled",
        "processingFailed",
        "errorMessage",
        "processingProgress",
        "percent",
        "effectChainChanged",
        "effectAdded",
        "id",
        "effectRemoved",
        "effectMoved",
        "newPosition",
        "effectParametersChanged",
        "effectEnabledChanged",
        "enabled",
        "handleProcessingFinished",
        "handleProcessingCanceled"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'processingStarted'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'processingFinished'
        QtMocHelpers::SignalData<void(const QImage &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QImage, 4 },
        }}),
        // Signal 'processingCanceled'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'processingFailed'
        QtMocHelpers::SignalData<void(const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Signal 'processingProgress'
        QtMocHelpers::SignalData<void(int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Signal 'effectChainChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'effectAdded'
        QtMocHelpers::SignalData<void(int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Signal 'effectRemoved'
        QtMocHelpers::SignalData<void(int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Signal 'effectMoved'
        QtMocHelpers::SignalData<void(int, int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 }, { QMetaType::Int, 15 },
        }}),
        // Signal 'effectParametersChanged'
        QtMocHelpers::SignalData<void(int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Signal 'effectEnabledChanged'
        QtMocHelpers::SignalData<void(int, bool)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 }, { QMetaType::Bool, 18 },
        }}),
        // Slot 'handleProcessingFinished'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleProcessingCanceled'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Engine, qt_meta_tag_ZN6EngineE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Engine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6EngineE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6EngineE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6EngineE_t>.metaTypes,
    nullptr
} };

void Engine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Engine *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->processingStarted(); break;
        case 1: _t->processingFinished((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1]))); break;
        case 2: _t->processingCanceled(); break;
        case 3: _t->processingFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->processingProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->effectChainChanged(); break;
        case 6: _t->effectAdded((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->effectRemoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->effectMoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 9: _t->effectParametersChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->effectEnabledChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 11: _t->handleProcessingFinished(); break;
        case 12: _t->handleProcessingCanceled(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Engine::*)()>(_a, &Engine::processingStarted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)(const QImage & )>(_a, &Engine::processingFinished, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)()>(_a, &Engine::processingCanceled, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)(const QString & )>(_a, &Engine::processingFailed, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)(int )>(_a, &Engine::processingProgress, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)()>(_a, &Engine::effectChainChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)(int )>(_a, &Engine::effectAdded, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)(int )>(_a, &Engine::effectRemoved, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)(int , int )>(_a, &Engine::effectMoved, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)(int )>(_a, &Engine::effectParametersChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (Engine::*)(int , bool )>(_a, &Engine::effectEnabledChanged, 10))
            return;
    }
}

const QMetaObject *Engine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Engine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6EngineE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Engine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Engine::processingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Engine::processingFinished(const QImage & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Engine::processingCanceled()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Engine::processingFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void Engine::processingProgress(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void Engine::effectChainChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Engine::effectAdded(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void Engine::effectRemoved(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void Engine::effectMoved(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2);
}

// SIGNAL 9
void Engine::effectParametersChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void Engine::effectEnabledChanged(int _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2);
}
QT_WARNING_POP

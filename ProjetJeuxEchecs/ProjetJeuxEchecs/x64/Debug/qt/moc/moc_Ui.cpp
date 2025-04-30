/****************************************************************************
** Meta object code from reading C++ file 'Ui.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../Ui.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Ui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
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
struct qt_meta_tag_ZN2Ui16ProjetJeuxEchecsE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN2Ui16ProjetJeuxEchecsE = QtMocHelpers::stringData(
    "Ui::ProjetJeuxEchecs"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN2Ui16ProjetJeuxEchecsE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject Ui::ProjetJeuxEchecs::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN2Ui16ProjetJeuxEchecsE.offsetsAndSizes,
    qt_meta_data_ZN2Ui16ProjetJeuxEchecsE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN2Ui16ProjetJeuxEchecsE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ProjetJeuxEchecs, std::true_type>
    >,
    nullptr
} };

void Ui::ProjetJeuxEchecs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProjetJeuxEchecs *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *Ui::ProjetJeuxEchecs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Ui::ProjetJeuxEchecs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN2Ui16ProjetJeuxEchecsE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Ui::ProjetJeuxEchecs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN2Ui11PieceWidgetE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN2Ui11PieceWidgetE = QtMocHelpers::stringData(
    "Ui::PieceWidget",
    "demanderDeplacerWidget",
    "",
    "PieceWidget*",
    "pieceWidget",
    "Modele::Position",
    "pos",
    "surPieceDeplacee",
    "Modele::Piece*",
    "piece"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN2Ui11PieceWidgetE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    2,   31,    2, 0x0a,    4 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 5,    9,    6,

       0        // eod
};

Q_CONSTINIT const QMetaObject Ui::PieceWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QLabel::staticMetaObject>(),
    qt_meta_stringdata_ZN2Ui11PieceWidgetE.offsetsAndSizes,
    qt_meta_data_ZN2Ui11PieceWidgetE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN2Ui11PieceWidgetE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PieceWidget, std::true_type>,
        // method 'demanderDeplacerWidget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<PieceWidget *, std::false_type>,
        QtPrivate::TypeAndForceComplete<Modele::Position, std::false_type>,
        // method 'surPieceDeplacee'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Modele::Piece *, std::false_type>,
        QtPrivate::TypeAndForceComplete<Modele::Position, std::false_type>
    >,
    nullptr
} };

void Ui::PieceWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PieceWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->demanderDeplacerWidget((*reinterpret_cast< std::add_pointer_t<PieceWidget*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Modele::Position>>(_a[2]))); break;
        case 1: _t->surPieceDeplacee((*reinterpret_cast< std::add_pointer_t<Modele::Piece*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Modele::Position>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< PieceWidget* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (PieceWidget::*)(PieceWidget * , Modele::Position );
            if (_q_method_type _q_method = &PieceWidget::demanderDeplacerWidget; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *Ui::PieceWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Ui::PieceWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN2Ui11PieceWidgetE.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int Ui::PieceWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Ui::PieceWidget::demanderDeplacerWidget(PieceWidget * _t1, Modele::Position _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {
struct qt_meta_tag_ZN2Ui15EchiquierWidgetE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN2Ui15EchiquierWidgetE = QtMocHelpers::stringData(
    "Ui::EchiquierWidget",
    "surPieceCapturee",
    "",
    "Modele::Piece*",
    "piece"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN2Ui15EchiquierWidgetE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x08,    1 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject Ui::EchiquierWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN2Ui15EchiquierWidgetE.offsetsAndSizes,
    qt_meta_data_ZN2Ui15EchiquierWidgetE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN2Ui15EchiquierWidgetE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<EchiquierWidget, std::true_type>,
        // method 'surPieceCapturee'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Modele::Piece *, std::false_type>
    >,
    nullptr
} };

void Ui::EchiquierWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<EchiquierWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->surPieceCapturee((*reinterpret_cast< std::add_pointer_t<Modele::Piece*>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *Ui::EchiquierWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Ui::EchiquierWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN2Ui15EchiquierWidgetE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Ui::EchiquierWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP

/****************************************************************************
** Meta object code from reading C++ file 'qtserver.h'
**
** Created: Sat 2. Jun 14:41:52 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qtserver.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qtserver[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      35,    9,    9,    9, 0x08,
      52,   44,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_qtserver[] = {
    "qtserver\0\0on_StartServer_clicked()\0"
    "OnExit()\0message\0OnServerMessage(QString)\0"
};

void qtserver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        qtserver *_t = static_cast<qtserver *>(_o);
        switch (_id) {
        case 0: _t->on_StartServer_clicked(); break;
        case 1: _t->OnExit(); break;
        case 2: _t->OnServerMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qtserver::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qtserver::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_qtserver,
      qt_meta_data_qtserver, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qtserver::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qtserver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qtserver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qtserver))
        return static_cast<void*>(const_cast< qtserver*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int qtserver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
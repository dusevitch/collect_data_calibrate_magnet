/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[343];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 23), // "on_activate_mag_toggled"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 7), // "checked"
QT_MOC_LITERAL(4, 44, 23), // "on_start_sensor_toggled"
QT_MOC_LITERAL(5, 68, 24), // "on_start_polaris_toggled"
QT_MOC_LITERAL(6, 93, 23), // "on_collect_data_toggled"
QT_MOC_LITERAL(7, 117, 21), // "on_updateBase_clicked"
QT_MOC_LITERAL(8, 139, 23), // "on_start_sensor_clicked"
QT_MOC_LITERAL(9, 163, 11), // "readMagData"
QT_MOC_LITERAL(10, 175, 12), // "getCalibData"
QT_MOC_LITERAL(11, 188, 23), // "on_printMagData_clicked"
QT_MOC_LITERAL(12, 212, 10), // "GUI_Update"
QT_MOC_LITERAL(13, 223, 25), // "on_save_coil_vals_clicked"
QT_MOC_LITERAL(14, 249, 26), // "on_load_calib_file_clicked"
QT_MOC_LITERAL(15, 276, 21), // "on_save_calib_clicked"
QT_MOC_LITERAL(16, 298, 20), // "on_run_calib_clicked"
QT_MOC_LITERAL(17, 319, 23) // "on_pushButton_2_clicked"

    },
    "MainWindow\0on_activate_mag_toggled\0\0"
    "checked\0on_start_sensor_toggled\0"
    "on_start_polaris_toggled\0"
    "on_collect_data_toggled\0on_updateBase_clicked\0"
    "on_start_sensor_clicked\0readMagData\0"
    "getCalibData\0on_printMagData_clicked\0"
    "GUI_Update\0on_save_coil_vals_clicked\0"
    "on_load_calib_file_clicked\0"
    "on_save_calib_clicked\0on_run_calib_clicked\0"
    "on_pushButton_2_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x08 /* Private */,
       4,    1,   92,    2, 0x08 /* Private */,
       5,    1,   95,    2, 0x08 /* Private */,
       6,    1,   98,    2, 0x08 /* Private */,
       7,    0,  101,    2, 0x08 /* Private */,
       8,    1,  102,    2, 0x08 /* Private */,
       9,    0,  105,    2, 0x08 /* Private */,
      10,    0,  106,    2, 0x08 /* Private */,
      11,    0,  107,    2, 0x08 /* Private */,
      12,    0,  108,    2, 0x08 /* Private */,
      13,    0,  109,    2, 0x08 /* Private */,
      14,    0,  110,    2, 0x08 /* Private */,
      15,    0,  111,    2, 0x08 /* Private */,
      16,    0,  112,    2, 0x08 /* Private */,
      17,    0,  113,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_activate_mag_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_start_sensor_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_start_polaris_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_collect_data_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_updateBase_clicked(); break;
        case 5: _t->on_start_sensor_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->readMagData(); break;
        case 7: _t->getCalibData(); break;
        case 8: _t->on_printMagData_clicked(); break;
        case 9: _t->GUI_Update(); break;
        case 10: _t->on_save_coil_vals_clicked(); break;
        case 11: _t->on_load_calib_file_clicked(); break;
        case 12: _t->on_save_calib_clicked(); break;
        case 13: _t->on_run_calib_clicked(); break;
        case 14: _t->on_pushButton_2_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

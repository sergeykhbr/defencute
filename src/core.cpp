/*
 *  Copyright 2026 Sergey Khabarov, sergeykhbr@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "core.h"
#include <QMetaType>
#include <QJsonParseError>
#include <QDebug>
#include "SceneGeneric.h"
#include "tower.h"
#include "enemy.h"

static Core *core_ = nullptr;

ICore *getpCoreInterface() {
    if (core_ == nullptr) {
        core_ = new Core();
    }
    return core_;
}

Core::Core() : ICore()
{
    uniqueIdx_ = 0;
}

void Core::configurate(QString filename) {
    qRegisterMetaType<SceneGeneric *>("SceneGeneric");
    qRegisterMetaType<ArrowTower *>("ArrowTower");
    qRegisterMetaType<RifleTower *>("RifleTower");
    qRegisterMetaType<Enemy *>("Enemy");    // remove me

    QString json =
    "{\n"
    "  'controller':{\n"
    "           'Level':'SceneGeneric',\n"
    "           'TickMs':16\n"
    "           },\n"
    "  'HexMenu':{\n"
    "           'BtnOrbit':54.0,\n"
    "           'BtnRadius':24.0,\n"
    "           'BtnMax':6,\n"
    "           'BtnTowers':['ArrowTower',\n"
    "                        '',\n"
    "                        'RifleTower',\n"
    "                        '',\n"
    "                        '',\n"
    "                        ''\n"
    "                        ]\n"
    "           },\n"
    "  'scene':'current scene run time generated',\n"
    "  'towers':{\n"
    "           'ArrowTower':{\n"
    "                        'sprite':':/images/archer_tower.png',\n"
    "                        'iconFile':':/images/build_icons_64x8.png',\n"
    "                        'iconx':0,\n"
    "                        'price':50\n"
    "                        },\n"
    "           'RifleTower':{\n"
    "                        'sprite':':/images/rifle_tower.png',\n"
    "                        'iconFile':':/images/build_icons_64x8.png',\n"
    "                        'iconx':64,\n"
    "                        'price':60\n"
    "                        }\n"
    "           },\n"
    "  'Routes':[\n"
    "               {\n"
    "               'RouteName':'route1',\n"
    "               'StartX':2,\n"
    "               'StartY':8,\n"
    "               'Steps':[\n"
    "                        {'dx':1, 'dy':0, 'N':5},\n"
    "                        {'dx':0, 'dy':1, 'N':3},\n"
    "                        {'dx':1, 'dy':1, 'N':1},\n"
    "                        {'dx':1, 'dy':0, 'N':1},\n"
    "                        {'dx':1, 'dy':1, 'N':1},\n"
    "                        {'dx':1, 'dy':0, 'N':6},\n"
    "                        {'dx':0, 'dy':-1, 'N':3},\n"
    "                        {'dx':1, 'dy':0, 'N':5}\n"
    "                       ]\n"
    "               }\n"
    "           ],\n"
    "  'Waves':[\n"
    "               [\n"
    "                {'dt':300, 'Spawn':'Enemy', 'Route':'route1', 'offx':10, 'offy':5},\n"
    "                {'dt':60, 'Spawn':'Enemy', 'Route':'route1', 'offx':0, 'offy':-5},\n"
    "                {'dt':600, 'Spawn':'Enemy', 'Route':'route1', 'offx':0, 'offy':-15},\n"
    "                {'dt':600, 'Spawn':'Enemy', 'Route':'route1', 'offx':-10, 'offy':15}\n"
    "               ]\n"
    "           ],\n"
    "  'ZDepth':{\n"
    "           'HexTile':-10,\n"
    "           'HexMenu':2000,\n"
    "           'TowerMenu':2000\n"
    "           }\n"
    "}\n";

    cfg_ = toJsonObject(json);

    mainWindow_ = new MainWindow(cfg_);
    mainWindow_->setWindowTitle("Defencute demo");
    mainWindow_->show();
}

Core::~Core() {
    delete mainWindow_;
}

QJsonObject Core::toJsonObject(QString str)  {
    str.replace('\'', '\"');

    QByteArray rawJson = str.toUtf8();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(rawJson, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON Parsing Error:" << parseError.errorString() 
                   << "at character offset:" << parseError.offset;
        return QJsonObject(); // Return an empty object
    }
    if (!doc.isObject()) {
        qWarning() << "JSON Error: Root element is a list [] or primitive, not an object.";
        return QJsonObject();
    }

    return doc.object();
}

void Core::registerCoreObject(QString objname,
                              Interface *iface) {
    objList_[objname] = iface;
}

void Core::registerCoreClass(QString name, Interface *iclsface) {
}

QObject *Core::createQtClassObject(QObject *parent,
                                   QString clsname,
                                   QJsonObject &jsonarg) {
    int typeId = QMetaType::type(clsname.toUtf8().constData());
    const QMetaObject* metaObj = QMetaType::metaObjectForType(typeId);

    if (!metaObj) {
        qWarning() << "Reflection Error: Class not  found ->" << clsname;
        return nullptr;
    }

    QString objname = clsname + QString::number(++uniqueIdx_);
    QObject *instance = metaObj->newInstance(
        Q_ARG(QObject*, parent),
        Q_ARG(QString, objname),
        Q_ARG(QJsonObject &, jsonarg)
    );
    if (!instance) {
        qWarning() << "Reflection Error: Failed to invoke for" << objname;
        return nullptr;
    }

    return instance;
}


Interface *Core::getpObjInterface(QString objname,
                                  QString iname) {
    auto it = objList_.find(objname);
    if (it == objList_.end()) {
        qDebug() << "Object " << objname << " not found";
        return nullptr;
    }

    ICoreObject *iobj = dynamic_cast<ICoreObject *>(it.value());
    Interface *i = iobj->getInterface(iname);
    return i;
}


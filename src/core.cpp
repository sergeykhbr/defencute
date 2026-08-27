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
#include <QDebug>
#include "SceneGeneric.h"
#include "tower.h"

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
}

void Core::registerCoreObject(QString objname,
                              Interface *iface) {
    objList_[objname] = iface;
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


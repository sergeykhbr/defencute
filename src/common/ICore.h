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

#pragma once

#include "IFace.h"
#include <QObject>
#include <QJsonObject>

class ICore : public Interface {
 public:
    ICore() : Interface("ICore") {}

    virtual void configurate(QString filename) = 0;

    virtual QObject *createQtClassObject(QObject *parent,
                                         QString clsname,
                                         QJsonObject &jsonarg) = 0;

    virtual void registerCoreObject(QString objname,
                                    Interface *iface) = 0;

    virtual Interface *getpObjInterface(QString objname,
                                        QString iname) = 0;

};

ICore *getpCoreInterface();


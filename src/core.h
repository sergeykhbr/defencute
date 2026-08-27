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

#include <ICore.h>
#include <QList>
#include <QHash>

class Core : public ICore {
 public:
    Core();

    virtual void configurate(QString filename) override;

    virtual QObject *createQtClassObject(QObject *parent,
                                         QString clsname,
                                         QJsonObject &jsonarg) override;

    virtual void registerCoreObject(QString objname,
                                    Interface *iface) override;

    virtual Interface *getpObjInterface(QString objname,
                                        QString iname) override;

 protected:

 private:
    QHash<QString, Interface *> objList_;
    int uniqueIdx_;
};

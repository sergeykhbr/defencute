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
#include <ICore.h>
#include <QList>

class ICoreObject : public Interface {
 public:
    ICoreObject(QString objName) : Interface("ICoreObject"),
       objName_(objName) {
       getpCoreInterface()->registerCoreObject(objName, this);
    }

    virtual QString getObjName() { return objName_; }

    virtual void setClassOwner(Interface *iclsface) { iclsface_ = iclsface; }

    virtual void registerInterface(Interface *iface) {
        listInterfaces_.append(iface);
    }

    virtual Interface *getInterface(QString iname) {
        for (Interface *i : listInterfaces_){
            if (i->getFaceName() == iname) {
                return i;
            }
        }
        return nullptr;
    }

 private:
    QString objName_;
    Interface *iclsface_;
    QList<Interface *> listInterfaces_;
};


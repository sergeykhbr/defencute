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
#include <ICoreObject.h>

class ICoreClass : public Interface {
 public:
    explicit ICoreClass(QString className)
        : Interface("ICoreClass") {
        className_ = className;
        icore_ = getpCoreInterface();
        icore_->registerCoreClass(className, this);
    }

    virtual ICoreObject *createCoreObject(QObject *parent,
                                          QString objname,
                                          QJsonObject &cfg) = 0;

    virtual QString getClassName() { return className_; }

 private:
    ICore *icore_;
    QString className_;
};

#define DECLARE_CLASS(name) \
class name ## CoreClass : public ICoreClass { \
 public: \
    name ## CoreClass() : ICoreClass(# name "CoreClass") {} \
    virtual ICoreObject *createCoreObject(QObject *parent, \
                                          QString objName, \
                                          QJsonObject &cfg) {  \
        name *obj = new name(parent, objName, cfg); \
        ICoreObject *ret = static_cast<ICoreObject *>(obj); \
        ret->setClassOwner(this); \
        return ret; \
    } \
};


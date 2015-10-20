//
//  Created by Bradley Austin Davis 2015/10/09
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#pragma once
#ifndef hifi_Controllers_Endpoint_h
#define hifi_Controllers_Endpoint_h

#include <list>
#include <memory>
#include <functional>

#include <QtCore/QObject>

#include "Input.h"

class QScriptValue;

namespace controller {
    /*
    * Encapsulates a particular input / output,
    * i.e. Hydra.Button0, Standard.X, Action.Yaw
    */
    class Endpoint : public QObject {
        Q_OBJECT;
    public:
        using Pointer = std::shared_ptr<Endpoint>;
        using List = std::list<Pointer>;
        using Pair = std::pair<Pointer, Pointer>;
        using ReadLambda = std::function<float()>;
        using WriteLambda = std::function<void(float)>;

        Endpoint(const Input& input) : _input(input) {}
        virtual float value() = 0;
        virtual void apply(float newValue, float oldValue, const Pointer& source) = 0;
        const Input& getInput() { return _input;  }
    protected:
        Input _input;
    };

    class LambdaEndpoint : public Endpoint {
    public:
        LambdaEndpoint(ReadLambda readLambda, WriteLambda writeLambda = [](float) {})
            : Endpoint(Input::INVALID_INPUT), _readLambda(readLambda), _writeLambda(writeLambda) { }

        virtual float value() override { return _readLambda(); }
        virtual void apply(float newValue, float oldValue, const Pointer& source) override { _writeLambda(newValue); }

    private:
        ReadLambda _readLambda;
        WriteLambda _writeLambda;
    };
}

#endif

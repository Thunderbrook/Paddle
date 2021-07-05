/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "paddle/fluid/framework/eigen.h"
#include "paddle/fluid/framework/op_registry.h"
#include "paddle/fluid/operators/eigen/eigen_function.h"

namespace paddle {
namespace operators {

template <typename DeviceContext, typename T>
class ErfKernel : public framework::OpKernel<T> {
 public:
  virtual void Compute(const framework::ExecutionContext& context) const {
    auto* out = context.Output<framework::Tensor>("Out");
    auto* in = context.Input<framework::Tensor>("X");
    out->mutable_data<T>(in->place());

    auto eigen_out = framework::EigenVector<T>::Flatten(*out);
    auto eigen_in = framework::EigenVector<T>::Flatten(*in);
    auto& place =
        *context.template device_context<DeviceContext>().eigen_device();
    EigenErf<std::decay_t<decltype(place)>, T>::Eval(place, eigen_out,
                                                     eigen_in);
  }
};

template <typename DeviceContext, typename T>
class ErfGradKernel : public framework::OpKernel<T> {
 public:
  virtual void Compute(const framework::ExecutionContext& context) const {
    auto* x = context.Input<framework::Tensor>("X");
    auto* dout =
        context.Input<framework::Tensor>(framework::GradVarName("Out"));
    auto* dx = context.Output<framework::Tensor>(framework::GradVarName("X"));

    dx->mutable_data<T>(dout->place());

    auto eigen_x = framework::EigenVector<T>::Flatten(*x);
    auto eigen_dout = framework::EigenVector<T>::Flatten(*dout);
    auto eigen_dx = framework::EigenVector<T>::Flatten(*dx);
    auto& place =
        *context.template device_context<DeviceContext>().eigen_device();
    EigenErfGrad<std::decay_t<decltype(place)>, T>::Eval(place, eigen_dx,
                                                         eigen_x, eigen_dout);
  }
};

}  // namespace operators
}  // namespace paddle

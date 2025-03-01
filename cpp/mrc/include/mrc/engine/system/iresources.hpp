/**
 * SPDX-FileCopyrightText: Copyright (c) 2021-2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <functional>
#include <memory>

namespace mrc::internal::system {

class ISystem;
class Resources;

class IResources
{
  public:
    IResources(std::shared_ptr<ISystem> system);
    virtual ~IResources() = 0;

  protected:
    void add_thread_initializer(std::function<void()> initializer_fn);
    void add_thread_finalizer(std::function<void()> finalizer_fn);

  private:
    std::unique_ptr<Resources> m_impl;
    friend Resources;
};

}  // namespace mrc::internal::system

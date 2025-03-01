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

#include "common.hpp"

#include "internal/system/system.hpp"

#include "mrc/options/options.hpp"

#include <utility>

std::shared_ptr<mrc::internal::system::System> make_system(std::function<void(mrc::Options&)> updater)
{
    auto options = std::make_shared<mrc::Options>();
    if (updater)
    {
        updater(*options);
    }

    return mrc::internal::system::make_system(std::move(options));
}

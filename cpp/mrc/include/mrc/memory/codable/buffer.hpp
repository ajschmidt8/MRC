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

#include "mrc/codable/codable_protocol.hpp"
#include "mrc/memory/buffer.hpp"

#include <cstddef>

// IWYU pragma: no_forward_declare mrc::codable::codable_protocol

namespace mrc::codable {
class EncodingOptions;
template <typename T>
class Encoder;
template <typename T>
class Decoder;

template <>
struct codable_protocol<mrc::memory::buffer>
{
    static void serialize(const memory::buffer& obj, Encoder<memory::buffer>& encoded, const EncodingOptions& opts);

    static memory::buffer deserialize(const Decoder<memory::buffer>& encoded, std::size_t object_idx);
};

}  // namespace mrc::codable

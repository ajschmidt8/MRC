/**
 * SPDX-FileCopyrightText: Copyright (c) 2022-2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include "nodes.hpp"

#include <rxcpp/rx.hpp>

namespace mrc::quickstart::cpp::ex01_node_library {

IntSource::IntSource() :
  mrc::node::RxSource<int>(rxcpp::observable<>::create<int>([](rxcpp::subscriber<int> s) {
      s.on_next(1);
      s.on_next(2);
      s.on_next(3);
      s.on_completed();
  }))
{}

}  // namespace mrc::quickstart::cpp::ex01_node_library

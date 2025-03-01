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

#include "mrc/metrics/counter.hpp"

#include <prometheus/counter.h>

namespace mrc::metrics {

Counter::Counter(prometheus::Counter* counter) : m_counter(counter) {}

void Counter::increment()
{
    m_counter->Increment();
}

void Counter::increment(const std::size_t& ticks)
{
    m_counter->Increment(ticks);
}

}  // namespace mrc::metrics

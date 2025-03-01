# SPDX-FileCopyrightText: Copyright (c) 2018-2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Keep all source files sorted!!!
add_executable(test_mrc
  coroutines/test_event.cpp
  coroutines/test_latch.cpp
  coroutines/test_ring_buffer.cpp
  coroutines/test_task.cpp
  modules/test_module_registry.cpp
  modules/test_module_util.cpp
  modules/test_segment_modules.cpp
  test_channel.cpp
  test_edges.cpp
  test_executor.cpp
  test_main.cpp
  test_metrics.cpp
  test_mrc.cpp
  test_node.cpp
  test_pipeline.cpp
  test_segment.cpp
  test_thread.cpp
  test_type_utils.cpp
)

target_link_libraries(test_mrc
  PRIVATE
  ${PROJECT_NAME}::libmrc
  GTest::gtest
)

add_test(
  NAME test_mrc
  COMMAND $<TARGET_FILE:test_mrc>
)

add_subdirectory(benchmarking)
add_subdirectory(logging)
add_subdirectory(modules)

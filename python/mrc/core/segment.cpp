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

#include "pymrc/segment.hpp"

#include "pymrc/module_registry.hpp"
#include "pymrc/node.hpp"  // IWYU pragma: keep
#include "pymrc/segment_modules.hpp"
#include "pymrc/types.hpp"
#include "pymrc/utilities/function_wrappers.hpp"  // IWYU pragma: keep
#include "pymrc/utils.hpp"

#include "mrc/edge/edge_connector.hpp"
#include "mrc/modules/segment_modules.hpp"
#include "mrc/runnable/launch_options.hpp"
#include "mrc/segment/builder.hpp"
#include "mrc/segment/definition.hpp"
#include "mrc/utils/string_utils.hpp"
#include "mrc/version.hpp"

#include <pybind11/cast.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

namespace mrc::pymrc {

namespace py = pybind11;

PYBIND11_MODULE(segment, module)
{
    module.doc() = R"pbdoc(
        Python bindings for MRC Segments
        -------------------------------
        .. currentmodule:: segment
        .. autosummary::
           :toctree: _generate
    )pbdoc";

    // Common must be first in every module
    pymrc::import(module, "mrc.core.common");
    pymrc::import(module, "mrc.core.subscriber");

    // Type 'b'
    edge::EdgeConnector<bool, PyHolder>::register_converter();
    edge::EdgeConnector<PyHolder, bool>::register_converter();

    // Type 'i'
    edge::EdgeConnector<int32_t, PyHolder>::register_converter();
    edge::EdgeConnector<PyHolder, int32_t>::register_converter();

    edge::EdgeConnector<int64_t, PyHolder>::register_converter();
    edge::EdgeConnector<PyHolder, int64_t>::register_converter();

    // Type 'u'
    edge::EdgeConnector<uint32_t, PyHolder>::register_converter();
    edge::EdgeConnector<PyHolder, uint32_t>::register_converter();

    edge::EdgeConnector<uint64_t, PyHolder>::register_converter();
    edge::EdgeConnector<PyHolder, uint64_t>::register_converter();

    // Type 'f'
    edge::EdgeConnector<float, PyHolder>::register_converter();
    edge::EdgeConnector<PyHolder, float>::register_converter();

    edge::EdgeConnector<double, PyHolder>::register_converter();
    edge::EdgeConnector<PyHolder, double>::register_converter();

    // Type 'S' and 'U'
    edge::EdgeConnector<std::string, PyHolder>::register_converter();
    edge::EdgeConnector<PyHolder, std::string>::register_converter();

    py::class_<mrc::runnable::LaunchOptions>(module, "LaunchOptions")
        .def_readwrite("pe_count", &mrc::runnable::LaunchOptions::pe_count)
        .def_readwrite("engines_per_pe", &mrc::runnable::LaunchOptions::engines_per_pe)
        .def_readwrite("engine_factory_name", &mrc::runnable::LaunchOptions::engine_factory_name);

    // Base SegmentObject that all object usually derive from
    py::class_<mrc::segment::ObjectProperties, std::shared_ptr<mrc::segment::ObjectProperties>>(module, "SegmentObject")
        .def_property_readonly("name", &PyNode::name)
        .def_property_readonly("launch_options",
                               py::overload_cast<>(&mrc::segment::ObjectProperties::launch_options),
                               py::return_value_policy::reference_internal);

    auto Builder       = py::class_<mrc::segment::Builder>(module, "Builder");
    auto Definition    = py::class_<mrc::segment::Definition>(module, "Definition");
    auto SegmentModule = py::class_<mrc::modules::SegmentModule, std::shared_ptr<mrc::modules::SegmentModule>>(module,
                                                                                                               "Segment"
                                                                                                               "Modul"
                                                                                                               "e");
    auto SegmentModuleRegistry = py::class_<ModuleRegistryProxy>(module, "ModuleRegistry");

    /** Builder Interface Declarations **/
    /*
     * @brief Make a source node that generates py::object values
     */
    Builder.def(
        "make_source",
        static_cast<std::shared_ptr<mrc::segment::ObjectProperties> (*)(mrc::segment::Builder&,
                                                                        const std::string&,
                                                                        py::iterator)>(&BuilderProxy::make_source));

    Builder.def(
        "make_source",
        static_cast<std::shared_ptr<mrc::segment::ObjectProperties> (*)(mrc::segment::Builder&,
                                                                        const std::string&,
                                                                        py::iterable)>(&BuilderProxy::make_source),
        py::return_value_policy::reference_internal);

    Builder.def(
        "make_source",
        static_cast<std::shared_ptr<mrc::segment::ObjectProperties> (*)(mrc::segment::Builder&,
                                                                        const std::string&,
                                                                        py::function)>(&BuilderProxy::make_source));

    Builder.def("make_source_component",
                static_cast<std::shared_ptr<mrc::segment::ObjectProperties> (*)(mrc::segment::Builder&,
                                                                                const std::string&,
                                                                                py::iterator)>(
                    &BuilderProxy::make_source_component));

    Builder.def("make_source_component",
                static_cast<std::shared_ptr<mrc::segment::ObjectProperties> (*)(mrc::segment::Builder&,
                                                                                const std::string&,
                                                                                py::iterable)>(
                    &BuilderProxy::make_source_component));

    Builder.def("make_source_component",
                static_cast<std::shared_ptr<mrc::segment::ObjectProperties> (*)(mrc::segment::Builder&,
                                                                                const std::string&,
                                                                                py::function)>(
                    &BuilderProxy::make_source_component));

    /**
     * Construct a new py::object sink.
     * Create and return a Segment node used to sink python objects following out of the Segment.
     *
     * (py) @param name: Unique name of the node that will be created in the MRC Segment.
     * (py) @param on_next: python/std function that will be called on a new data element.
     * (py) @param on_error: python/std function that will be called if an error occurs.
     * (py) @param on_completed: python/std function that will be called
     *  Python example.
     *  ```python
     *      def my_on_next(x):
     *          print(f"Sinking {x}")
     *      def my_on_error(err):
     *          print(f"An error occurred: {err}")
     *      def my_on_completed():
     *          print(f"Completed processing")
     *
     *      sink = segment.make_sink("test", my_on_next, my_on_error, my_on_completed)
     *  ```
     */
    Builder.def("make_sink",
                &BuilderProxy::make_sink,
                py::return_value_policy::reference_internal,
                py::arg("name"),
                py::arg("on_next").none(true)     = py::none(),
                py::arg("on_error").none(true)    = py::none(),
                py::arg("on_complete").none(true) = py::none());

    Builder.def("make_sink_component",
                &BuilderProxy::make_sink_component,
                py::return_value_policy::reference_internal,
                py::arg("name"),
                py::arg("on_next").none(true)     = py::none(),
                py::arg("on_error").none(true)    = py::none(),
                py::arg("on_complete").none(true) = py::none());

    Builder.def("make_node",
                py::overload_cast<mrc::segment::Builder&, const std::string&, OnDataFunction>(&BuilderProxy::make_node),
                py::return_value_policy::reference_internal);

    /**
     * Construct a new 'pure' python::object -> python::object node
     *
     * This will create and return a new lambda function with the following signature:
     * (py) @param name : Unique name of the node that will be created in the MRC Segment.
     * python-function which will be called on each data element as it flows through the node.
     */
    Builder.def("make_node",
                py::overload_cast<mrc::segment::Builder&, const std::string&, pybind11::args>(&BuilderProxy::make_node),
                py::return_value_policy::reference_internal);

    Builder.def("make_node_component", &BuilderProxy::make_node_component, py::return_value_policy::reference_internal);

    /**
     * Find and return an existing egress port -- throws if `name` does not exist
     * (py) @param name: Name of the egress port
     */
    Builder.def("get_egress", &BuilderProxy::get_egress, py::arg("name"));

    /**
     * Find and return an existing ingress port -- throws if `name` does not exist
     * (py) @param name: Name of the ingress port
     */
    Builder.def("get_ingress", &BuilderProxy::get_ingress, py::arg("name"));

    Builder.def("make_edge", &BuilderProxy::make_edge);

    Builder.def("make_edge", &BuilderProxy::make_edge, py::arg("source"), py::arg("sink"));

    Builder.def("load_module",
                &BuilderProxy::load_module_from_registry,
                py::arg("module_id"),
                py::arg("registry_namespace"),
                py::arg("module_name"),
                py::arg("module_config"),
                py::return_value_policy::reference_internal);

    Builder.def("init_module", &BuilderProxy::init_module, py::arg("module"));

    Builder.def("register_module_input", &BuilderProxy::register_module_input, py::arg("input_name"), py::arg("object"));

    Builder.def("register_module_output",
                &BuilderProxy::register_module_output,
                py::arg("output_name"),
                py::arg("object"));

    Builder.def("get_current_module_config", &BuilderProxy::get_current_module_config);

    Builder.def("make_node_full", &BuilderProxy::make_node_full, py::return_value_policy::reference_internal);

    /** Segment Module Interface Declarations **/
    SegmentModule.def("config", &SegmentModuleProxy::config);

    SegmentModule.def("component_prefix", &SegmentModuleProxy::component_prefix);

    SegmentModule.def("input_port", &SegmentModuleProxy::input_port, py::arg("input_id"));

    SegmentModule.def("input_ports", &SegmentModuleProxy::input_ports);

    SegmentModule.def("module_type_name", &SegmentModuleProxy::module_type_name);

    SegmentModule.def("name", &SegmentModuleProxy::name);

    SegmentModule.def("output_port", &SegmentModuleProxy::output_port, py::arg("output_id"));

    SegmentModule.def("output_ports", &SegmentModuleProxy::output_ports);

    SegmentModule.def("input_ids", &SegmentModuleProxy::input_ids);

    SegmentModule.def("output_ids", &SegmentModuleProxy::output_ids);

    // TODO(drobison): need to think about if/how we want to expose type_ids to Python... It might allow for some nice
    // flexibility SegmentModule.def("input_port_type_id", &SegmentModuleProxy::input_port_type_id, py::arg("input_id"))
    // SegmentModule.def("input_port_type_ids", &SegmentModuleProxy::input_port_type_id)
    // SegmentModule.def("output_port_type_id", &SegmentModuleProxy::output_port_type_id, py::arg("output_id"))
    // SegmentModule.def("output_port_type_ids", &SegmentModuleProxy::output_port_type_id)

    /** Module Register Interface Declarations **/
    SegmentModuleRegistry.def_static("contains",
                                     &ModuleRegistryProxy::contains,
                                     py::arg("name"),
                                     py::arg("registry_namespace"));

    SegmentModuleRegistry.def_static("contains_namespace",
                                     &ModuleRegistryProxy::contains_namespace,
                                     py::arg("registry_namespace"));

    SegmentModuleRegistry.def_static("registered_modules", &ModuleRegistryProxy::registered_modules);

    SegmentModuleRegistry.def_static("is_version_compatible",
                                     &ModuleRegistryProxy::is_version_compatible,
                                     py::arg("release_version"));

    SegmentModuleRegistry.def_static("get_module_constructor",
                                     &ModuleRegistryProxy::get_module_constructor,
                                     py::arg("name"),
                                     py::arg("registry_namespace"));

    SegmentModuleRegistry.def_static(
        "register_module",
        static_cast<void (*)(std::string, const std::vector<unsigned int>&, std::function<void(mrc::segment::Builder&)>)>(
            &ModuleRegistryProxy::register_module),
        py::arg("name"),
        py::arg("release_version"),
        py::arg("fn_constructor"));

    SegmentModuleRegistry.def_static(
        "register_module",
        static_cast<void (*)(std::string,
                             std::string,
                             const std::vector<unsigned int>&,
                             std::function<void(mrc::segment::Builder&)>)>(&ModuleRegistryProxy::register_module),
        py::arg("name"),
        py::arg("registry_namespace"),
        py::arg("release_version"),
        py::arg("fn_constructor"));

    SegmentModuleRegistry.def_static("unregister_module",
                                     &ModuleRegistryProxy::unregister_module,
                                     py::arg("name"),
                                     py::arg("registry_namespace"),
                                     py::arg("optional") = true);

    module.attr("__version__") = MRC_CONCAT_STR(mrc_VERSION_MAJOR << "." << mrc_VERSION_MINOR << "."
                                                                  << mrc_VERSION_PATCH);
}
}  // namespace mrc::pymrc

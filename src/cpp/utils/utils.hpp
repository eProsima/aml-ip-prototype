// Copyright 2021 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file utils.hpp
 *
 */

#ifndef TYPES_UTILS_HPP
#define TYPES_UTILS_HPP

#include "../types/DLOutput/DLOutput.h"
#include "../types/Atomization/Atomization.h"

AML_IP_Atomization generate_random_atomization_data(
        uint32_t data_size);

AML_IP_DLOutput generate_random_dloutput_data(
        uint32_t data_size);

std::ostream& operator <<(
        std::ostream& output,
        const AML_IP_Atomization& data);

std::ostream& operator <<(
        std::ostream& output,
        const AML_IP_DLOutput& data);

#endif // TYPES_UTILS_HPP

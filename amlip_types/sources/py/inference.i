// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/*!
 * @file inference.i
 * This header file contains the SWIG interface of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

%module inference

// SWIG helper modules
%include "typemaps.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_array.i"
%include "std_map.i"

// Assignemt operators are ignored, as there is no such thing in Python.
// Trying to export them issues a warning
%ignore *::operator=;

// Definition of internal types

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;


// Macro declarations
// Any macro used on the Fast DDS header files will give an error if it is not redefined here
#define RTPS_DllAPI
#define eProsima_user_DllExport

%include "multiservice.i"

%{
#include "inference.h"
%}

////////////////////////////////////////////////////////
// Binding for class Inference_Task_Data
////////////////////////////////////////////////////////

// Ignore overloaded methods that have no application on Python
// Otherwise they will issue a warning
%ignore Inference_Task_Data::Inference_Task_Data(Inference_Task_Data&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Avoid a warning ignoring all but one
%ignore Inference_Task_Data::inference(std::vector<uint8_t>&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Const accesors produced constant enums instead of arrays/dictionaries when used
// We ignore them to prevent this
%ignore Inference_Task_Data::inference() const;
%template(uint8_t_vector) std::vector<uint8_t>;


////////////////////////////////////////////////////////
// Binding for class Inference_Solution_Data
////////////////////////////////////////////////////////

// Ignore overloaded methods that have no application on Python
// Otherwise they will issue a warning
%ignore Inference_Solution_Data::Inference_Solution_Data(Inference_Solution_Data&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Avoid a warning ignoring all but one
%ignore Inference_Solution_Data::solution(std::vector<uint8_t>&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Const accesors produced constant enums instead of arrays/dictionaries when used
// We ignore them to prevent this
%ignore Inference_Solution_Data::solution() const;
%template(uint8_t_vector) std::vector<uint8_t>;


////////////////////////////////////////////////////////
// Binding for class Inference_Task
////////////////////////////////////////////////////////

// Ignore overloaded methods that have no application on Python
// Otherwise they will issue a warning
%ignore Inference_Task::Inference_Task(Inference_Task&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Avoid a warning ignoring all but one
%ignore Inference_Task::data(Inference_Task_Data&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Const accesors produced constant enums instead of arrays/dictionaries when used
// We ignore them to prevent this
%ignore Inference_Task::data();
%rename("%s") Inference_Task::data() const;


////////////////////////////////////////////////////////
// Binding for class Inference_Solution
////////////////////////////////////////////////////////

// Ignore overloaded methods that have no application on Python
// Otherwise they will issue a warning
%ignore Inference_Solution::Inference_Solution(Inference_Solution&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Avoid a warning ignoring all but one
%ignore Inference_Solution::data(Inference_Solution_Data&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Const accesors produced constant enums instead of arrays/dictionaries when used
// We ignore them to prevent this
%ignore Inference_Solution::data();
%rename("%s") Inference_Solution::data() const;



// Include the class interfaces
%include "inference.h"

// Include the corresponding TopicDataType
%include "inferencePubSubTypes.i"

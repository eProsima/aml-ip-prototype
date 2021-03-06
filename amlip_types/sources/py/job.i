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
 * @file job.i
 * This header file contains the SWIG interface of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

%module job

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
#include "job.h"
%}

////////////////////////////////////////////////////////
// Binding for class Job_Task_Data
////////////////////////////////////////////////////////

// Ignore overloaded methods that have no application on Python
// Otherwise they will issue a warning
%ignore Job_Task_Data::Job_Task_Data(Job_Task_Data&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Avoid a warning ignoring all but one
%ignore Job_Task_Data::job(std::vector<uint8_t>&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Const accesors produced constant enums instead of arrays/dictionaries when used
// We ignore them to prevent this
%ignore Job_Task_Data::job() const;
%template(uint8_t_vector) std::vector<uint8_t>;


////////////////////////////////////////////////////////
// Binding for class Job_Solution_Data
////////////////////////////////////////////////////////

// Ignore overloaded methods that have no application on Python
// Otherwise they will issue a warning
%ignore Job_Solution_Data::Job_Solution_Data(Job_Solution_Data&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Avoid a warning ignoring all but one
%ignore Job_Solution_Data::solution(std::vector<uint8_t>&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Const accesors produced constant enums instead of arrays/dictionaries when used
// We ignore them to prevent this
%ignore Job_Solution_Data::solution() const;
%template(uint8_t_vector) std::vector<uint8_t>;


////////////////////////////////////////////////////////
// Binding for class Job_Task
////////////////////////////////////////////////////////

// Ignore overloaded methods that have no application on Python
// Otherwise they will issue a warning
%ignore Job_Task::Job_Task(Job_Task&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Avoid a warning ignoring all but one
%ignore Job_Task::data(Job_Task_Data&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Const accesors produced constant enums instead of arrays/dictionaries when used
// We ignore them to prevent this
%ignore Job_Task::data();
%rename("%s") Job_Task::data() const;


////////////////////////////////////////////////////////
// Binding for class Job_Solution
////////////////////////////////////////////////////////

// Ignore overloaded methods that have no application on Python
// Otherwise they will issue a warning
%ignore Job_Solution::Job_Solution(Job_Solution&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Avoid a warning ignoring all but one
%ignore Job_Solution::data(Job_Solution_Data&&);

// Overloaded getter methods shadow each other and are equivalent in python
// Const accesors produced constant enums instead of arrays/dictionaries when used
// We ignore them to prevent this
%ignore Job_Solution::data();
%rename("%s") Job_Solution::data() const;



// Include the class interfaces
%include "job.h"

// Include the corresponding TopicDataType
%include "jobPubSubTypes.i"

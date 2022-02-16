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
 * @file job.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace {
char dummy;
}  // namespace
#endif  // _WIN32

#include "job.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

Job_Task_Data::Job_Task_Data()    : Multiservice_Task_Data() 
{
    // m_job com.eprosima.idl.parser.typecode.SequenceTypeCode@1dd92fe2


}

Job_Task_Data::~Job_Task_Data()
{
}

Job_Task_Data::Job_Task_Data(
        const Job_Task_Data& x)    : Multiservice_Task_Data(x) 
{
    m_job = x.m_job;
}

Job_Task_Data::Job_Task_Data(
        Job_Task_Data&& x)    : Multiservice_Task_Data(std::move(x)) 
{
    m_job = std::move(x.m_job);
}

Job_Task_Data& Job_Task_Data::operator =(
        const Job_Task_Data& x)
{
    Multiservice_Task_Data::operator =(x); 

    m_job = x.m_job;

    return *this;
}

Job_Task_Data& Job_Task_Data::operator =(
        Job_Task_Data&& x)
{
    Multiservice_Task_Data::operator =(std::move(x)); 

    m_job = std::move(x.m_job);

    return *this;
}

bool Job_Task_Data::operator ==(
        const Job_Task_Data& x) const
{
     if (Multiservice_Task_Data::operator !=(x)) return false; 

    return (m_job == x.m_job);
}

bool Job_Task_Data::operator !=(
        const Job_Task_Data& x) const
{
    return !(*this == x);
}

size_t Job_Task_Data::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += Multiservice_Task_Data::getMaxCdrSerializedSize(current_alignment); 

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);

    current_alignment += (100 * 1) + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);



    return current_alignment - initial_alignment;
}

size_t Job_Task_Data::getCdrSerializedSize(
        const Job_Task_Data& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += Multiservice_Task_Data::getCdrSerializedSize(data, current_alignment); 

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);

    if (data.job().size() > 0)
    {
        current_alignment += (data.job().size() * 1) + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);
    }



    return current_alignment - initial_alignment;
}

void Job_Task_Data::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    Multiservice_Task_Data::serialize(scdr); 

    scdr << m_job;
}

void Job_Task_Data::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    Multiservice_Task_Data::deserialize(dcdr); 

    dcdr >> m_job;}

/*!
 * @brief This function copies the value in member job
 * @param _job New value to be copied in member job
 */
void Job_Task_Data::job(
        const std::vector<uint8_t>& _job)
{
    m_job = _job;
}

/*!
 * @brief This function moves the value in member job
 * @param _job New value to be moved in member job
 */
void Job_Task_Data::job(
        std::vector<uint8_t>&& _job)
{
    m_job = std::move(_job);
}

/*!
 * @brief This function returns a constant reference to member job
 * @return Constant reference to member job
 */
const std::vector<uint8_t>& Job_Task_Data::job() const
{
    return m_job;
}

/*!
 * @brief This function returns a reference to member job
 * @return Reference to member job
 */
std::vector<uint8_t>& Job_Task_Data::job()
{
    return m_job;
}

size_t Job_Task_Data::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t current_align = current_alignment;

    current_align += Multiservice_Task_Data::getKeyMaxCdrSerializedSize(current_align); 


    return current_align;
}

bool Job_Task_Data::isKeyDefined()
{
    if (Multiservice_Task_Data::isKeyDefined())
        return true;
     return false;
}

void Job_Task_Data::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
    Multiservice_Task_Data::serializeKey(scdr); 
     
}

Job_Solution_Data::Job_Solution_Data()    : Multiservice_Solution_Data() 
{
    // m_solution com.eprosima.idl.parser.typecode.SequenceTypeCode@1b68b9a4


}

Job_Solution_Data::~Job_Solution_Data()
{
}

Job_Solution_Data::Job_Solution_Data(
        const Job_Solution_Data& x)    : Multiservice_Solution_Data(x) 
{
    m_solution = x.m_solution;
}

Job_Solution_Data::Job_Solution_Data(
        Job_Solution_Data&& x)    : Multiservice_Solution_Data(std::move(x)) 
{
    m_solution = std::move(x.m_solution);
}

Job_Solution_Data& Job_Solution_Data::operator =(
        const Job_Solution_Data& x)
{
    Multiservice_Solution_Data::operator =(x); 

    m_solution = x.m_solution;

    return *this;
}

Job_Solution_Data& Job_Solution_Data::operator =(
        Job_Solution_Data&& x)
{
    Multiservice_Solution_Data::operator =(std::move(x)); 

    m_solution = std::move(x.m_solution);

    return *this;
}

bool Job_Solution_Data::operator ==(
        const Job_Solution_Data& x) const
{
     if (Multiservice_Solution_Data::operator !=(x)) return false; 

    return (m_solution == x.m_solution);
}

bool Job_Solution_Data::operator !=(
        const Job_Solution_Data& x) const
{
    return !(*this == x);
}

size_t Job_Solution_Data::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += Multiservice_Solution_Data::getMaxCdrSerializedSize(current_alignment); 

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);

    current_alignment += (100 * 1) + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);



    return current_alignment - initial_alignment;
}

size_t Job_Solution_Data::getCdrSerializedSize(
        const Job_Solution_Data& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += Multiservice_Solution_Data::getCdrSerializedSize(data, current_alignment); 

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);

    if (data.solution().size() > 0)
    {
        current_alignment += (data.solution().size() * 1) + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);
    }



    return current_alignment - initial_alignment;
}

void Job_Solution_Data::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    Multiservice_Solution_Data::serialize(scdr); 

    scdr << m_solution;
}

void Job_Solution_Data::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    Multiservice_Solution_Data::deserialize(dcdr); 

    dcdr >> m_solution;}

/*!
 * @brief This function copies the value in member solution
 * @param _solution New value to be copied in member solution
 */
void Job_Solution_Data::solution(
        const std::vector<uint8_t>& _solution)
{
    m_solution = _solution;
}

/*!
 * @brief This function moves the value in member solution
 * @param _solution New value to be moved in member solution
 */
void Job_Solution_Data::solution(
        std::vector<uint8_t>&& _solution)
{
    m_solution = std::move(_solution);
}

/*!
 * @brief This function returns a constant reference to member solution
 * @return Constant reference to member solution
 */
const std::vector<uint8_t>& Job_Solution_Data::solution() const
{
    return m_solution;
}

/*!
 * @brief This function returns a reference to member solution
 * @return Reference to member solution
 */
std::vector<uint8_t>& Job_Solution_Data::solution()
{
    return m_solution;
}

size_t Job_Solution_Data::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t current_align = current_alignment;

    current_align += Multiservice_Solution_Data::getKeyMaxCdrSerializedSize(current_align); 


    return current_align;
}

bool Job_Solution_Data::isKeyDefined()
{
    if (Multiservice_Solution_Data::isKeyDefined())
        return true;
     return false;
}

void Job_Solution_Data::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
    Multiservice_Solution_Data::serializeKey(scdr); 
     
}

Job_Task::Job_Task()    : Multiservice_Task() 
{
    // m_data com.eprosima.fastdds.idl.parser.typecode.StructTypeCode@2b4a2ec7


}

Job_Task::~Job_Task()
{
}

Job_Task::Job_Task(
        const Job_Task& x)    : Multiservice_Task(x) 
{
    m_data = x.m_data;
}

Job_Task::Job_Task(
        Job_Task&& x)    : Multiservice_Task(std::move(x)) 
{
    m_data = std::move(x.m_data);
}

Job_Task& Job_Task::operator =(
        const Job_Task& x)
{
    Multiservice_Task::operator =(x); 

    m_data = x.m_data;

    return *this;
}

Job_Task& Job_Task::operator =(
        Job_Task&& x)
{
    Multiservice_Task::operator =(std::move(x)); 

    m_data = std::move(x.m_data);

    return *this;
}

bool Job_Task::operator ==(
        const Job_Task& x) const
{
     if (Multiservice_Task::operator !=(x)) return false; 

    return (m_data == x.m_data);
}

bool Job_Task::operator !=(
        const Job_Task& x) const
{
    return !(*this == x);
}

size_t Job_Task::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += Multiservice_Task::getMaxCdrSerializedSize(current_alignment); 

    current_alignment += Job_Task_Data::getMaxCdrSerializedSize(current_alignment);

    return current_alignment - initial_alignment;
}

size_t Job_Task::getCdrSerializedSize(
        const Job_Task& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += Multiservice_Task::getCdrSerializedSize(data, current_alignment); 

    current_alignment += Job_Task_Data::getCdrSerializedSize(data.data(), current_alignment);

    return current_alignment - initial_alignment;
}

void Job_Task::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    Multiservice_Task::serialize(scdr); 

    scdr << m_data;

}

void Job_Task::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    Multiservice_Task::deserialize(dcdr); 

    dcdr >> m_data;
}

/*!
 * @brief This function copies the value in member data
 * @param _data New value to be copied in member data
 */
void Job_Task::data(
        const Job_Task_Data& _data)
{
    m_data = _data;
}

/*!
 * @brief This function moves the value in member data
 * @param _data New value to be moved in member data
 */
void Job_Task::data(
        Job_Task_Data&& _data)
{
    m_data = std::move(_data);
}

/*!
 * @brief This function returns a constant reference to member data
 * @return Constant reference to member data
 */
const Job_Task_Data& Job_Task::data() const
{
    return m_data;
}

/*!
 * @brief This function returns a reference to member data
 * @return Reference to member data
 */
Job_Task_Data& Job_Task::data()
{
    return m_data;
}

size_t Job_Task::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t current_align = current_alignment;

    current_align += Multiservice_Task::getKeyMaxCdrSerializedSize(current_align); 


    return current_align;
}

bool Job_Task::isKeyDefined()
{
    if (Multiservice_Task::isKeyDefined())
        return true;
     return false;
}

void Job_Task::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
    Multiservice_Task::serializeKey(scdr); 
     
}

Job_Solution::Job_Solution()    : Multiservice_Solution() 
{
    // m_data com.eprosima.fastdds.idl.parser.typecode.StructTypeCode@57175e74


}

Job_Solution::~Job_Solution()
{
}

Job_Solution::Job_Solution(
        const Job_Solution& x)    : Multiservice_Solution(x) 
{
    m_data = x.m_data;
}

Job_Solution::Job_Solution(
        Job_Solution&& x)    : Multiservice_Solution(std::move(x)) 
{
    m_data = std::move(x.m_data);
}

Job_Solution& Job_Solution::operator =(
        const Job_Solution& x)
{
    Multiservice_Solution::operator =(x); 

    m_data = x.m_data;

    return *this;
}

Job_Solution& Job_Solution::operator =(
        Job_Solution&& x)
{
    Multiservice_Solution::operator =(std::move(x)); 

    m_data = std::move(x.m_data);

    return *this;
}

bool Job_Solution::operator ==(
        const Job_Solution& x) const
{
     if (Multiservice_Solution::operator !=(x)) return false; 

    return (m_data == x.m_data);
}

bool Job_Solution::operator !=(
        const Job_Solution& x) const
{
    return !(*this == x);
}

size_t Job_Solution::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += Multiservice_Solution::getMaxCdrSerializedSize(current_alignment); 

    current_alignment += Job_Solution_Data::getMaxCdrSerializedSize(current_alignment);

    return current_alignment - initial_alignment;
}

size_t Job_Solution::getCdrSerializedSize(
        const Job_Solution& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += Multiservice_Solution::getCdrSerializedSize(data, current_alignment); 

    current_alignment += Job_Solution_Data::getCdrSerializedSize(data.data(), current_alignment);

    return current_alignment - initial_alignment;
}

void Job_Solution::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    Multiservice_Solution::serialize(scdr); 

    scdr << m_data;

}

void Job_Solution::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    Multiservice_Solution::deserialize(dcdr); 

    dcdr >> m_data;
}

/*!
 * @brief This function copies the value in member data
 * @param _data New value to be copied in member data
 */
void Job_Solution::data(
        const Job_Solution_Data& _data)
{
    m_data = _data;
}

/*!
 * @brief This function moves the value in member data
 * @param _data New value to be moved in member data
 */
void Job_Solution::data(
        Job_Solution_Data&& _data)
{
    m_data = std::move(_data);
}

/*!
 * @brief This function returns a constant reference to member data
 * @return Constant reference to member data
 */
const Job_Solution_Data& Job_Solution::data() const
{
    return m_data;
}

/*!
 * @brief This function returns a reference to member data
 * @return Reference to member data
 */
Job_Solution_Data& Job_Solution::data()
{
    return m_data;
}

size_t Job_Solution::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t current_align = current_alignment;

    current_align += Multiservice_Solution::getKeyMaxCdrSerializedSize(current_align); 


    return current_align;
}

bool Job_Solution::isKeyDefined()
{
    if (Multiservice_Solution::isKeyDefined())
        return true;
     return false;
}

void Job_Solution::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
    Multiservice_Solution::serializeKey(scdr); 
     
}

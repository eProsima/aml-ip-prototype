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
 * @file status.cpp
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

#include "status.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>



Status::Status()
{
    // m_id com.eprosima.idl.parser.typecode.StringTypeCode@1b1473ab
    m_id ="";
    // m_name com.eprosima.idl.parser.typecode.StringTypeCode@7880cdf3
    m_name ="";
    // m_node_kind com.eprosima.idl.parser.typecode.EnumTypeCode@5be6e01c
    m_node_kind = ::UNDETERMINED;
    // m_status com.eprosima.idl.parser.typecode.EnumTypeCode@1c93084c
    m_status = ::RUNNING;

}

Status::~Status()
{




}

Status::Status(
        const Status& x)
{
    m_id = x.m_id;
    m_name = x.m_name;
    m_node_kind = x.m_node_kind;
    m_status = x.m_status;
}

Status::Status(
        Status&& x)
{
    m_id = std::move(x.m_id);
    m_name = std::move(x.m_name);
    m_node_kind = x.m_node_kind;
    m_status = x.m_status;
}

Status& Status::operator =(
        const Status& x)
{

    m_id = x.m_id;
    m_name = x.m_name;
    m_node_kind = x.m_node_kind;
    m_status = x.m_status;

    return *this;
}

Status& Status::operator =(
        Status&& x)
{

    m_id = std::move(x.m_id);
    m_name = std::move(x.m_name);
    m_node_kind = x.m_node_kind;
    m_status = x.m_status;

    return *this;
}

bool Status::operator ==(
        const Status& x) const
{

    return (m_id == x.m_id && m_name == x.m_name && m_node_kind == x.m_node_kind && m_status == x.m_status);
}

bool Status::operator !=(
        const Status& x) const
{
    return !(*this == x);
}

size_t Status::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 255 + 1;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 255 + 1;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

size_t Status::getCdrSerializedSize(
        const Status& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + data.id().size() + 1;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + data.name().size() + 1;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

void Status::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{

    scdr << m_id;
    scdr << m_name;
    scdr << (uint32_t)m_node_kind;
    scdr << (uint32_t)m_status;

}

void Status::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{

    dcdr >> m_id;
    dcdr >> m_name;
    {
        uint32_t enum_value = 0;
        dcdr >> enum_value;
        m_node_kind = (NodeKind)enum_value;
    }

    {
        uint32_t enum_value = 0;
        dcdr >> enum_value;
        m_status = (StatusKind)enum_value;
    }

}

/*!
 * @brief This function copies the value in member id
 * @param _id New value to be copied in member id
 */
void Status::id(
        const std::string& _id)
{
    m_id = _id;
}

/*!
 * @brief This function moves the value in member id
 * @param _id New value to be moved in member id
 */
void Status::id(
        std::string&& _id)
{
    m_id = std::move(_id);
}

/*!
 * @brief This function returns a constant reference to member id
 * @return Constant reference to member id
 */
const std::string& Status::id() const
{
    return m_id;
}

/*!
 * @brief This function returns a reference to member id
 * @return Reference to member id
 */
std::string& Status::id()
{
    return m_id;
}
/*!
 * @brief This function copies the value in member name
 * @param _name New value to be copied in member name
 */
void Status::name(
        const std::string& _name)
{
    m_name = _name;
}

/*!
 * @brief This function moves the value in member name
 * @param _name New value to be moved in member name
 */
void Status::name(
        std::string&& _name)
{
    m_name = std::move(_name);
}

/*!
 * @brief This function returns a constant reference to member name
 * @return Constant reference to member name
 */
const std::string& Status::name() const
{
    return m_name;
}

/*!
 * @brief This function returns a reference to member name
 * @return Reference to member name
 */
std::string& Status::name()
{
    return m_name;
}
/*!
 * @brief This function sets a value in member node_kind
 * @param _node_kind New value for member node_kind
 */
void Status::node_kind(
        NodeKind _node_kind)
{
    m_node_kind = _node_kind;
}

/*!
 * @brief This function returns the value of member node_kind
 * @return Value of member node_kind
 */
NodeKind Status::node_kind() const
{
    return m_node_kind;
}

/*!
 * @brief This function returns a reference to member node_kind
 * @return Reference to member node_kind
 */
NodeKind& Status::node_kind()
{
    return m_node_kind;
}

/*!
 * @brief This function sets a value in member status
 * @param _status New value for member status
 */
void Status::status(
        StatusKind _status)
{
    m_status = _status;
}

/*!
 * @brief This function returns the value of member status
 * @return Value of member status
 */
StatusKind Status::status() const
{
    return m_status;
}

/*!
 * @brief This function returns a reference to member status
 * @return Reference to member status
 */
StatusKind& Status::status()
{
    return m_status;
}


size_t Status::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t current_align = current_alignment;







    return current_align;
}

bool Status::isKeyDefined()
{
    return false;
}

void Status::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
        
}

/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * JULEA engine using the JULEA storage framework to handle lower I/O.
 *
 *  Created on: Nov 14, 2018
 *      Author: Kira Duwe duwe@informatik.uni-hamburg.de
 */

#ifndef ADIOS2_ENGINE_JULEAMETADATA_H_
#define ADIOS2_ENGINE_JULEAMETADATA_H_

#include <julea.h>


namespace adios2
{
namespace core
{
namespace engine
{

/* ADIOS Types in ADIOS2-2.4.0 */
enum variable_type{
    STRING,
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    INT64,
    UINT64,
    FLOAT,
    DOUBLE,
    LONG_DOUBLE,
    FLOAT_COMPLEX,
    DOUBLE_COMPLEX
};
typedef enum variable_type variable_type;

union value_type{
    char *string; //TODO: needed?
    int8_t integer_8;
    uint8_t u_integer_8;
    int16_t integer_16;
    uint16_t u_integer_16;
    int32_t integer_32;
    uint32_t u_integer_32;
    int64_t integer_64;
    uint64_t u_integer_64;
    float real_float;
    double real_double;
    long double lreal_double;
    // float complex float_complex;     //not yet implemented in ADIOS2
    // double complex double_complex;   //not yet implemented in ADIOS2
};
typedef union value_type value_type;

/**
 * Metadata information to be stored in kv store or structured metadata backend.
 *
 * TODO: VariableBase.h members are stored in JULEA but are currently not used in ADIOS.
 *
 */
struct Metadata{
    char* name;

    unsigned long* shape;
    unsigned long* start;
    unsigned long* count;
    unsigned long* memory_start; //TODO
    unsigned long* memory_count; //TODO


    unsigned long shape_size;
    unsigned long start_size;
    unsigned long count_size;
    unsigned long memory_start_size; //TODO
    unsigned long memory_count_size; //TODO
    unsigned long test_header; //TODO

    size_t steps_start;
    size_t steps_count;
    size_t block_id;                //TODO
    size_t index_start;             //VariableBase.h TODO
    size_t element_size;            //VariableBase.h TODO
    size_t available_steps_start;   //VariableBase.h TODO
    size_t available_steps_count;   //VariableBase.h TODO

    variable_type var_type;

    value_type min_value;
    value_type max_value;
    value_type curr_value;

    unsigned int sizeof_var_type;   //store the actual size of the variable type
    unsigned int data_size;
    // unsigned int deferred_counter; //VariableBase.h TODO: implement!

    bool is_value;
    bool is_single_value;           //VariableBase.h TODO
    bool is_constant_dims;          //VariableBase.h TODO? //protected
    bool is_read_as_joined;         //VariableBase.h TODO
    bool is_read_as_local_value;    //VariableBase.h TODO
    bool is_random_access;          //VariableBase.h TODO
    bool is_first_streaming_step;   //VariableBase.h TODO

    //TODO: ShapeID m_ShapeID; see shape types in ADIOSTypes.h
    //TODO: Operations Map
    // size_t m_IndexStart = 0; TODO: needed?
};
typedef struct Metadata Metadata;

struct AttributeMetadata
{
    char* name;
    variable_type attr_type;
    size_t number_elements;
    bool is_single_value;
    guint data_size;
};
typedef struct AttributeMetadata AttributeMetadata;

struct JuleaInfo{
 JSemantics* semantics;
 char* name_space;
};
typedef struct JuleaInfo JuleaInfo;


} // end namespace engine
} // end namespace core
} // end namespace adios2

#endif /* ADIOS2_ENGINE_JULEAMETADATA_H_ */

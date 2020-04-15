/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * JULEA engine using the JULEA storage framework to handle lower I/O.
 *
 *  Created on: Aug 02, 2019
 *      Author: Kira Duwe duwe@informatik.uni-hamburg.de
 */

#ifndef ADIOS2_ENGINE_JULEAFORMATREADER_
#define ADIOS2_ENGINE_JULEAFORMATREADER_

#include "JuleaFormatReader.h"

#include <bson.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace adios2
{
namespace core
{
namespace engine
{

void DefineVariableInInitNew(core::IO *io, const std::string varName,
                             std::string stringType, Dims shape, Dims start,
                             Dims count, bool constantDims)
{
    const char *type = stringType.c_str();
    std::cout << "------ DefineVariableInInitNew ----------" << std::endl;
    std::cout << "------ type  ---------- " << type << std::endl;

    if (strcmp(type, "unknown") == 0)
    {
        // TODO
    }
    else if (strcmp(type, "compound") == 0)
    {
    }
    else if (strcmp(type, "string") == 0)
    {
        io->DefineVariable<std::string>(varName, shape, start, count,
                                        constantDims);
        std::cout << "Defined variable of type: " << type << std::endl;
    }
    else if (strcmp(type, "int8_t") == 0)
    {
        io->DefineVariable<int8_t>(varName, shape, start, count, constantDims);
        std::cout << "Defined variable of type: " << type << std::endl;
    }
    else if (strcmp(type, "uint8_t") == 0)
    {
        io->DefineVariable<uint8_t>(varName, shape, start, count, constantDims);
    }
    else if (strcmp(type, "int16_t") == 0)
    {
        io->DefineVariable<int16_t>(varName, shape, start, count, constantDims);
    }
    else if (strcmp(type, "uint16_t") == 0)
    {
        io->DefineVariable<uint16_t>(varName, shape, start, count,
                                     constantDims);
    }
    else if (strcmp(type, "int32_t") == 0)
    {
        io->DefineVariable<int32_t>(varName, shape, start, count, constantDims);
    }
    else if (strcmp(type, "uint32_t") == 0)
    {
        io->DefineVariable<uint32_t>(varName, shape, start, count,
                                     constantDims);
    }
    else if (strcmp(type, "int64_t") == 0)
    {
        io->DefineVariable<int64_t>(varName, shape, start, count, constantDims);
    }
    else if (strcmp(type, "uint64_t") == 0)
    {
        io->DefineVariable<uint64_t>(varName, shape, start, count,
                                     constantDims);
    }
    else if (strcmp(type, "float") == 0)
    {
        io->DefineVariable<float>(varName, shape, start, count, constantDims);
        std::cout << "Defined variable of type: " << type << std::endl;
    }
    else if (strcmp(type, "double") == 0)
    {
        io->DefineVariable<double>(varName, shape, start, count, constantDims);
        std::cout << "Defined variable of type: " << type << std::endl;
    }
    else if (strcmp(type, "long double") == 0)
    {
        io->DefineVariable<long double>(varName, shape, start, count,
                                        constantDims);
    }
    else if (strcmp(type, "complex float") == 0)
    {
    }
    else if (strcmp(type, "complex double") == 0)
    {
    }
}

void DeserializeVariableMetadata(gpointer buffer, std::string *type,
                                 Dims *shape, Dims *start, Dims *count,
                                 bool *constantDims)
{

    char tmpType[8];
    char *tmpBuffer = (char *)buffer;
    bool isConstantDims = true;

    size_t typeLen = 0;
    size_t shapeLen = 0;
    size_t startLen = 0;
    size_t countLen = 0;

    size_t shapeSize = 0;
    size_t startSize = 0;
    size_t countSize = 0;

    memcpy(&isConstantDims, tmpBuffer, sizeof(bool));
    tmpBuffer += sizeof(bool);
    // std::cout << "constantDims: " << isConstantDims << std::endl;

    memcpy(&typeLen, tmpBuffer, sizeof(size_t)); // type
    tmpBuffer += sizeof(size_t);
    // std::cout << "typeLen: " << typeLen << std::endl;

    memcpy(&tmpType, tmpBuffer, typeLen);
    tmpBuffer += typeLen;
    std::string t(tmpType);
    *type = t;
    // std::cout << "tmpType: " << tmpType << std::endl;

    memcpy(&shapeSize, tmpBuffer, sizeof(size_t)); // shape
    tmpBuffer += sizeof(size_t);
    shapeLen = sizeof(size_t) * shapeSize;

    size_t tmpShapeBuffer[shapeSize];

    memcpy(&tmpShapeBuffer, tmpBuffer, shapeLen);
    tmpBuffer += shapeLen;
    if (shapeSize > 0)
    {
        Dims tmpShape(tmpShapeBuffer, tmpShapeBuffer + shapeSize);
        *shape = tmpShape;
    }

    memcpy(&startSize, tmpBuffer, sizeof(size_t)); // start
    tmpBuffer += sizeof(size_t);
    startLen = sizeof(size_t) * startSize;

    size_t tmpStartBuffer[startSize];

    memcpy(&tmpStartBuffer, tmpBuffer, startLen);
    tmpBuffer += startLen;
    if (startSize > 0)
    {
        Dims tmpStart(tmpStartBuffer, tmpStartBuffer + startSize);
        *start = tmpStart;
    }

    memcpy(&countSize, tmpBuffer, sizeof(size_t)); // count
    tmpBuffer += sizeof(size_t);
    countLen = sizeof(size_t) * countSize;

    size_t tmpCountBuffer[countSize];

    memcpy(&tmpCountBuffer, tmpBuffer, countLen);
    tmpBuffer += countLen;
    if (countSize > 0)
    {
        Dims tmpCount(tmpCountBuffer, tmpCountBuffer + countSize);
        *count = tmpCount;
        // std::cout << "count: " << count->front() <<std::endl;
    }
}

void GetAdiosTypeString(int type, std::string *typeString)
{
    switch (type)
    {
    // case COMPOUND:
    //     //TODO
    //     break;
    // case UNKNOWN:
    //     //TODO
    //     break;
    case STRING:
        *typeString = "string";
        break;
    case INT8:
        *typeString = "int8_t";
        break;
    case UINT8:
        *typeString = "uint8_t";
        break;
    case INT16:
        *typeString = "int16_t";
        break;
    case UINT16:
        *typeString = "uint16_t";
        break;
    case INT32:
        *typeString = "int32_t";
        break;
    case UINT32:
        *typeString = "uint32_t";
        break;
    case INT64:
        *typeString = "int64_t";
        break;
    case UINT64:
        *typeString = "uint64_t";
        break;
    case FLOAT:
        *typeString = "float";
        break;
    case DOUBLE:
        *typeString = "double";
        break;
    case LONG_DOUBLE:
        *typeString = "long double";
        break;
    case COMPLEX_FLOAT:
        *typeString = "complex float";
        break;
    case COMPLEX_DOUBLE:
        *typeString = "complex double";
        break;
    }
}

void ParseAttributeFromBSON(const std::string nameSpace,
                            const std::string attrName, bson_t *bsonMetadata,
                            long unsigned int *dataSize, size_t *numberElements,
                            bool *IsSingleValue, int *type)
{
    bson_iter_t b_iter;
    gchar *key = NULL;

    if (bson_iter_init(&b_iter, bsonMetadata))
    {
        // std::cout << "++ Julea Format Reader: Bson iterator is valid"
        // << std::endl;
    }
    else
    {
        std::cout << "ERROR: Bson iterator is not valid!" << std::endl;
    }

    while (bson_iter_next(&b_iter))
    {
        if (g_strcmp0(bson_iter_key(&b_iter), "number_elements") == 0)
        {
            *numberElements = (size_t)bson_iter_int64(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "is_single_value") == 0)
        {
            *IsSingleValue = (bool)bson_iter_bool(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "data_size") == 0)
        {
            // std::cout << "___ Datasize = " << *dataSize << std::endl;
            *dataSize = bson_iter_int64(&b_iter);
            // std::cout << "___ Datasize = " << *dataSize << std::endl;
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "attr_type") == 0)
        {
            // std::cout << "___ type = " << *type << std::endl;
            *type = bson_iter_int32(&b_iter);
            // std::cout << "___ type = " << *type << std::endl;
        }
    }
    g_free(key);
}

void ParseAttributeFromBSON(const std::string nameSpace,
                            const std::string attrName, bson_t *bsonMetadata,
                            long unsigned int *completeSize,
                            size_t *numberElements, bool *IsSingleValue,
                            int *type, unsigned long **dataSizes)
{
    bson_iter_t b_iter;
    gchar *key = NULL;

    if (bson_iter_init(&b_iter, bsonMetadata))
    {
        std::cout << "++ Julea Format Reader: Bson iterator is valid"
                  << std::endl;
    }
    else
    {
        std::cout << "ERROR: Bson iterator is not valid!" << std::endl;
    }

    while (bson_iter_next(&b_iter))
    {
        if (g_strcmp0(bson_iter_key(&b_iter), "number_elements") == 0)
        {
            *numberElements = (size_t)bson_iter_int64(&b_iter);
            // std::cout << "numberElements: " << *numberElements << std::endl;
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "is_single_value") == 0)
        {
            *IsSingleValue = (bool)bson_iter_bool(&b_iter);
            // std::cout << "IsSingleValue: " << *IsSingleValue << std::endl;
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "complete_data_size") == 0)
        {
            *completeSize = bson_iter_int64(&b_iter);
            // std::cout << "___ completeSize = " << *completeSize << std::endl;
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "attr_type") == 0)
        {
            // std::cout << "___ type = " << *type << std::endl;
            *type = bson_iter_int32(&b_iter);
            // std::cout << "___ type = " << *type << std::endl;
        }
    }
    if (!*IsSingleValue && (*type == STRING))
    {
        bson_iter_init(&b_iter, bsonMetadata);
        *dataSizes = new unsigned long[*numberElements];
        for (int i = 0; i < *numberElements; ++i)
        {
            std::cout << "i: " << i << std::endl;

            key = g_strdup_printf("entry_size_%d", i);
            // std::cout << "key: " << key << std::endl;

            if (bson_iter_find(&b_iter, key))
            {
                (*dataSizes)[i] = bson_iter_int64(&b_iter);
                // std::cout << "___ Datasize = " << (*dataSizes)[i] <<
                // std::endl;
            }
            g_free(key);
        }
    }
}

void GetVariableMetadataForInitFromBSON(const std::string nameSpace,
                                        const std::string varName,
                                        bson_t *bsonMetadata, int *type,
                                        Dims *shape, Dims *start, Dims *count,
                                        bool *constantDims)
{
    bson_iter_t b_iter;
    gchar *key = NULL;
    std::cout << "------- GetVariableMetadataForInitFromBSON -----------"
              << std::endl;
    if (bson_iter_init(&b_iter, bsonMetadata))
    {
        std::cout << "++ Julea Format Reader: Bson iterator is valid"
                  << std::endl;
    }
    else
    {
        std::cout << "ERROR: Bson iterator is not valid!" << std::endl;
    }

    /* probably not very efficient */
    while (bson_iter_next(&b_iter))
    {
        if (g_strcmp0(bson_iter_key(&b_iter), "shape_size") == 0)
        {
            auto shapeSize = bson_iter_int64(&b_iter);
            std::cout << "-- JADIOS DEBUG PRINT: shapeSize = " << shapeSize
                      << std::endl;
            if (shapeSize > 0)
            {
                for (guint i = 0; i < shapeSize; ++i)
                {
                    bson_iter_next(&b_iter);
                    key = g_strdup_printf("shape_%d", i);

                    if (g_strcmp0(bson_iter_key(&b_iter), key) == 0)
                    {
                        (*shape).push_back(bson_iter_int64(&b_iter));
                    }
                }
            }
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "start_size") == 0)
        {
            auto startSize = bson_iter_int64(&b_iter);
            std::cout << "-- JADIOS DEBUG PRINT: startSize = " << startSize
                      << std::endl;

            if (startSize > 0)
            {
                for (guint i = 0; i < startSize; ++i)
                {
                    bson_iter_next(&b_iter);
                    key = g_strdup_printf("start_%d", i);

                    if (g_strcmp0(bson_iter_key(&b_iter), key) == 0)
                    {
                        (*start).push_back(bson_iter_int64(&b_iter));
                    }
                }
            }
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "count_size") == 0)
        {
            auto countSize = bson_iter_int64(&b_iter);

            if (countSize > 0)
            {

                for (guint i = 0; i < countSize; ++i)
                {
                    bson_iter_next(&b_iter);
                    key = g_strdup_printf("count_%d", i);
                    std::cout << "-- key = " << key << std::endl;

                    if (g_strcmp0(bson_iter_key(&b_iter), key) == 0)
                    {
                        (*count).push_back(bson_iter_int64(&b_iter));
                        std::cout << "-- Test in for loop: count[i] = "
                                  << bson_iter_int64(&b_iter) << std::endl;
                    }
                }
            }
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "var_type") == 0)
        {
            *type = bson_iter_int32(&b_iter);
            std::cout << "-- TYPE = " << bson_iter_int32(&b_iter) << std::endl;
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "is_constant_dims") == 0)
        {
            *constantDims = bson_iter_bool(&b_iter);
        }
    } // end while
    g_free(key);
}

void DefineVariableInInit(core::IO *io, const std::string varName, int type,
                          Dims shape, Dims start, Dims count, bool constantDims)
{
    switch (type)
    {
    // case COMPOUND:
    //     //TODO
    //     break;
    // case UNKNOWN:
    //     //TODO
    //     break;
    case STRING:
        io->DefineVariable<std::string>(varName, shape, start, count,
                                        constantDims);
        break;
    case INT8:
        io->DefineVariable<int8_t>(varName, shape, start, count, constantDims);
        break;
    case UINT8:
        io->DefineVariable<uint8_t>(varName, shape, start, count, constantDims);
        break;
    case INT16:
        io->DefineVariable<int16_t>(varName, shape, start, count, constantDims);
        break;
    case UINT16:
        io->DefineVariable<uint16_t>(varName, shape, start, count,
                                     constantDims);
        break;
    case INT32:
        io->DefineVariable<int32_t>(varName, shape, start, count, constantDims);
        break;
    case UINT32:
        io->DefineVariable<uint32_t>(varName, shape, start, count,
                                     constantDims);
        break;
    case INT64:
        io->DefineVariable<int64_t>(varName, shape, start, count, constantDims);
        break;
    case UINT64:
        io->DefineVariable<uint64_t>(varName, shape, start, count,
                                     constantDims);
        break;
    case FLOAT:
        io->DefineVariable<float>(varName, shape, start, count, constantDims);
        break;
    case DOUBLE:
        io->DefineVariable<double>(varName, shape, start, count, constantDims);
        break;
    case LONG_DOUBLE:
        io->DefineVariable<long double>(varName, shape, start, count,
                                        constantDims);
        break;
    case COMPLEX_FLOAT:
        io->DefineVariable<std::complex<float>>(varName, shape, start, count,
                                                constantDims);
        break;
    case COMPLEX_DOUBLE:
        io->DefineVariable<std::complex<double>>(varName, shape, start, count,
                                                 constantDims);
        break;
    }
}

template <class T>
void ParseVariableFromBSON(Variable<T> &variable, bson_t *bsonMetadata,
                           const std::string nameSpace,
                           long unsigned int *dataSize)
{
    bson_iter_t b_iter;
    gchar *key = NULL;
    unsigned int size;

    if (bson_iter_init(&b_iter, bsonMetadata))
    {
        std::cout << "++ Julea Format Reader: Bson iterator is valid"
                  << std::endl;
    }
    else
    {
        std::cout << "ERROR: Bson iterator is not valid!" << std::endl;
    }

    // TODO: what to do with the of the keys? max_value etc

    /* probably not very efficient */
    while (bson_iter_next(&b_iter))
    {

        if (g_strcmp0(bson_iter_key(&b_iter), "memory_start_size") == 0)
        {
            size = bson_iter_int64(&b_iter);

            if (size > 0)
            {
                for (guint i = 0; i < size; ++i)
                {
                    bson_iter_next(&b_iter);
                    key = g_strdup_printf("memory_start_%d", i);
                    if (g_strcmp0(bson_iter_key(&b_iter), key) == 0)
                    {
                        variable.m_MemoryStart[i] = bson_iter_int64(&b_iter);
                    }
                }
            }
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "memory_count_size") == 0)
        {
            size = bson_iter_int64(&b_iter);

            if (size > 0)
            {
                for (guint i = 0; i < size; ++i)
                {
                    bson_iter_next(&b_iter);
                    key = g_strdup_printf("memory_count_%d", i);
                    if (g_strcmp0(bson_iter_key(&b_iter), key) == 0)
                    {
                        variable.m_MemoryCount[i] = bson_iter_int64(&b_iter);
                    }
                }
            }
        }
        /* unsigned long */
        else if (g_strcmp0(bson_iter_key(&b_iter), "steps_start") == 0)
        {
            variable.m_StepsStart = bson_iter_int64(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "steps_count") == 0)
        {
            variable.m_StepsCount = bson_iter_int64(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "block_id") == 0)
        {
            variable.m_BlockID = bson_iter_int64(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "index_start") == 0)
        {
            variable.m_IndexStart = bson_iter_int64(&b_iter);
        }
        // else if (g_strcmp0(bson_iter_key(&b_iter) , "element_size" == 0))
        // {
        //     variable.m_ElementSize = bson_iter_int64(&b_iter); //TODO
        //     elementSize read only?!
        // }
        else if (g_strcmp0(bson_iter_key(&b_iter), "available_steps_start") ==
                 0)
        {
            variable.m_AvailableStepsStart = bson_iter_int64(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "available_steps_count") ==
                 0)
        {
            variable.m_AvailableStepsCount = bson_iter_int64(&b_iter);
        }
        /* boolean */
        // else if (g_strcmp0(bson_iter_key(&b_iter) , "is_value" == 0))
        // {
        //     variable.m_is_value = (bool)bson_iter_bool(&b_iter);
        // }
        else if (g_strcmp0(bson_iter_key(&b_iter), "data_size") == 0)
        {
            std::cout << "___ Datasize = " << *dataSize << std::endl;
            *dataSize = bson_iter_int64(&b_iter);
            std::cout << "___ Datasize = " << *dataSize << std::endl;
        }

        else if (g_strcmp0(bson_iter_key(&b_iter), "is_single_value") == 0)
        {
            variable.m_SingleValue = (bool)bson_iter_bool(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "is_constant_dims") == 0)
        {
            bool constantDims = (bool)bson_iter_bool(&b_iter);

            if (constantDims)
            {
                variable.SetConstantDims();
            }
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "is_read_as_joined") == 0)
        {
            variable.m_ReadAsJoined = (bool)bson_iter_bool(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "is_read_as_local_value") ==
                 0)
        {
            variable.m_ReadAsLocalValue = (bool)bson_iter_bool(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "is_random_access") == 0)
        {
            variable.m_RandomAccess = (bool)bson_iter_bool(&b_iter);
        }
        else if (g_strcmp0(bson_iter_key(&b_iter), "is_first_streaming_step") ==
                 0)
        {
            variable.m_FirstStreamingStep = (bool)bson_iter_bool(&b_iter);
        }
        /* value_type*/
        else if (g_strcmp0(bson_iter_key(&b_iter), "min_value") == 0)
        {
            // if (variable.m_var_type == INT32)
            // if (variable.GetType() == INT32)
            // {
            // variable.m_Min = bson_iter_int32(&b_iter);
            variable.m_Min = 42; // FIXME
            // }
            // else if (variable.m_var_type == INT64)
            // {
            //     variable.m_min_value.integer_64 = bson_iter_int64(&b_iter);
            // }
            // // else if(variable.m_var_type == UNSIGNED_LONG_LONG_INT) //FIXME
            // // data type
            // // {
            // //  bson_iter_decimal128(&b_iter, (bson_decimal128_t*)
            // // variable.m_min_value.ull_integer);
            // // }
            // else if (variable.m_var_type == FLOAT)
            // {
            //     variable.m_min_value.real_float = bson_iter_double(&b_iter);
            // }
            // else if (variable.m_var_type == DOUBLE)
            // {
            //     variable.m_min_value.real_double = bson_iter_double(&b_iter);
            // }
        }
        // else if (g_strcmp0(bson_iter_key(&b_iter) , "max_value" == 0))
        // {
        //     if (variable.m_var_type == INT32)
        //     {
        //         variable.m_max_value.integer_32 = bson_iter_int32(&b_iter);
        //     }
        //     else if (variable.m_var_type == INT64)
        //     {
        //         variable.m_max_value.integer_64 = bson_iter_int64(&b_iter);
        //     }
        //     // else if(variable.m_var_type == UNSIGNED_LONG_LONG_INT)
        //     // {
        //     //  bson_iter_decimal128(&b_iter,(bson_decimal128_t*)
        //     // variable.m_max_value.ull_integer);
        //     // }
        //     else if (variable.m_var_type == FLOAT)
        //     {
        //         variable.m_max_value.real_float =
        //             (float)bson_iter_double(&b_iter);
        //     }
        //     else if (variable.m_var_type == DOUBLE)
        //     {
        //         variable.m_max_value.real_double = bson_iter_double(&b_iter);
        //     }
        // }
        // else if (g_strcmp0(bson_iter_key(&b_iter) , "curr_value" == 0))
        // {
        //     if (variable.m_var_type == INT32)
        //     {
        //         variable.m_curr_value.integer_32 = bson_iter_int32(&b_iter);
        //     }
        //     else if (variable.m_var_type == INT64)
        //     {
        //         variable.m_curr_value.integer_64 = bson_iter_int64(&b_iter);
        //     }
        //     // else if(variable.m_var_type == UNSIGNED_LONG_LONG_INT)
        //     // {
        //     //  bson_iter_decimal128(&b_iter,(bson_decimal128_t*)
        //     // variable.m_curr_value.ull_integer);
        //     // }
        //     else if (variable.m_var_type == FLOAT)
        //     {
        //         variable.m_curr_value.real_float =
        //             (float)bson_iter_double(&b_iter);
        //     }
        //     else if (variable.m_var_type == DOUBLE)
        //     {
        //         variable.m_curr_value.real_double =
        //         bson_iter_double(&b_iter);
        //     }
        // }
        // else
        // {
        //     std::cout << "Unknown key " << bson_iter_key(&b_iter)
        //               << " when retrieving metadata for variable "
        //               << variable.m_Name << std::endl;
        // }

    } // end while
    g_free(key);
}

template <>
void ParseVarTypeFromBSON<std::string>(Variable<std::string> &variable,
                                       bson_iter_t *b_iter)
{
    // FIXME: set min, max for string?
    // bson_append_int32(bsonMetadata, "var_type", -1, STRING);
    // bson_append_int32(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_int32(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_int32(bsonMetadata, "curr_value", -1, variable.m_Value);

    std::cout << "ParseVarTypeFromBSON String: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<int8_t>(Variable<int8_t> &variable,
                                  bson_iter_t *b_iter)
{
    variable.m_Min = bson_iter_int32(b_iter);
    // bson_append_int32(bsonMetadata, "var_type", -1, INT8);
    // bson_append_int32(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_int32(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_int32(bsonMetadata, "curr_value", -1, variable.m_Value);

    std::cout << "ParseVarTypeFromBSON int8_t: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<uint8_t>(Variable<uint8_t> &variable,
                                   bson_iter_t *b_iter)
{
    // bson_append_int32(bsonMetadata, "var_type", -1, UINT8);
    // bson_append_int32(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_int32(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_int32(bsonMetadata, "curr_value", -1, variable.m_Value);

    std::cout << "ParseVarTypeFromBSON uint8_t: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<int16_t>(Variable<int16_t> &variable,
                                   bson_iter_t *b_iter)
{
    // bson_append_int32(bsonMetadata, "var_type", -1, INT16);
    // bson_append_int32(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_int32(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_int32(bsonMetadata, "curr_value", -1, variable.m_Value);
    std::cout << "ParseVarTypeFromBSON int16_t: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<uint16_t>(Variable<uint16_t> &variable,
                                    bson_iter_t *b_iter)
{
    // bson_append_int32(bsonMetadata, "var_type", -1, UINT16);
    // bson_append_int32(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_int32(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_int32(bsonMetadata, "curr_value", -1, variable.m_Value);
    std::cout << "ParseVarTypeFromBSON uint16_t: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<int32_t>(Variable<int32_t> &variable,
                                   bson_iter_t *b_iter)
{
    // bson_append_int32(bsonMetadata, "var_type", -1, INT32);
    // bson_append_int32(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_int32(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_int32(bsonMetadata, "curr_value", -1, variable.m_Value);
    std::cout << "ParseVarTypeFromBSON int32_t: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<uint32_t>(Variable<uint32_t> &variable,
                                    bson_iter_t *b_iter)
{
    // bson_append_int32(bsonMetadata, "var_type", -1,
    //                   UINT32); // FIXME: does int32 suffice?
    // bson_append_int32(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_int32(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_int32(bsonMetadata, "curr_value", -1, variable.m_Value);
    std::cout << "ParseVarTypeFromBSON uint32_t: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<int64_t>(Variable<int64_t> &variable,
                                   bson_iter_t *b_iter)
{
    // bson_append_int64(bsonMetadata, "var_type", -1, INT64);
    // bson_append_int64(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_int64(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_int64(bsonMetadata, "curr_value", -1, variable.m_Value);
    std::cout << "ParseVarTypeFromBSON int64_t: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<uint64_t>(Variable<uint64_t> &variable,
                                    bson_iter_t *b_iter)
{
    // bson_append_int64(bsonMetadata, "var_type", -1, UINT64);
    // bson_append_int64(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_int64(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_int64(bsonMetadata, "curr_value", -1, variable.m_Value);
    std::cout << "ParseVarTypeFromBSON uint64_t: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<float>(Variable<float> &variable, bson_iter_t *b_iter)
{
    // bson_append_double(bsonMetadata, "var_type", -1, FLOAT);
    // bson_append_double(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_double(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_double(bsonMetadata, "curr_value", -1, variable.m_Value);
    std::cout << "ParseVarTypeFromBSON float: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<double>(Variable<double> &variable,
                                  bson_iter_t *b_iter)
{
    // bson_append_double(bsonMetadata, "var_type", -1, DOUBLE);
    // bson_append_double(bsonMetadata, "min_value", -1, variable.Min());
    // bson_append_double(bsonMetadata, "max_value", -1, variable.Max());
    // bson_append_double(bsonMetadata, "curr_value", -1, variable.m_Value);
    std::cout << "ParseVarTypeFromBSON double: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<long double>(Variable<long double> &variable,
                                       bson_iter_t *b_iter)
{
    // FIXME: implement!
    // how to store long double in bson file?
    std::cout << "ParseVarTypeFromBSON long double: min = " << variable.Min()
              << std::endl;
}

template <>
void ParseVarTypeFromBSON<std::complex<float>>(
    Variable<std::complex<float>> &variable, bson_iter_t *b_iter)
{
    // FIXME: implement!
    // use two doubles? one for imaginary, one for real part?
    std::cout << "ParseVarTypeFromBSON std::complex<float>: min = "
              << variable.Min() << std::endl;
}

template <>
void ParseVarTypeFromBSON<std::complex<double>>(
    Variable<std::complex<double>> &variable, bson_iter_t *b_iter)
{
    // FIXME: implement!
    // use two doubles? one for imaginary, one for real part?
    std::cout << "ParseVarTypeFromBSON std::complex<double>: min = "
              << variable.Min() << std::endl;
}

// template void DefineAttributeInInit(                                       \
        core::IO *io, const std::string attrName, T *data, int type,           \
        bool IsSingleValue, size_t numberElements);

#define variable_template_instantiation(T)                                     \
    template void ParseVariableFromBSON(                                       \
        core::Variable<T> &, bson_t *bsonMetadata,                             \
        const std::string nameSpace, long unsigned int *dataSize);             \
    template void ParseVarTypeFromBSON(Variable<T> &variable,                  \
                                       bson_iter_t *b_iter);
ADIOS2_FOREACH_STDTYPE_1ARG(variable_template_instantiation)
#undef variable_template_instantiation

// ADIOS2_FOREACH_ATTRIBUTE_STDTYPE_1ARG(attribute_template_instantiation)
// #undef attribute_template_instantiation

} // end namespace engine
} // end namespace core
} // end namespace adios2
#endif /* ADIOS2_ENGINE_JULEAFORMATREADER_ */

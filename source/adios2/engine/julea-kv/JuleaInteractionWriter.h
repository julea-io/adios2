/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * JULEA engine using the JULEA storage framework to handle lower I/O.
 *
 *  Created on: Aug 01, 2019
 *      Author: Kira Duwe duwe@informatik.uni-hamburg.de
 */

#ifndef ADIOS2_ENGINE_JULEAINTERACTION_H_
#define ADIOS2_ENGINE_JULEAINTERACTION_H_

#include "JuleaKVWriter.h"
#include "JuleaMetadata.h"

namespace adios2
{
namespace core
{
namespace engine
{

void PutNameToJulea(std::string paramName, std::string nameSpace,
                    std::string kvName);
void PutVariableMetadataToJulea(const std::string nameSpace, gpointer buffer,
                                guint32 bufferLen, const std::string varName);
void PutBlockMetadataToJulea(const std::string nameSpace,
                             const std::string varName, gpointer &buffer,
                             guint32 bufferLen, const std::string stepBlockID);

/* Variable Functions */
template <class T>
void PutVariableDataToJulea(Variable<T> &variable, const T *data,
                            const std::string nameSpace, size_t currentStep,
                            size_t blockID);

/* Attribute Functions */
template <class T>
void PutAttributeDataToJulea(Attribute<T> &attribute,
                             const std::string nameSpace);

template <class T>
void PutAttributeMetadataToJulea(Attribute<T> &attribute, bson_t *bsonMetadata,
                                 const std::string nameSpace);

template <class T>
void PutAttributeDataToJuleaSmall(Attribute<T> &attribute, const T *data,
                                  const std::string nameSpace);
template <class T>
void PutAttributeMetadataToJuleaSmall(Attribute<T> &attribute,
                                      bson_t *bsonMetadata,
                                      const std::string nameSpace);

#define declare_template_instantiation(T)                                      \
    extern template void PutVariableDataToJulea(                               \
        Variable<T> &variable, const T *data, const std::string nameSpace,     \
        size_t currentStep, size_t blockID);                                   \
                                                                               \
    extern template void PutAttributeDataToJulea(Attribute<T> &attribute,      \
                                                 const std::string nameSpace); \
    extern template void PutAttributeDataToJuleaSmall(                         \
        Attribute<T> &attribute, const T *data, const std::string nameSpace);  \
    extern template void PutAttributeMetadataToJulea(                          \
        Attribute<T> &attribute, bson_t *bsonMetadata,                         \
        const std::string nameSpace);                                          \
    extern template void PutAttributeMetadataToJuleaSmall(                     \
        Attribute<T> &attribute, bson_t *bsonMetadata,                         \
        const std::string nameSpace);                                          \
    ADIOS2_FOREACH_STDTYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

} // end namespace engine
} // end namespace core
} // end namespace adios2

#endif /* ADIOS2_ENGINE_JULEAINTERACTION_H_ */

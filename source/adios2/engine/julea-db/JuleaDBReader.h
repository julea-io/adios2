/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * JULEA engine using the JULEA storage framework to handle lower I/O.
 *
 *  Created on: Jul 26, 2019
 *      Author: Kira Duwe duwe@informatik.uni-hamburg.de
 */

#ifndef ADIOS2_ENGINE_JULEADBREADER_H_
#define ADIOS2_ENGINE_JULEADBREADER_H_

// #include "adios2/ADIOSConfig.h"  //FIXME: missing
#include "adios2/core/ADIOS.h"
#include "adios2/core/Engine.h"
#include "adios2/helper/adiosFunctions.h"
// #include "adios2/toolkit/format/bp3/BP3.h" //BP3Deserializer
#include "adios2/toolkit/format/bp/bp3/BP3Serializer.h"
#include "adios2/toolkit/transportman/TransportMan.h" //transport::TransportsMan

#include <complex.h>
#include <glib.h>
#include <julea.h>

namespace adios2
{
namespace core
{
namespace engine
{

class JuleaDBReader : public Engine
{
public:
    /**
     * Constructor for single BP capsule engine, writes in BP format into a
     * single
     * heap capsule
     * @param name unique name given to the engine
     * @param accessMode
     * @param mpiComm
     * @param method
     * @param debugMode
     * @param hostLanguage
     */
    JuleaDBReader(IO &adios, const std::string &name, const Mode mode,
                  helper::Comm comm);

    ~JuleaDBReader();
    // StepStatus BeginStep(StepMode mode = StepMode::NextAvailable,
    //                      const float timeoutSeconds = -1.0) final;
    StepStatus BeginStep(StepMode mode = StepMode::Read,
                         const float timeoutSeconds = -1.0) final;
    size_t CurrentStep() const final;
    void EndStep() final;
    void PerformGets() final;

private:
    // JuleaInfo *m_JuleaInfo;
    int m_Verbosity = 5; // TODO: changed to 5 for debugging
    int m_ReaderRank;    // my rank in the readers' comm

    // step info should be received from the writer side in BeginStep()
    size_t m_CurrentStep = -1;
    bool m_FirstStep = true;

    // EndStep must call PerformGets if necessary
    bool m_NeedPerformGets = false;

    bool m_CollectiveMetadata = true;

    /** Parameter to flush transports at every number of steps, to be used at
     * EndStep */
    size_t m_FlushStepsCount = 1;

    /** tracks Put and Get variables in deferred mode */
    std::set<std::string> m_DeferredVariables;

    /** tracks the overall size of deferred variables */
    size_t m_DeferredVariablesDataSize = 0;

    /** statistics verbosity, only 0 is supported */
    unsigned int m_StatsLevel = 0;

    void Init() final; ///< called from constructor, gets the selected Skeleton
                       /// transport method from settings

    /** Parses parameters from IO SetParameters */
    void InitParameters() final;
    /** Parses transports and parameters from IO AddTransport */
    void InitTransports() final;

    // template <class T>
    void InitVariables(); // needs to be final? HELP

#define declare_type(T)                                                        \
    void DoGetSync(Variable<T> &, T *) final;                                  \
    void DoGetDeferred(Variable<T> &, T *) final;
    ADIOS2_FOREACH_STDTYPE_1ARG(declare_type)
#undef declare_type

    template <class T>
    void GetSyncCommon(Variable<T> &variable, T *data);

    template <class T>
    void GetDeferredCommon(Variable<T> &variable, T *data);

    void DoClose(const int transportIndex = -1);

    // void ReadData();

    // void AggregateReadData();

    /**
     * DESIGN: is this function needed here? is there something one would want
     * to do different with a variable coming from JULEA?
     *
     * Sets read block information from the available metadata information
     * @param variable
     * @param blockInfo
     */
    template <class T>
    void SetVariableBlockInfo(core::Variable<T> &variable,
                              typename core::Variable<T>::Info &blockInfo);
};

} // end namespace engine
} // end namespace core
} // end namespace adios2

#endif /* ADIOS2_ENGINE_JULEADBREADER_H_ */

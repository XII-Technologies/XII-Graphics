#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <Foundation/Configuration/Startup.h>
#include <Foundation/Profiling/Profiling.h>
#include <GraphicsFoundation/CommandEncoder/CommandList.h>
#include <GraphicsFoundation/Device/Device.h>
#include <GraphicsFoundation/Profiling/Profiling.h>
#include <GraphicsFoundation/Resources/Query.h>

#if XII_ENABLED(XII_USE_PROFILING)

#  if 0
struct GPUTimingScope
{
  XII_DECLARE_POD_TYPE();

  xiiGALQueryHandle m_BeginTimestamp;
  xiiGALQueryHandle m_EndTimestamp;
  char              m_szName[48];
};

class GPUProfilingSystem
{
public:
  static void ProcessTimestamps(const xiiGALDeviceEvent& e)
  {
    if (e.m_Type != xiiGALDeviceEventType::AfterEndFrame)
      return;

    if (s_PendingScopes.GetCount() > s_uiExpectedQueryLimit)
    {
      xiiLog::Warning("There are {} pending queries, which exceeds the specified expected limit ({}).", s_PendingScopes.GetCount(), s_uiExpectedQueryLimit);
    }

    while (!s_PendingScopes.IsEmpty())
    {
      auto& timingScope = s_PendingScopes.PeekBack();

      xiiGALQuery* pStartQuery = xiiGALDevice::GetDefaultDevice()->GetQuery(timingScope.m_BeginTimestamp);
      xiiGALQuery* pEndQuery   = xiiGALDevice::GetDefaultDevice()->GetQuery(timingScope.m_EndTimestamp);

      xiiGALQueryDataTimestamp startTimestampData;

      // Do not invalidate the query until we also get the end timestamp.
      bool bIsDataAvailable = pStartQuery->GetData(&startTimestampData, sizeof(startTimestampData), false);
      if (bIsDataAvailable)
      {
        xiiGALQueryDataTimestamp endTimestampData;

        bIsDataAvailable = pEndQuery->GetData(&endTimestampData, sizeof(endTimestampData));
        if (bIsDataAvailable)
        {
          pStartQuery->Invalidate();

          xiiTime startTime = xiiTime::Seconds(static_cast<double>(startTimestampData.m_uiCounter) / static_cast<double>(startTimestampData.m_uiFrequency));
          xiiTime endTime   = xiiTime::Seconds(static_cast<double>(endTimestampData.m_uiCounter) / static_cast<double>(endTimestampData.m_uiFrequency));

          if (!endTime.IsZero() && !startTime.IsZero())
          {
#    if XII_ENABLED(XII_COMPILE_FOR_DEBUG)
            static bool warnOnRingBufferOverun = true;
            if (warnOnRingBufferOverun && endTime < startTime)
            {
              warnOnRingBufferOverun = false;
              xiiLog::Error("Profiling end is before start, the timestamp ring buffer was probably overrun.");
            }
#    endif
            xiiProfilingSystem::AddGPUScope(timingScope.m_szName, startTime, endTime);
          }
        }

        s_TimingScopes.PushBack(std::move(timingScope));
        s_PendingScopes.PopBack();
      }
    }
  }

  static GPUTimingScope& AllocateScope()
  {
    auto& scope = s_TimingScopes.ExpandAndGetRef();

    {
      xiiGALQueryCreationDescription queryDesc;
      queryDesc.m_sName      = "Duration start timestamp query.";
      queryDesc.m_Type       = xiiGALQueryType::Timestamp;
      scope.m_BeginTimestamp = xiiGALDevice::GetDefaultDevice()->CreateQuery(queryDesc);

      XII_ASSERT_DEV(!scope.m_BeginTimestamp.IsInvalidated(), "Failed to create start query.");
    }

    {
      xiiGALQueryCreationDescription queryDesc;
      queryDesc.m_sName    = "Duration end timestamp query.";
      queryDesc.m_Type     = xiiGALQueryType::Timestamp;
      scope.m_EndTimestamp = xiiGALDevice::GetDefaultDevice()->CreateQuery(queryDesc);

      XII_ASSERT_DEV(!scope.m_BeginTimestamp.IsInvalidated(), "Failed to create end query.");
    }

    return scope;
  }

  static constexpr xiiUInt32                                        s_uiExpectedQueryLimit = 5U;
  static xiiDeque<GPUTimingScope, xiiStaticAllocatorWrapper>        s_TimingScopes;
  static xiiDynamicArray<GPUTimingScope, xiiStaticAllocatorWrapper> s_PendingScopes;

private:
  static void OnEngineStartup()
  {
    xiiGALDevice::GetDefaultDevice()->m_Events.AddEventHandler(&GPUProfilingSystem::ProcessTimestamps);
  }

  static void OnEngineShutdown()
  {
    for (xiiUInt32 i = 0; i < s_TimingScopes.GetCount(); ++i)
    {
      auto scope = s_TimingScopes[i];

      xiiGALDevice::GetDefaultDevice()->DestroyQuery(scope.m_BeginTimestamp);
      xiiGALDevice::GetDefaultDevice()->DestroyQuery(scope.m_EndTimestamp);
    }
    s_TimingScopes.Clear();

    for (xiiUInt32 i = 0; i < s_PendingScopes.GetCount(); ++i)
    {
      auto scope = s_PendingScopes[i];

      xiiGALDevice::GetDefaultDevice()->DestroyQuery(scope.m_BeginTimestamp);
      xiiGALDevice::GetDefaultDevice()->DestroyQuery(scope.m_EndTimestamp);
    }
    s_PendingScopes.Clear();

    xiiGALDevice::GetDefaultDevice()->m_Events.RemoveEventHandler(&GPUProfilingSystem::ProcessTimestamps);
  }

  XII_MAKE_SUBSYSTEM_STARTUP_FRIEND(GraphicsFoundation, GPUProfilingSystem);
};

// clang-format off
XII_BEGIN_SUBSYSTEM_DECLARATION(GraphicsFoundation, GPUProfilingSystem)

  BEGIN_SUBSYSTEM_DEPENDENCIES
    "Foundation",
    "Core"
  END_SUBSYSTEM_DEPENDENCIES

  ON_HIGHLEVELSYSTEMS_STARTUP
  {
    // GPUProfilingSystem::OnEngineStartup();
  }

  ON_HIGHLEVELSYSTEMS_SHUTDOWN
  {
    // GPUProfilingSystem::OnEngineShutdown();
  }

XII_END_SUBSYSTEM_DECLARATION;
// clang-format on

xiiDeque<GPUTimingScope, xiiStaticAllocatorWrapper>        GPUProfilingSystem::s_TimingScopes;
xiiDynamicArray<GPUTimingScope, xiiStaticAllocatorWrapper> GPUProfilingSystem::s_PendingScopes;

//////////////////////////////////////////////////////////////////////////

xiiProfilingScopeAndMarker::xiiProfilingScopeAndMarker(xiiGALCommandList* pCommandList, xiiStringView sName) :
  xiiProfilingScope(sName, {}, xiiTime::Zero()), m_pCommandList(pCommandList)
{
  m_pTimingScope = Start(pCommandList, sName);
}

xiiProfilingScopeAndMarker::~xiiProfilingScopeAndMarker()
{
  Stop(m_pCommandList, m_pTimingScope);
}

GPUTimingScope* xiiProfilingScopeAndMarker::Start(xiiGALCommandList* pCommandList, xiiStringView sName)
{
  pCommandList->BeginDebugGroup(sName);

#    if 0
  auto& timingScope = GPUProfilingSystem::AllocateScope();

  pCommandList->EndQuery(timingScope.m_BeginTimestamp);
  xiiStringUtils::Copy(timingScope.m_szName, XII_ARRAY_SIZE(timingScope.m_szName), sName.GetStartPointer(), sName.GetEndPointer());

  return &timingScope;
#    else
  return nullptr;
#    endif
}

void xiiProfilingScopeAndMarker::Stop(xiiGALCommandList* pCommandList, GPUTimingScope*& ref_pTimingScope)
{
  pCommandList->EndDebugGroup();

#    if 0
  pCommandList->EndQuery(ref_pTimingScope->m_EndTimestamp);

  auto durationQuery = std::move(GPUProfilingSystem::s_TimingScopes.PeekBack());
  GPUProfilingSystem::s_TimingScopes.PopBack();

  GPUProfilingSystem::s_PendingScopes.Insert(std::move(durationQuery), 0);

  ref_pTimingScope = nullptr;
#    endif
}

#  endif
#endif

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Profiling_Implementation_Profiling);

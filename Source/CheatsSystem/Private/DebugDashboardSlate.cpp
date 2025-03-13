#include "DebugDashboardSlate.h"
#include "DebugSystem.h"
#include "DebugVar.h"
#include "DebugSignal.h"
#include "Widgets/SWidget.h"

namespace dbg
{

    CHEATSSYSTEM_API TSharedPtr<SWidget> make_slate_widget_impl(const var<bool>& i_var)
    {
        return nullptr;
    }

    CHEATSSYSTEM_API TSharedPtr<SWidget> make_slate_widget_impl(const var<float>& i_var)
    {
        return nullptr;
    }

    CHEATSSYSTEM_API TSharedPtr<SWidget> make_slate_widget_impl(const signal& i_var)
    {
        return nullptr;
    }


    DebugDashboardSlate::DebugDashboardSlate()
    {
        Initialize();
    }

    DebugDashboardSlate::~DebugDashboardSlate()
    {

    }

    void DebugDashboardSlate::Initialize()
    {
        m_onVarRegisteredConnection = DebugSystem::GetInstance().onVarRegistered.connect(std::bind(&DebugDashboardSlate::OnVarRegistered, this, std::placeholders::_1));
        m_onVarUnregisteredConnection = DebugSystem::GetInstance().onVarUnregistered.connect(std::bind(&DebugDashboardSlate::OnVarUnregistered, this, std::placeholders::_1));

        for (std::reference_wrapper<detail::var> var : DebugSystem::GetInstance().ListAllVars())
        {
            OnVarRegistered(var);
        }
    }

    void DebugDashboardSlate::OnVarRegistered(std::reference_wrapper<detail::var> i_var)
    {
        reinterpret_cast<slate::slate_capable*>(&i_var.get())->make_slate_widget();
    }

    void DebugDashboardSlate::OnVarUnregistered(std::reference_wrapper<detail::var> i_var)
    {
    }
}
#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

#include <Sennet/Sennet.hpp>

#include <SennetZED/Messages/Request.hpp>
#include <SennetZED/Messages/Response.hpp>
#include <SennetZED/Primitives/Settings.hpp>

namespace Sennet
{

namespace ZED
{

class ControlLayer : public Layer
{
public:
	ControlLayer();
	~ControlLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;

	virtual void OnEvent(Event& event) override;
	virtual void OnMessage(Ref<Message> msg) override;

private:
	bool OnCommandResponse(Ref<CommandResponse> msg);
	bool OnSettingsResponse(Ref<SettingsResponse> msg);
	bool OnStateResponse(Ref<StateResponse> msg);

	void RenderControlWindow();
	void RenderControlExportCommandsWindow();
	void RenderControlExportSettingsWindow();

	void RenderInitParametersTreeNode();
	void RenderRecordingParametersTreeNode();
	void RenderRuntimeParametersTreeNode();

private:
	InitParameters m_InitParameters;
	RecordingParameters m_RecordingParameters;
	RuntimeParameters m_RuntimeParameters;
};

}
}

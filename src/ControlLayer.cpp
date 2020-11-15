#include "Sennet/ZED/ControlLayer.hpp"

namespace Sennet { namespace ZED {

ControlLayer::ControlLayer()
	: Layer("ControlLayer"), m_CameraController(1280.0f / 720.0f, false)
{
}

ControlLayer::~ControlLayer()
{
}

void ControlLayer::OnAttach()
{
	// Set up client.
	m_Client = CreateRef<Client>();

	// Set up panels.
	m_ClientPanel.SetClient(m_Client);
	m_InitParametersPanel.SetClient(m_Client);
	m_RecordingParametersPanel.SetClient(m_Client);
	m_RuntimeParametersPanel.SetClient(m_Client);
	m_SensorControllerPanel.SetClient(m_Client);
}

void ControlLayer::OnDetach()
{
}

void ControlLayer::OnUpdate(Timestep ts)
{
	while (!m_Client->Incoming().empty())
	{
		auto message = m_Client->Incoming().pop_front().Msg;
		OnMessage(message);
	}

	m_CameraController.OnUpdate(ts);

	RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Sennet::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.6f, 0.9f }, 
		{ 0.8f, 0.2f, 0.3f, 1.0f });
	Renderer2D::EndScene();
}

void ControlLayer::OnImGuiRender()
{
	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::SetNextWindowSize(ImVec2(500,600));
	if (ImGui::Begin("ZED"))
	{
		m_ClientPanel.OnImGuiRender();
		ImGui::Separator();
		m_InitParametersPanel.OnImGuiRender();
		m_RecordingParametersPanel.OnImGuiRender();
		m_RuntimeParametersPanel.OnImGuiRender();
		ImGui::End();
	}
}

void ControlLayer::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}

void ControlLayer::OnMessage(Message<MessageTypes>& message)
{
	switch (message.Header.ID)
	{
		// Server Messages.
		case MessageTypes::ServerPing:
			m_ClientPanel.OnServerPing(message);
			break;
		case MessageTypes::ServerSynchronize:
			m_ClientPanel.OnServerSynchronize(message);
			break;
		case MessageTypes::ServerAccept:
			m_ClientPanel.OnServerAccept(message);
			break;
		case MessageTypes::ServerDeny:
			m_ClientPanel.OnServerDeny(message);
			break;
		// Sensor Controller Messages.
		case MessageTypes::SensorControllerAccept:
			m_SensorControllerPanel.OnSensorControllerAccept(
				message);
			break;
		case MessageTypes::SensorControllerDeny:
			m_SensorControllerPanel.OnSensorControllerDeny(message);
			break;
		case MessageTypes::Image:
			uint32_t width, height, channels;
			message >> channels >> height >> width;
			auto image = CreateRef<Image<uint8_t>>(width, height, 
				channels);
			message >> image->GetBuffer();
			m_SensorControllerPanel.SetImage(image);
			break;
	}
}

}}

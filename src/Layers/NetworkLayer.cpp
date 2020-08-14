#include <SennetZED/Layers/NetworkLayer.hpp>

namespace Sennet
{

namespace ZED
{

NetworkLayer::NetworkLayer(const unsigned short port, uint64_t waitFor)
	: Layer("Network"), m_ConnectionManager(port, waitFor)
{
	if (Sennet::Application::GetPtr())
	{
		m_ConnectionManager.SetMessageCallback(std::bind(
			&Sennet::Application::OnMessage, 
			&Sennet::Application::Get(),
			std::placeholders::_1));

	}
	else if (Sennet::Node::GetPtr())
	{
		m_ConnectionManager.SetMessageCallback(std::bind(
			&Sennet::Node::OnMessage, 
			&Sennet::Node::Get(), 
			std::placeholders::_1));
	}
}

void NetworkLayer::OnAttach()
{
	m_ConnectionManager.Start();
}

void NetworkLayer::OnDetach()
{
}

void NetworkLayer::OnUpdate(Timestep ts)
{
}

void NetworkLayer::OnImGuiRender()
{
	RenderNetworkWindow();
}

void NetworkLayer::OnEvent(Event& event)
{
}

void NetworkLayer::OnMessage(Ref<Message> msg)
{
	SN_TRACE("[NetworkLayer] Got message: {0}", msg->ToString());
}

void NetworkLayer::Connect(const std::string address, const std::string port)
{
	SN_TRACE("[NetworkLayer] Connecting to {0}:{1}.", address, port);
	m_ConnectionManager.Connect(address, port);
}

void NetworkLayer::RenderNetworkWindow()
{
	ImGui::Begin("Network");

	RenderNetworkWindowConnect();
	RenderNetworkWindowConnections();

	ImGui::End();
}

void NetworkLayer::RenderNetworkWindowConnect()
{
	bool collapsed = !ImGui::CollapsingHeader("Connect");
	ImGui::SameLine();
	Sennet::ImGuiHelpMarker("Connect to a node or application with an active "
		"connection manager.");

	if (collapsed)
        	return;

	ImGui::PushID("Connect");
		
	static char addressBuffer[20];
	static char portBuffer[10];
	ImGui::InputText("Address", addressBuffer, IM_ARRAYSIZE(addressBuffer));
	ImGui::InputText("Port", portBuffer, IM_ARRAYSIZE(portBuffer));
	if(ImGui::Button("Connect"))
	{
		ImGui::Text("Connecting...");
		try
		{
			Connect(addressBuffer, portBuffer);
		}
		catch (std::exception e)
		{
			SN_TRACE("Could not connect.");
		}
	}
	else
	{
		ImGui::Dummy(ImVec2(0.0f, 13.0f));
	}
}

void NetworkLayer::RenderNetworkWindowConnections()
{
	bool collapsed = !ImGui::CollapsingHeader("Current Connections");
	ImGui::SameLine();
	Sennet::ImGuiHelpMarker("Information about the connection managers current "
		"connections.");
	
	if (collapsed)
		return;

	ImGui::PushID("Current Connections");

	ImGui::Columns(2, "EndpointColumns");
	ImGui::Text("Local endpoints");
	auto localEndpoints = m_ConnectionManager.GetLocalEndpointsData();
	auto remoteEndpoints = m_ConnectionManager.GetRemoteEndpointsData();

	for (auto endpoint : *localEndpoints)
	{
		ImGui::Text("%s:%hu", endpoint.first.c_str(), endpoint.second);
	}

	ImGui::NextColumn();

	ImGui::Text("Remote endpoints");
	for (auto endpoint : *remoteEndpoints)
	{
		ImGui::Text("%s:%hu", endpoint.first.c_str(), endpoint.second);
	}

	ImGui::Columns(1);
}

}
}

#pragma once
#include "Sennet/Sennet.hpp"

#include "Sennet/ZED/Messages.hpp"
#include "Sennet/ZED/Recorder.hpp"

#include "Sennet/ZED/InitParameters.hpp"
#include "Sennet/ZED/RecordingParameters.hpp"
#include "Sennet/ZED/RuntimeParameters.hpp"

namespace Sennet { namespace ZED {

class RecordServer : public Sennet::Server<MessageTypes>
{
public:
	RecordServer(const uint16_t& port, const std::string& root);
	virtual ~RecordServer();

protected:
	virtual bool OnClientConnect(Ref<Connection<MessageTypes>> client)
		override;

	virtual void OnClientDisconnect(Ref<Connection<MessageTypes>>)
		override;

	virtual void OnMessage(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) override;

private:
	void OnPingRequest(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnSynchronizationRequest(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnInitializationRequest(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnShutdownRequest(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnStartRecordRequest(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnStopRecordRequest(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnInitParametersUpdate(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnRecordingParametersUpdate(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnRuntimeParametersUpdate(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnSettingsRequest(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;

	void OnImageRequest(Ref<Connection<MessageTypes>> client,
		Message<MessageTypes>& message) const;
		
private:
	Recorder m_Recorder;
	uint8_t m_Clients = 0;
};

}}
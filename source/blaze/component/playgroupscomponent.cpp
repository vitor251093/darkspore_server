
// Include
#include "playgroupscomponent.h"

#include "blaze/client.h"
#include "blaze/functions.h"
#include "utils/functions.h"

#include <iostream>

/*
	Packet IDs
		0x01 = CreatePlaygroup
		0x02 = DestroyPlaygroup
		0x03 = JoinPlaygroup
		0x04 = LeavePlaygroup
		0x05 = SetPlaygroupAttributes
		0x06 = SetMemberAttributes
		0x07 = KickPlaygroupMember
		0x08 = SetPlaygroupJoinControls
		0x09 = FinalizePlaygroupCreation
		0x0A = LookupPlaygroupInfo
		0x0B = ResetPlaygroupSession

	Notification Packet IDs
		0x32 = NotifyDestroyPlaygroup
		0x33 = NotifyJoinPlaygroup
		0x34 = NotifyMemberJoinedPlaygroup
		0x35 = NotifyMemberRemovedFromPlaygroup
		0x36 = NotifyPlaygroupAttributesSet
		0x45 = NotifyMemberAttributesSet
		0x4F = NotifyLeaderChange
		0x50 = NotifyMemberPermissionsChange
		0x55 = NotifyJoinControlsChange
		0x56 = NotifyXboxSessionInfo
		0x57 = NotifyXboxSessionChange

	Blaze fields
		INFO (PlaygroupInfo)
			ATTR = 0x54
			ENBV = 0x50
			HNET = 0x14
			HSID = 0x48
			JOIN = 0x1C
			MLIM = 0x40
			NAME = 0x24
			NTOP = 0x1C
			OWNR = 0x34
			PGID = 0x38
			PRES = 0x1C
			UKEY = 0x24
			UPRS = 0x50
			UUID = 0x24
			VOIP = 0x1C
			XNNC = 0x20
			XSES = 0x20

	Notify Packets
		NotifyDestroyPlaygroup
			PGID = 0x38
			REAS = 0x1C

		NotifyJoinPlaygroup
			INFO = 0x18
			MLST = 0x58
			USER = 0x34

		NotifyMemberJoinedPlaygroup
			MEMB = 0x18
			PGID = 0x38

		NotifyMemberRemovedFromPlaygroup
			MLST = 0x34
			PGID = 0x38
			REAS = 0x1C

		NotifyPlaygroupAttributesSet
			ATTR = 0x54
			PGID = 0x38

		NotifyMemberAttributesSet
			ATTR = 0x54
			EID = 0x34
			PGID = 0x38

		NotifyLeaderChange
			HSID = 0x48
			LID = 0x34
			PGID = 0x38

		NotifyMemberPermissionsChange
			LID = 0x34
			PERM = 0x28
			PGID = 0x38

		NotifyJoinControlsChange
			OPEN = 0x1C
			PGID = 0x38

		NotifyXboxSessionInfo
			PGID = 0x38
			PRES = 0x50
			XNNC = 0x20
			XSES = 0x20
*/

// Blaze
namespace Blaze {
	// PlaygroupsComponent
	void PlaygroupsComponent::Parse(Client* client, const Header& header) {
		switch (header.command) {
			case 0x01: CreatePlaygroup(client, header);           break;
			case 0x02: DestroyPlaygroup(client, header);          break;
			case 0x03: JoinPlaygroup(client, header);             break;
			case 0x04: LeavePlaygroup(client, header);            break;
			case 0x05: SetPlaygroupAttributes(client, header);    break;
			case 0x06: SetMemberAttributes(client, header);       break;
			case 0x07: KickPlaygroupMember(client, header);       break;
			case 0x08: SetPlaygroupJoinControls(client, header);  break;
			case 0x09: FinalizePlaygroupCreation(client, header); break;
			case 0x0A: LookupPlaygroupInfo(client, header);       break;
			case 0x0B: ResetPlaygroupSession(client, header);     break;

			default:
				logger::error("Unknown playgroups command: " + header.command);
				break;
		}
	}

	void PlaygroupsComponent::NotifyDestroyPlaygroup(Client* client, uint32_t playgroupId, uint32_t reason) {
		TDF::Packet packet;
		packet.put_integer("PGID", playgroupId);
		packet.put_integer("REAS", reason);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x32
		}, packet);
	}

	void PlaygroupsComponent::NotifyJoinPlaygroup(Client* client) {
		const auto& user = client->get_user();
		if (!user) {
			return;
		}

		auto userId = user->get_id();

		PlaygroupInfo info;
		info.enbv = true;
		info.hostSlotId = 0;
		info.state = PlaygroupJoinState::Open;
		info.pres = PresenceMode::Standard;
		info.ownerId = userId;
		info.memberLimit = 1;
		info.ntop = GameNetworkTopology::ClientServerDedicated;
		info.playgroupId = 1;
		info.ukey = "what";
		info.uprs = true;
		info.name = "Test playgroup";
		info.uuid = "71bc4bdb-82ec-494d-8d75-ca5123b827ac";

		TDF::Packet packet;
		packet.push_struct("INFO");
		info.Write(packet);
		packet.pop();

		packet.push_list("MLST", TDF::Type::Struct);
		packet.pop();

		packet.put_integer("USER", userId);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x33
		}, packet);
	}

	void PlaygroupsComponent::NotifyMemberJoinedPlaygroup(Client* client, uint32_t playgroupId) {
		PlaygroupMemberInfo memberInfo;
		memberInfo.jtim = 0;
		memberInfo.permissions = 0;
		memberInfo.slot = 0;

		UserIdentification& memberUser = memberInfo.user;
		memberUser.id = 0;
		memberUser.localization = 0;
		memberUser.name = "Unknown";

		TDF::Packet packet;
		packet.push_struct("MEMB");
		memberInfo.Write(packet);
		packet.pop();

		packet.put_integer("PGID", playgroupId);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x34
		}, packet);
	}

	void PlaygroupsComponent::NotifyMemberRemovedFromPlaygroup(Client* client, uint32_t playgroupId, uint32_t reason) {
		TDF::Packet packet;
		packet.put_integer("MLST", 0);
		packet.put_integer("PGID", playgroupId);
		packet.put_integer("REAS", reason);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x35
		}, packet);
	}

	void PlaygroupsComponent::NotifyPlaygroupAttributesSet(Client* client, uint32_t playgroupId) {
		TDF::Packet packet;
		packet.push_map("ATTR", TDF::Type::String, TDF::Type::String);
		// playgroup attributes
		packet.pop();

		packet.put_integer("PGID", playgroupId);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x36
		}, packet);
	}

	void PlaygroupsComponent::NotifyMemberAttributesSet(Client* client, uint32_t playgroupId) {
		TDF::Packet packet;
		packet.push_map("ATTR", TDF::Type::String, TDF::Type::String);
		// member attributes
		packet.pop();

		packet.put_integer("EID", 0); // member id?
		packet.put_integer("PGID", playgroupId);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x45
		}, packet);
	}

	void PlaygroupsComponent::NotifyLeaderChange(Client* client, uint32_t playgroupId) {
		TDF::Packet packet;
		packet.put_integer("HSID", 0); // new host(leader) slot id?
		packet.put_integer("LID", 0); // Leader ID?
		packet.put_integer("PGID", playgroupId);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x4F
		}, packet);
	}

	void PlaygroupsComponent::NotifyMemberPermissionsChange(Client* client, uint32_t playgroupId) {
		TDF::Packet packet;
		packet.put_integer("LID", 0);
		packet.put_integer("PERM", 0);
		packet.put_integer("PGID", playgroupId);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x50
		}, packet);
	}

	void PlaygroupsComponent::NotifyJoinControlsChange(Client* client, uint32_t playgroupId) {
		TDF::Packet packet;
		packet.put_integer("OPEN", PlaygroupJoinState::Open);
		packet.put_integer("PGID", playgroupId);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x55
		}, packet);
	}

	void PlaygroupsComponent::NotifyXboxSessionInfo(Client* client, uint32_t playgroupId) {
		TDF::Packet packet;
		packet.put_integer("PGID", playgroupId);
		packet.put_integer("PRES", PresenceMode::Standard);
		packet.put_blob("XNNC", nullptr, 0);
		packet.put_blob("XSES", nullptr, 0);

		client->notify({
			.component = Component::Playgroups,
			.command = 0x56
		}, packet);
	}

	void PlaygroupsComponent::NotifyXboxSessionChange(Client* client, uint32_t playgroupId) {
		NotifyXboxSessionInfo(client, playgroupId);
	}

	void PlaygroupsComponent::CreatePlaygroup(Client* client, Header header) {
		logger::info("CreatePlaygroup");
	}

	void PlaygroupsComponent::DestroyPlaygroup(Client* client, Header header) {
		logger::info("DestroyPlaygroup");
	}

	void PlaygroupsComponent::JoinPlaygroup(Client* client, Header header) {
		logger::info("JoinPlaygroup");
	}

	void PlaygroupsComponent::LeavePlaygroup(Client* client, Header header) {
		logger::info("LeavePlaygroup");
	}

	void PlaygroupsComponent::SetPlaygroupAttributes(Client* client, Header header) {
		logger::info("SetPlaygroupAttributes");
	}

	void PlaygroupsComponent::SetMemberAttributes(Client* client, Header header) {
		logger::info("SetMemberAttributes");
	}

	void PlaygroupsComponent::KickPlaygroupMember(Client* client, Header header) {
		logger::info("KickPlaygroupMember");
	}

	void PlaygroupsComponent::SetPlaygroupJoinControls(Client* client, Header header) {
		logger::info("SetPlaygroupJoinControls");
	}

	void PlaygroupsComponent::FinalizePlaygroupCreation(Client* client, Header header) {
		logger::info("FinalizePlaygroupCreation");
	}

	void PlaygroupsComponent::LookupPlaygroupInfo(Client* client, Header header) {
		logger::info("LookupPlaygroupInfo");
	}

	void PlaygroupsComponent::ResetPlaygroupSession(Client* client, Header header) {
		logger::info("ResetPlaygroupSession");
	}
}

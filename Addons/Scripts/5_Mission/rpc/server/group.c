class PM_RPC_S_group
{
	void PM_RPC_S_group()
	{
		GetRPCManager().AddRPC("PartyMe", "PlayerLeaveGroup", this, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("PartyMe", "PlayerKickGroup", this, SingleplayerExecutionType.Both);
	}

	void PlayerLeaveGroup(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		Print("[PartyMe] Player " + sender.GetName() + " is leaving group.");
		PM_GetGroupManager().LeaveGroup(sender.GetId());
	}

	/*
		Data:
			Param1: Player trying to kick
			Param2: Player to be kicked
		Event:
			playerIdFrom: ID of player leaving group
	*/
	void PlayerKickGroup(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		Param2<string, string> data;
		if (!ctx.Read(data)) return;
		string owner1 = PM_GetGroupManager().GetPlayerGroupId(data.param1);
		string owner2 = PM_GetGroupManager().GetPlayerGroupId(data.param2);
		PlayerIdentity toKickIdentity;

		Print("[PartyMe] Kick en cours !");
		toKickIdentity = PM_GetGroupManager().GetPlayerIdentity(data.param2);
		Print("[PartyMe][Kick] param1: " + data.param1);
		Print("[PartyMe][Kick] param2: " + data.param2);
		Print("[PartyMe][Kick] owner1: " + owner1);
		Print("[PartyMe][Kick] owner2: " + owner2);
		Print("[PartyMe][Kick] check 1: " + owner1 != string.Empty);
		Print("[PartyMe][Kick] check 2: " + owner2 != string.Empty);
		Print("[PartyMe][Kick] check 3: " + owner1 == owner2);
		Print("[PartyMe][Kick] check 4: " + data.param1 == owner1);
		Print("[PartyMe][Kick] check 5: " + toKickIdentity);
		if (owner1 != string.Empty && owner2 != string.Empty && owner1 == owner2 && data.param1 == owner1 && toKickIdentity)
		{
			Print("[PartyMe] Kick sur " + data.param2 + " !");
			PM_GetGroupManager().LeaveGroup(data.param2);
			GetRPCManager().SendRPC("PartyMe", "GroupDestroyed", NULL, false, toKickIdentity);
		}
	}
};
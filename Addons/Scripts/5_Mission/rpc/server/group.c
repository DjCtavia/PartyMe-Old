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
		ref PM_Event_Params eventParams = new PM_Event_Params;

		eventParams.playerIdFrom = sender.GetId();
		eventParams.playerIdFromIdentity = sender;
		PM_GetEvents().CallEvent("PlayerLeaveGroup", eventParams);
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

		toKickIdentity = PM_GetGroupManager().GetPlayerIdentity(data.param2);
		if (owner1 != string.Empty && owner2 != string.Empty && owner1 == owner2 && data.param1 == owner1 && toKickIdentity)
		{
			PM_GetGroupManager().LeaveGroup(data.param2);
			GetRPCManager().SendRPC("PartyMe", "GroupDestroyed", NULL, false, toKickIdentity);
		}
	}
};
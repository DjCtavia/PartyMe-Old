class PM_RPC_C_group
{
	void PM_RPC_C_group()
	{
		GetRPCManager().AddRPC("PartyMe", "SetMember", this, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("PartyMe", "PlayerJoinGroup", this, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("PartyMe", "PlayerLeaveGroup", this, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("PartyMe", "GroupDestroyed", this, SingleplayerExecutionType.Both);
	}

	void SetMember(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		PM_GetGroup().isLeader = false;
	}

	/*
		Description:
			When player is joining a group, we need at least
			4 parameters to initialize it
		Data:
			Param1: Player ID
			Param2: Player name
			Param3: Player position
			Param4: Player health
		Event:
			playerIdFrom: ID of player joining group
			name: Name of player joining group
			position: Position of player joining group
			health: Health of player joining GROUP_LIQUID_ALL
	*/
	void PlayerJoinGroup(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		Param4<string, string, vector, float> data;
		if (!ctx.Read(data)) return;

		ref PM_Event_Params eventParams = new PM_Event_Params;
		eventParams.playerIdFrom = data.param1;
		eventParams.name = data.param2;
		eventParams.position = data.param3;
		eventParams.health = data.param4;
		PM_GetEvents().CallEvent("PlayerJoinGroup", eventParams);
    }

	void PlayerLeaveGroup(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		Param1<string> data;
		if (!ctx.Read(data)) return;
		ref PM_Event_Params eventParams = new PM_Event_Params;
		eventParams.playerIdFrom = data.param1;

		PM_GetEvents().CallEvent("PlayerLeaveGroup", eventParams);
	}

	void GroupDestroyed(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		PM_GetEvents().CallEvent("GroupDestroyed", NULL);
	}
};
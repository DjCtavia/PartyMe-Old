class PM_RPC_S_invites
{
    void PM_RPC_S_invites()
    {
        GetRPCManager().AddRPC("PartyMe", "InvitationReceivedd", this, SingleplayerExecutionType.Both);
        // GetRPCManager().AddRPC("PartyMe", "InvitationResponse", this, SingleplayerExecutionType.Both);
    }

    /*
        Data:
            Param1: ID of the inviting player
        Event:
            playerIdFrom: ID of the inviting player
    */
    void InvitationReceivedd(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        ref PM_Event_Params eventParams = new PM_Event_Params;
        eventParams.playerIdFrom = data.param1;
		eventParams.name = data.param2;
        PM_GetEvents().CallEvent("InvitationReceived", eventParams);
    }

    /*
        Data:
            Param1: ID of the player accepting the invitation
        Event:
            playerIdFrom: ID of the player accepting the invitation
			answer: Has accept invitation or no
    */
    // void InvitationResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    // {
    //     Param2<string, bool> data;
    //     if (!ctx.Read(data)) return;
	// 	ref PM_Event_Params eventParams = new PM_Event_Params;
        
	// 	eventParams.playerIdFrom = data.param1;
	// 	eventParams.answer = data.param2;
	// 	SendInvitationResponseToClient(sender, data.param1, data.param2);
	// 	PM_GetEvents().CallEvent("InvitationResponse", eventParams);
    // }

	// private void SendInvitationResponseToClient(ref PlayerIdentity sender, string receiverId, bool response)
	// {
	// 	ref PM_Group_Manager groupManager = PM_GetGroupManager();
	// 	ref PM_S_settings settings = GetServerSettings();
	// 	int groupSize = groupManager.GetGroupSize(receiverId);
	// 	bool isLeader = groupManager.IsLeader(receiverId);
	// 	PlayerIdentity receiverIdentity = missionServer.GetPlayerIdentity(receiverId);
		
	// 	if (receiverIdentity)
	// 	{
	// 		auto params = new Param2<string, bool>(sender.GetId(), response);
	// 		if (response && isLeader && !settings.group.IsGroupFull(groupSize))
	// 		{
	// 			GetRPCManager().SendRPC("PartyMe", "GroupSettings", params, false, receiverIdentity);
	// 		}
	// 		else
	// 		{
	// 			params.param2 = false;
	// 			GetRPCManager().SendRPC("PartyMe", "GroupSettings", params, false, receiverIdentity);
	// 		}
	// 	}
	// }
};
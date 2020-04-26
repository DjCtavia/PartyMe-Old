/**
 * Main element for getting positions of group members
 * 
 * Note:
 *      The name of class could be edited later if other informations could be
 *      gatehered from client side.
*/
class PM_Group_Position_Updater
{
    private ScriptCallQueue timer;

    void PM_Group_Position_Updater()
    {
        timer = GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM);
        timer.CallLater(Update, 0, true);
        AddRPCs();
    }

    void AddRPCs()
    {
        GetRPCManager().AddRPC("PartyMe", "UpdatePlayerPositionNetwork", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "ResetPlayerEntity", this, SingleplayerExecutionType.Both);
    }

    void Update()
    {
        ref PM_Group group = PM_GetGroup();
        ref Man playerSelected;

        IsLocalPlayerMissing(group);
        for (int playerIndex = 0; playerIndex < ClientData.m_PlayerBaseList.Count(); playerIndex++)
        {
            playerSelected = ClientData.m_PlayerBaseList.Get(playerIndex);
            ref PlayerIdentity pIdentity;
            if (playerSelected && playerSelected.IsAlive())
            {
                pIdentity = playerSelected.GetIdentity();
                FindLocalPlayer(playerSelected, pIdentity, group);
                UpdatePlayerPositionLocally(playerSelected, pIdentity, group);
            }
        }
    }

    // Get player entity (class Man) for updating position later locally
    void FindLocalPlayer(ref Man playerSelected, ref PlayerIdentity pIdentity, ref PM_Group group)
    {
        string playerId = "";
        if (!pIdentity || !PM_GetPlayerId(playerId)) return;
        ref PM_Player_Infos_t pInfos = group.players.Get(pIdentity.GetId());

        if (pInfos && !pInfos.isLocal)
        {
            // Tell server no more update is required [RPC]
            GetRPCManager().SendRPC("PartyMe", "RemovePlayerUpdatePosition", new Param2<string, string>(playerId, pInfos.id));
            Print("[PM] Local player " + pInfos.id + " has been found!");
            pInfos.localPlayer = playerSelected;
            pInfos.isLocal = true;
        }
    }

    // Update player position locally
    void UpdatePlayerPositionLocally(ref Man playerSelected, ref PlayerIdentity pIdentity, ref PM_Group group)
    {
        if (!pIdentity) return;
        ref PM_Player_Infos_t pInfos = group.players.Get(pIdentity.GetId());

        if (pInfos)
        {
            MiscGameplayFunctions.GetHeadBonePos(PlayerBase.Cast(playerSelected), pInfos.position);
            pInfos.position += "0 0.3 0";
        }
    }

    // Update player position over network
    void UpdatePlayerPositionNetwork(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<string, vector> data;
        if (!ctx.Read(data)) return;

        ref PM_Group group = PM_GetGroup();
        ref PM_Player_Infos_t pInfos = group.players.Get(data.param1);

        if (pInfos)
        {
            pInfos.position = data.param2;
        }
    }

    // Verify if Man entity is reachable locally
    void IsLocalPlayerMissing(ref PM_Group group)
    {
        string playerId = "";
        if (!group || !PM_GetPlayerId(playerId)) return;

        foreach (ref PM_Player_Infos_t pInfos : group.players.GetValueArray())
        {
            if (pInfos && pInfos.isLocal && !pInfos.localPlayer)
            {
                // Ask server update [RPC]
                GetRPCManager().SendRPC("PartyMe", "AddPlayerUpdatePosition", new Param2<string, string>(playerId, pInfos.id));
                Print("[PM] Local player " + pInfos.id + " is missing...");
                pInfos.isLocal = false;
            }
        }
    }

    /*
        When a player died, force to reset the player entity of party member.
        Entity is not defined as null even after respawn, it could cause issue.
    */
    void ResetPlayerEntity(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param1<string> data;
        if (!ctx.Read(data)) return;

        ref PM_Group group = PM_GetGroup();
        ref PM_Player_Infos_t pInfos = group.players.Get(data.param1);

        if (pInfos)
        {
            pInfos.localPlayer = null;
            pInfos.isLocal = false;
        }
        Print("[PM] Player died, localPlayer variable has been reset to NULL. Dead player id: " + data.param1);
    }
};

static ref PM_Group_Position_Updater g_PM_group_position_updater;
static ref PM_Group_Position_Updater PM_GetGroupPositionUpdater()
{
    if (!g_PM_group_position_updater)
    {
        g_PM_group_position_updater = new PM_Group_Position_Updater();
    }
    return g_PM_group_position_updater;
}
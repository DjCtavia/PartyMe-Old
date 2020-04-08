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
    }

    void Update()
    {
        ref PM_Group group = PM_GetGroup();
        ref Man playerSelected;

        for (int playerIndex = 0; playerIndex < ClientData.m_PlayerBaseList.Count(); playerIndex++)
        {
            playerSelected = ClientData.m_PlayerBaseList.Get(playerIndex);
            ref PlayerIdentity pIdentity;
            if (playerSelected)
            {
                pIdentity = playerSelected.GetIdentity();
                UpdatePlayerInfos(playerSelected, pIdentity, group);
            }
        }
    }

    void UpdatePlayerInfos(ref Man playerSelected, ref PlayerIdentity pIdentity, ref PM_Group group)
    {
        if (!pIdentity) return;
        ref PM_Player_Infos_t pInfos = group.players.Get(pIdentity.GetId());

        if (pInfos)
        {
            MiscGameplayFunctions.GetHeadBonePos(playerSelected, pInfos.position);
            pInfos.position += "0 0.3 0";
            pInfos.w_marker.UpdateFromPlayerInfoStruct(pInfos);
        }
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
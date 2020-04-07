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
        Man playerSelected;

        for (int playerIndex = 0; playerIndex < ClientData.m_PlayerBaseList.Count(); playerIndex++)
        {
            playerSelected = ClientData.m_PlayerBaseList.Get(playerIndex);
            PlayerIdentity pIdentity;
            if (playerSelected)
            {
                pIdentity = playerSelected.GetIdentity();
                UpdatePlayerInfos(playerSelected, pIdentity);
            }
        }
    }

    void UpdatePlayerInfos(Man playerSelected, PlayerIdentity pIdentity)
    {
        if (!pIdentity) return;
        ref PM_Players_Infos_t pInfos = group.Get(pIdentity.GetId());

        if (pInfos)
        {
            pInfos.position = playerSelected.GetPosition();
        }
    }
};
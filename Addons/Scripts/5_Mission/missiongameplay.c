/**
 * Second account uid: FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs= 
*/
modded class MissionGameplay
{
    ref PM_GroupMenu                m_groupMenu;

    void MissionGameplay()
    {
        InitPartyMe();
    }

    void InitPartyMe()
    {
        PM_GetGroupConstraint();
        PM_GetGroup();
        PM_GetGroupPositionUpdater();
        PM_GetMarkerUpdater();

        PM_GetGroup().AddPlayer("FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs=", "Bobby", "0 0 0", 100);
    }

    override void OnUpdate(float timeslice)
	{
        super.OnUpdate(timeslice);
        UIScriptedMenu menu = m_UIManager.GetMenu();
        PM_GroupMenu groupMenu = PM_GroupMenu.Cast(m_UIManager.FindMenu(MENU_GROUP));
        Input input = GetGame().GetInput();

        if (input.LocalPress("OpenPartyMeMenu", false))
        {
            if(!groupMenu)
            {
                ShowGroupMenu();
                menu = groupMenu;
            }
            else if (menu == groupMenu)
            {
                HideGroupMenu();
            }
        }
    }

    void InitGroupMenu()
    {
        if (!m_groupMenu)
        {
            m_groupMenu = PM_GroupMenu.Cast(GetUIManager().EnterScriptedMenu(MENU_GROUP, NULL));
            GetUIManager().HideScriptedMenu(m_groupMenu);
        }
    }

    void ShowGroupMenu()
    {
        UIScriptedMenu menu = GetUIManager().GetMenu();

        if (!menu)
        {
            if (!m_groupMenu)
            {
                InitGroupMenu();
            }

            if(!GetUIManager().FindMenu(MENU_GROUP))
			{
				GetUIManager().ShowScriptedMenu(m_groupMenu, null);
			}
            // A verifier
            MoveHudForInventory(true);
			PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);
        }
    }

    void HideGroupMenu()
    {
        if (m_groupMenu)
        {
            GetUIManager().HideScriptedMenu(m_groupMenu);
			MoveHudForInventory(false);
			PlayerControlEnable(false);
        }
    }

    void DestroyGroupMenu()
    {
        if(m_groupMenu)
		{
			m_groupMenu.Close();
			m_groupMenu = NULL;
		}
    }
};
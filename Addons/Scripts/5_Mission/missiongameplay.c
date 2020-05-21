/**
 * Second account uid: FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs= 
*/
modded class MissionGameplay
{
    ref PM_GroupMenu                    m_pm_groupMenu;
	ref PM_RPC_C_handler                m_pm_rpc_handler;
	ref PM_Settings_Group				m_pm_settings_group;
	ref PM_C_Invitations				m_pm_invitations;

    void MissionGameplay()
    {
        InitPartyMe();
    }

    void InitPartyMe()
    {
        m_pm_rpc_handler = new PM_RPC_C_handler;
		m_pm_settings_group = new PM_Settings_Group;
		m_pm_invitations = new PM_C_Invitations;
        PM_GetGroupConstraint();
        PM_GetGroup();
        PM_GetGroupPositionUpdater();
        PM_GetMarkerUpdater();

        // PM_GetGroup().AddPlayer("FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs=", "Bobby", "0 0 0", 100);
    }

    //--------------------------------------------------------------------------
    override void OnUpdate(float timeslice)
	{
        super.OnUpdate(timeslice);
        UIScriptedMenu menu = m_UIManager.GetMenu();
        PM_GroupMenu groupMenu = PM_GroupMenu.Cast(m_UIManager.FindMenu(MENU_GROUP));
        Input input = GetGame().GetInput();

        if (input.LocalPress("OpenPartyMeMenu", false))
        {
            if (!groupMenu)
            {
                ShowGroupMenu();
                menu = groupMenu;
            }
            else if (menu == groupMenu)
            {
                HideGroupMenu();
            }
        }
        if(input.LocalPress("UAUIBack",false))
        {
            if(menu == groupMenu)
            {
                HideGroupMenu();
            }
        }
    }

    void InitGroupMenu()
    {
        if (!m_pm_groupMenu)
        {
            m_pm_groupMenu = PM_GroupMenu.Cast(GetUIManager().EnterScriptedMenu(MENU_GROUP, NULL));
            GetUIManager().HideScriptedMenu(m_pm_groupMenu);
        }
    }

    void ShowGroupMenu()
    {
        UIScriptedMenu menu = GetUIManager().GetMenu();

        if (!menu)
        {
            if (!m_pm_groupMenu)
            {
                InitGroupMenu();
            }

            if(!GetUIManager().FindMenu(MENU_GROUP))
            {
                GetUIManager().ShowScriptedMenu(m_pm_groupMenu, null);
            }
            // A verifier
            MoveHudForInventory(true);
            PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);
        }
    }

    void HideGroupMenu()
    {
        if (m_pm_groupMenu)
        {
            GetUIManager().HideScriptedMenu(m_pm_groupMenu);
            MoveHudForInventory(false);
            PlayerControlEnable(false);
        }
    }

    void DestroyGroupMenu()
    {
        if(m_pm_groupMenu)
        {
            m_pm_groupMenu.Close();
            m_pm_groupMenu = NULL;
        }
    }
};
class PM_UI_Option_Party extends PM_UI_Category
{
    void PM_UI_Option_Party(Widget parent, int index)
    {
        m_icon = "partyme/gui/images/options/icons/party.tga";
        m_w_parent = parent;
        Init(index);
    }

    override void GetWidgets()
    {
        super.GetWidgets();

        m_menu = new PM_UI_Menu_Party(m_w_parent);
    }

    override void ConfigureWidget()
    {
        super.ConfigureWidget();
        m_txt_optionName.SetText("Party");
    }
	
	//-------------------------------------------------------------------------- UI Events
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (super.OnClick(w, x, y, button) || m_menu.OnClick(w, x, y, button))
		{
			return true;
		}
		return false;
	}
};

class PM_UI_Menu_Party extends PM_UI_Menu
{
    protected ScrollWidget                          	m_scroll_playerList;
    protected ButtonWidget                          	m_btn_leaveParty;

    protected ref array<ref PM_UI_party_PlayerWidget>	m_players;

    void PM_UI_Menu_Party(Widget parent)
    {
        m_layoutPath = "partyme/gui/layouts/submenus/group/options/party/option.layout";
        m_w_parent = parent;
        m_players = new array<ref PM_UI_party_PlayerWidget>;
        Init();
		InitPlayersGroup();
        PM_GetEvents().AddEvent("PlayerJoinGroup", this);
		PM_GetEvents().AddEvent("PlayerLeaveGroup", this);
		PM_GetEvents().AddEvent("GroupDestroyed", this);
    }

	/*
		Note:
			This method is only useful in specific cases. In standard use, it is of no use at all.
			A player needs to open the menu at least once to accept an invitation, so initializing
			this method is only useful for a permanent group system, for example.
	*/
	void InitPlayersGroup()
	{
		array<PM_Player_Infos_t> members = PM_GetGroup().GetPlayers();

		for (int iMember = 0; iMember < members.Count(); iMember++)
		{
			auto member = members.Get(iMember);
			
			if (member)
			{
				AddPlayer(member.id, member.name);
			}
		}
	}

    override void GetWidgets()
    {
        super.GetWidgets();
        m_scroll_playerList = ScrollWidget.Cast(m_w_root.FindAnyWidget("playerList"));
        m_btn_leaveParty = ButtonWidget.Cast(m_w_root.FindAnyWidget("leaveParty"));
    }

    void SetWidgetPlayerPosition(int index)
    {

    }

    int FindPlayer(string playerId)
    {
        for (int index = 0; index < m_players.Count(); index++)
        {
            ref PM_UI_party_PlayerWidget playerWidget = m_players.Get(index);
            if (playerWidget && playerWidget.GetPlayerId() == playerId)
            {
                return index;
            }
        }
        return -1;
    }

    void AddPlayer(string playerId, string playerName)
    {
        if (FindPlayer(playerId) == -1)
        {
			auto playerWidget = new PM_UI_party_PlayerWidget(m_w_parent, m_scroll_playerList, playerId, playerName);
            int pos = m_players.Insert(playerWidget);
            playerWidget.SetPosition(pos);
        }
    }

    void RemovePlayer(string playerId)
    {
        int indexPlayer = FindPlayer(playerId);

        if (indexPlayer != -1)
        {
            m_players.Remove(indexPlayer);
            UpdateWidgetsPosition();
        }
    }

    void UpdateWidgetsPosition()
    {
		for (int indexWidget = 0; indexWidget < m_players.Count(); indexWidget++)
        {
			auto playerWidget = m_players.Get(indexWidget);
			playerWidget.SetPosition(indexWidget);
        }
    }

    //-------------------------------------------------------------------------- Events
    void OnPlayerJoinGroup(PM_Event_Params eventParams)
    {
        AddPlayer(eventParams.playerIdFrom, eventParams.name);
    }

    void OnPlayerLeaveGroup(PM_Event_Params eventParams)
    {
        RemovePlayer(eventParams.playerIdFrom);
    }

	void OnGroupDestroyed(PM_Event_Params eventParams)
	{
		m_players.Clear();
	}

	//-------------------------------------------------------------------------- UI Events
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (LeaveGroup(w)) return true;
		for (int iWidget = 0; iWidget < m_players.Count(); iWidget++)
		{
			ref PM_UI_party_PlayerWidget widget = m_players.Get(iWidget);
			
			if (widget && widget.OnClick(w, x, y, button))
			{
				return true;
			}
		}
		return false;
	}

    //-------------------------------------------------------------------------- UI Events Actions
    bool LeaveGroup(Widget w)
    {
        if (w != m_btn_leaveParty)
            return false;
        PM_GetGroup().LeaveGroup();
        m_players.Clear();
        GetRPCManager().SendRPC("PartyMe", "PlayerLeaveGroup");
        return true;
    }
};

class PM_UI_party_PlayerWidget
{
    private static const string DEFAULT_LAYOUT          = "partyme/gui/layouts/submenus/group/options/party/widgets/player.layout";
    private static const string ICON_MORE_OPTIONS       = "partyme/gui/images/options/icons/arrow.tga";
    private static const string ICON_SHOWHIDE_MARKER    = "partyme/gui/images/options/icons/eye.tga";
    Widget                                              m_w_parent;
    Widget                                              m_w_root;
    TextWidget                                          m_txt_playerName;
    ButtonWidget                                        m_btn_SHMarker;
    ButtonWidget                                        m_btn_leadOptions;
    ImageWidget                                         m_img_SHMarker;
    ImageWidget                                         m_img_leadOptions;

    string                                              m_playerId;
	string                                              m_playerName;

    void PM_UI_party_PlayerWidget(Widget master, Widget parent, string playerId, string playerName)
    {
        m_playerId = playerId;
		m_playerName = playerName;
		m_w_parent = master;
        m_w_root = GetGame().GetWorkspace().CreateWidgets(DEFAULT_LAYOUT, parent);
        GetWidgets();
        InitIcons();
        RetrievePlayerInfos();
    }

    void ~PM_UI_party_PlayerWidget()
    {
        m_w_root.Unlink();
    }

    void GetWidgets()
    {
        m_txt_playerName = TextWidget.Cast(m_w_root.FindAnyWidget("playerName"));
        m_btn_SHMarker = ButtonWidget.Cast(m_w_root.FindAnyWidget("SHMarker"));
        m_btn_leadOptions = ButtonWidget.Cast(m_w_root.FindAnyWidget("leadOptions"));
        m_img_SHMarker = ImageWidget.Cast(m_w_root.FindAnyWidget("SHMarkerImage"));
        m_img_leadOptions = ImageWidget.Cast(m_w_root.FindAnyWidget("leadOptionsImage"));
    }

    void InitIcons()
    {
        m_img_SHMarker.LoadImageFile(0, ICON_SHOWHIDE_MARKER);
        m_img_leadOptions.LoadImageFile(0, ICON_MORE_OPTIONS);
    }
	
	void SetPosition(int index)
    {
        m_w_root.SetPos(50, (40 + 65 * index));
    }

    //--------------------------------------------------------------------------
    void RetrievePlayerInfos()
    {
        string playerName;

        if (PM_GetPlayerUtilities().GetPlayerName(m_playerId, playerName))
        {
            m_txt_playerName.SetText(playerName);
        }
        else
        {
            m_txt_playerName.SetText("Unknow name");
        }
    }

    //-------------------------------------------------------------------------- Getters
    string GetPlayerId()
    {
        return m_playerId;
    }
	
	string GetPlayerName()
    {
        return m_playerName;
    }

    //-------------------------------------------------------------------------- UI Events
    bool OnClick(Widget w, int x, int y, int button)
    {
        if (button == MouseState.LEFT)
        {
			if (OpenLeadOptions(w)) return true;
			if (ToggleMarker(w)) return true;
        }
       return false;
    }
    //-------------------------------------------------------------------------- UI Events Functions
    bool OpenLeadOptions(Widget w)
    {
		if (w != m_btn_leadOptions)
			return false;
		auto params = new Param2<string, string>(PM_GetPlayerId(), m_playerId);
		// Kick for the moment, need to edit later
		GetRPCManager().SendRPC("PartyMe", "PlayerKickGroup", params);
        Print("[PartyMe] Trying to kick: " + m_playerId);
		return true;
    }

	bool ToggleMarker(Widget w)
	{
		if (w != m_btn_SHMarker)
			return false;
		ref PM_Player_Infos_t pInfos = PM_GetGroup().players.Get(m_playerId);
		
		if (!pInfos)
			return true;
		pInfos.w_marker.SetVisible(!pInfos.w_marker.IsVisible());
		return true;
	}
};

class PM_UI_LeadOptions
{

};
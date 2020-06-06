class PM_UI_Option_Playerlist extends PM_UI_Category
{
    void PM_UI_Option_Playerlist(Widget parent, int index)
    {
        m_icon = "partyme/gui/images/options/icons/players.tga";
        m_w_parent = parent;
        Init(index);
    }

    override void GetWidgets()
    {
        super.GetWidgets();

        m_menu = new PM_UI_Menu_Playerlist(m_w_parent);
    }

    override void ConfigureWidget()
    {
        super.ConfigureWidget();
        m_txt_optionName.SetText("Players");
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

class PM_UI_Menu_Playerlist extends PM_UI_Menu
{
    protected ScrollWidget                                  m_scroll_playerList;
    protected ref array<ref PM_UI_playerlist_PlayerWidget>  m_players;

    void PM_UI_Menu_Playerlist(Widget parent)
    {
        m_layoutPath = "partyme/gui/layouts/submenus/playerlist/option.layout";
        m_w_parent = parent;
        m_players = new array<ref PM_UI_playerlist_PlayerWidget>;
        Init();
		InitPlayerList();
        PM_GetEvents().AddEvent("PlayerJoinServer", this);
		PM_GetEvents().AddEvent("PlayerLeaveServer", this);
    }

    override void GetWidgets()
    {
        super.GetWidgets();
        m_scroll_playerList = ScrollWidget.Cast(m_w_root.FindAnyWidget("playerList"));
    }

	void InitPlayerList()
	{
		string ownPlayerId;
		ref array<PlayerIdentity> players = new array<PlayerIdentity>;

		GetGame().GetPlayerIndentities(players);	
		for (int i = 0; i < players.Count(); i++)
		{
			PlayerIdentity playerIdentity = players[i];

			if (playerIdentity)
			{	
				string playerId = playerIdentity.GetId();		
				string playerName = playerIdentity.GetName();

				AddPlayer(playerId, playerName);
			}
		}
	}

    //--------------------------------------------------------------------------
    int FindPlayer(string playerId)
    {
        for (int index = 0; index < m_players.Count(); index++)
        {
            ref PM_UI_playerlist_PlayerWidget playerWidget = m_players.Get(index);
            if (playerWidget && playerWidget.GetPlayerId() == playerId)
            {
                return index;
            }
        }
        return -1;
    }

    void AddPlayer(string playerId, string playerName)
    {
		string ownPlayerId;

        if (FindPlayer(playerId) == -1 && PM_GetPlayerId(ownPlayerId) && playerId != ownPlayerId)
        {
            auto playerWidget = new PM_UI_playerlist_PlayerWidget(m_scroll_playerList, playerId, playerName);
            int pos = m_players.Insert(playerWidget);
            playerWidget.SetPosition(pos);
        }
    }

     void RemovePlayer(string playerId)
    {
        int indexPlayer = FindPlayer(playerId);

        if (indexPlayer != -1)
        {
			m_players.Get(indexPlayer).Destroy();
            m_players.Remove(indexPlayer);
            UpdateWidgetsPosition();
        }
    }

    void AddMissingPlayers(array<Param2<string, string>> players)
    {
        foreach (auto player : players)
        {
            AddPlayer(player.param1, player.param2);
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
    void OnPlayerJoinServer(ref PM_Event_Params eventParams)
    {
		AddPlayer(eventParams.playerIdFrom, eventParams.name);
    }
	
	void OnPlayerLeaveServer(ref PM_Event_Params eventParams)
    {
		RemovePlayer(eventParams.playerIdFrom);
    }
	
	//-------------------------------------------------------------------------- UI Events
	override bool OnClick(Widget w, int x, int y, int button)
	{
		for (int iWidget = 0; iWidget < m_players.Count(); iWidget++)
		{
			ref PM_UI_playerlist_PlayerWidget widget = m_players.Get(iWidget);
			
			if (widget && widget.OnClick(w, x, y, button))
			{
				return true;
			}
		}
		return false;
	}
};

class PM_UI_playerlist_PlayerWidget
{
    private static const int INVITE_DELAY                   = 2000;
    private static const string DEFAULT_LAYOUT              = "partyme/gui/layouts/submenus/playerlist/widgets/player.layout";
    private static const string ICON_INVITE                 = "partyme/gui/images/options/icons/add.tga";

    private Widget                                          m_w_root;
    private TextWidget                                      m_txt_playerName;
    ref PM_widget_smallbutton							    m_b_invite;

    private string                                          m_playerId;
    private string                                          m_playerName;
    private bool                                            m_canInvite;

    void PM_UI_playerlist_PlayerWidget(Widget parent, string playerId, string playerName)
    {
        m_playerId = playerId;
        m_playerName = playerName;
        m_canInvite = true;
        m_w_root = GetGame().GetWorkspace().CreateWidgets(DEFAULT_LAYOUT, parent);
        GetWidgets();
        ConfigWidgets();
    }

    private void GetWidgets()
    {
        m_b_invite = new PM_widget_smallbutton(m_w_root, ICON_INVITE, 1052689, 1, 16777215, 1);
        m_b_invite.SetPos(574, 6);
        m_b_invite.SetBackgroundDisable(1052689, 0.5);
		m_b_invite.SetIconDisable(3750202, 1);
        m_txt_playerName = TextWidget.Cast(m_w_root.FindAnyWidget("playerName"));
    }

    private void ConfigWidgets()
    {
        m_txt_playerName.SetText(m_playerName);
    }

    void SetPosition(int index)
    {
        m_w_root.SetPos(34, (32 + 72 * index));
    }

    private void EnableInviteButton(bool isEnable)
    {
        m_b_invite.Enable(isEnable);
    }
	
	void Destroy()
	{
		m_w_root.Unlink();
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
            if (m_b_invite.OnClick(w, x, y, button) && m_canInvite && PM_GetGroup().CanInvite())
            {
                Invite();
                return true;
            }
        }
        return false;
    }

    //-------------------------------------------------------------------------- UI Events Functions
    private void Invite()
    {
		string playerName;

        EnableInviteButton(false);
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(EnableInviteButton, INVITE_DELAY, false, true);
		if (PM_GetPlayerName(playerName))
			GetRPCManager().SendRPC("PartyMe", "InvitationReceived", new Param2<string, string>(m_playerId, playerName));
    }
};
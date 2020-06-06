class PM_UI_Option_Invitations extends PM_UI_Category
{
    void PM_UI_Option_Invitations(Widget parent, int index)
    {
        m_icon = "partyme/gui/images/options/icons/invitations.tga";
        m_w_parent = parent;
        Init(index);
    }

    override void GetWidgets()
    {
        super.GetWidgets();

        m_menu = new PM_UI_Menu_Invitations(m_w_parent);
    }

    override void ConfigureWidget()
    {
        super.ConfigureWidget();
        m_txt_optionName.SetText("Invitations");
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

class PM_UI_Menu_Invitations extends PM_UI_Menu
{
    protected ScrollWidget                                  m_scroll_playerList;
    protected ref array<ref PM_UI_invitations_PlayerWidget> m_players;

    void PM_UI_Menu_Invitations(Widget parent)
    {
        m_layoutPath = "partyme/gui/layouts/submenus/invitations/options/option.layout";
        m_w_parent = parent;
        m_players = new array<ref PM_UI_invitations_PlayerWidget>;
        Init();
		InitInvitations();
		AddEvents();
    }
	
	void InitInvitations()
	{
		ref PM_C_Invitations invitations = MissionGameplay.Cast(GetGame().GetMission()).m_pm_invitations;
		string playerId;

		if (invitations && PM_GetPlayerId(playerId))
		{
			ref array<string> playersInvitations = invitations.m_invitations.Get(playerId);

			if (playersInvitations)
			{
				for (int iPlayer = 0; iPlayer < playersInvitations.Count(); iPlayer++)
				{
					string pId = playersInvitations.Get(iPlayer);
					string pName;
					
					if (PM_GetPlayerUtilities().GetPlayerName(pId, pName))
						AddPlayer(pId, pName);
					else
						AddPlayer(pId, "Unknown name");
				}
			}
		}
	}

	void AddEvents()
	{
		PM_GetEvents().AddEvent("InvitationReceivedUI", this);
	}

    override void GetWidgets()
    {
        super.GetWidgets();
        m_scroll_playerList = ScrollWidget.Cast(m_w_root.FindAnyWidget("playerList"));
    }

    //-------------------------------------------------------------------------- Utils
    int FindPlayer(string playerId)
    {
        for (int index = 0; index < m_players.Count(); index++)
        {
            ref PM_UI_invitations_PlayerWidget playerWidget = m_players.Get(index);
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
            auto playerWidget = new PM_UI_invitations_PlayerWidget(m_scroll_playerList, playerId, playerName);
            int pos = m_players.Count();
            m_players.Insert(playerWidget);
            playerWidget.SetPosition(pos);
        }
    }

    void RemovePlayer(string playerId)
    {
        int indexPlayer = FindPlayer(playerId);

        if (indexPlayer > -1)
        {
            m_players.Remove(indexPlayer);
            UpdateWidgetsPosition();
        }
    }
	
	void RemovePlayerByWidgetReference(ref PM_UI_invitations_PlayerWidget widget)
    {
		int remove_index = m_players.Find(widget);

		if (remove_index > -1)
		{
			m_players.RemoveOrdered(remove_index);
			UpdateWidgetsPosition();
		}
    }

    //-------------------------------------------------------------------------- Widget Positioning
    void UpdateWidgetsPosition()
    {
		for (int indexWidget = 0; indexWidget < m_players.Count(); indexWidget++)
        {
			auto playerWidget = m_players.Get(indexWidget);
			playerWidget.SetPosition(indexWidget);
        }
    }
	//-------------------------------------------------------------------------- Events
    void OnInvitationReceivedUI(ref PM_Event_Params eventParams)
    {
		string playerId = eventParams.playerIdFrom;
		string name = eventParams.name;

		AddPlayer(playerId, name);
	}
	
	//-------------------------------------------------------------------------- UI Events
	override bool OnClick(Widget w, int x, int y, int button)
	{
		for (int iWidget = 0; iWidget < m_players.Count(); iWidget++)
		{
			ref PM_UI_invitations_PlayerWidget widget = m_players.Get(iWidget);
			
			if (widget && widget.OnClick(w, x, y, button))
			{
				RemovePlayerByWidgetReference(widget);
				return true;
			}
		}
		return false;
	}
};

class PM_UI_invitations_PlayerWidget
{
    private static const string DEFAULT_LAYOUT              = "partyme/gui/layouts/submenus/invitations/options/widgets/player.layout";
    private static const string ICON_ACCEPT                 = "partyme/gui/images/options/icons/accept.tga";
    private static const string ICON_DECLINE                = "partyme/gui/images/options/icons/decline.tga";

    private Widget                                          m_w_root;
    private TextWidget                                      m_txt_playerName;
    private ButtonWidget                                    m_btn_accept;
    private ButtonWidget                                    m_btn_decline;
    private ImageWidget                                     m_img_accept;
    private ImageWidget                                     m_img_decline;

    private string                                          m_playerId;
    private string                                          m_playerName;

    void PM_UI_invitations_PlayerWidget(Widget parent, string playerId, string playerName)
    {
        m_playerId = playerId;
        m_playerName = playerName;
        m_w_root = GetGame().GetWorkspace().CreateWidgets(DEFAULT_LAYOUT, parent);
        GetWidgets();
        ConfigWidgets();
    }
	
	void ~PM_UI_invitations_PlayerWidget()
	{
		if (m_w_root)
			m_w_root.Unlink();
	}

    private void GetWidgets()
    {
        m_txt_playerName = TextWidget.Cast(m_w_root.FindAnyWidget("playerName"));
        m_btn_accept = ButtonWidget.Cast(m_w_root.FindAnyWidget("accept"));
        m_btn_decline = ButtonWidget.Cast(m_w_root.FindAnyWidget("decline"));
        m_img_accept = ImageWidget.Cast(m_w_root.FindAnyWidget("acceptLogo"));
        m_img_decline = ImageWidget.Cast(m_w_root.FindAnyWidget("declineLogo"));
    }

    private void ConfigWidgets()
    {
        m_txt_playerName.SetText(m_playerName);
        InitIcons();
    }

    private void InitIcons()
    {
        m_img_accept.LoadImageFile(0, ICON_ACCEPT);
        m_img_decline.LoadImageFile(0, ICON_DECLINE);
    }

    void SetPosition(int index)
    {
        m_w_root.SetPos(50, (40 + 65 * index));
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
			if (w == m_btn_accept)
			{
				InvitationResponse(true);
				return true;
			}
			if (w == m_btn_decline)
			{
				InvitationResponse(false);
				return true;
			}
		}
        return false;
    }

	//-------------------------------------------------------------------------- UI Events Functions
	void InvitationResponse(bool response)
	{
		MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
		auto params = new Param2<string, bool>(m_playerId, response);
		string playerId;

		Print("[PartyMe][UI]PlayerInvite widget sending RPC");
		GetRPCManager().SendRPC("PartyMe", "InvitationResponse", params);
		if (mission && PM_GetPlayerId(playerId))
			mission.m_pm_invitations.RemovePlayerInvite(m_playerId, playerId);
	}
};
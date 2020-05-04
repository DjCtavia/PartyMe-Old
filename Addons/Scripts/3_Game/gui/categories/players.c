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
};

class PM_UI_Menu_Party extends PM_UI_Menu
{
    protected ScrollWidget                          m_scroll_playerList;
    protected ButtonWidget                          m_btn_leaveParty;

    protected ref array<ref PM_UI_party_PlayerWidget>     m_players;

    void PM_UI_Menu_Party(Widget parent)
    {
        m_layoutPath = "partyme/gui/layouts/submenus/group/options/party/option.layout";
        m_w_parent = parent;
        m_players = new array<ref PM_UI_party_PlayerWidget>;
        Init();
        RetrievePlayersFromGroup()
        PM_GetEvents().AddEvent("PlayerJoinGroup", this);
    }

    override void GetWidgets()
    {
        super.GetWidgets();
        m_scroll_playerList = ScrollWidget.Cast(m_w_root.FindAnyWidget("playerList"));
        m_btn_leaveParty = ButtonWidget.Cast(m_w_root.FindAnyWidget("leaveParty"));
    }

    void RetrievePlayersFromGroup()
    {
        array<string> playerIDs = PM_GetGroup().players.GetKeyArray();

        if (playerIDs)
        {
            foreach (string playerId : playerIDs)
            {
                AddPlayer(playerId);
            }
        }
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

    void AddPlayer(string playerId)
    {
        if (FindPlayer(playerId) == -1)
        {
            int pos = m_players.Count();
            m_players.Insert(new PM_UI_party_PlayerWidget(m_w_parent, m_scroll_playerList, playerId, pos));
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

    }

    // Events
    void OnPlayerJoinGroup(PM_Event_Params eventParams)
    {
        // Delayed call, the player must first exist in the group
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(AddPlayer, 10, false, eventParams.playerIdFrom);
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

    void PM_UI_party_PlayerWidget(Widget master, Widget parent, string playerId, int arrayPosition)
    {
        m_playerId = playerId;
        m_w_root = GetGame().GetWorkspace().CreateWidgets(DEFAULT_LAYOUT, parent);
        GetWidgets();
        InitIcons();
        RetrievePlayerInfos();
        SetPosition(arrayPosition);
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

    //--------------------------------------------------------------------------
    void RetrievePlayerInfos()
    {
        ref PM_Player_Infos_t playersInfos = PM_GetGroup().players.Get(m_playerId);

        if (playersInfos)
        {
            m_txt_playerName.SetText(playersInfos.name);
        }
        else
        {
            m_txt_playerName.SetText("Player not found");
        }
    }

    //--------------------------------------------------------------------------
    string GetPlayerId()
    {
        return m_playerId;
    }

    //--------------------------------------------------------------------------
    void SetPosition(int index)
    {
        m_w_root.SetPos(50, (40 + 65 * index));
    }

    bool OnClick(Widget w, int x, int y, int button)
    {
        if (button == MouseState.LEFT)
        {
            if (w == m_btn_leadOptions)
            {

            }
        }
       return false;
    }
    //--------------------------------------------------------------------------
    void OpenLeadOptions()
    {

    }
};

class PM_UI_LeadOptions
{

};
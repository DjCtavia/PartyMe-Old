class PM_UI_Option_Playerlist extends PM_UI_Category
{
    void PM_UI_Option_Playerlist(Widget parent, int index)
    {
        m_icon = "partyme/gui/images/options/icons/party.tga";
        m_w_parent = parent;
        Init(index);
    }
};


class PM_UI_Menu_Playerlist extends PM_UI_Menu
{
    protected ScrollWidget                                  m_scroll_playerList;
    protected ref array<ref PM_UI_party_PlayerWidget>       m_players;

    void PM_UI_Menu_Party(Widget parent)
    {
        m_layoutPath = "partyme/gui/layouts/submenus/playerlist/options/option.layout";
        m_w_parent = parent;
        Init();
        PM_GetEvents().AddEvent("PlayerJoinGroup", this);
    }

    override void GetWidgets()
    {
        super.GetWidgets();
        m_scroll_playerList = ScrollWidget.Cast(m_w_root.FindAnyWidget("playerList"));
    }

    // Events
    void OnPlayerJoinServer(ref PM_Event_Params eventParams)
    {

    }
};

class PM_UI_playerlist_PlayerWidget
{

};
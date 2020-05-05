modded class MissionBase
{
    void MissionBase()
    {
        // We init events on client and server side
        PM_GetEvents();
    }

    void ~MissionBase()
    {
        PM_GetEvents().Clear();
    }

    //--------------------------------------------------------------------------
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        UIScriptedMenu menu = NULL;

        menu = super.CreateScriptedMenu(id);
        if (!menu)
        {
            switch (id)
            {
                case MENU_GROUP:
                    menu = new PM_GroupMenu;
                    break;
            }
            if (menu)
            {
                menu.SetID(id);
            }
        }
        return menu;
    }
};
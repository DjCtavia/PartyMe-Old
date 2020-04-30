modded class MissionBase
{
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
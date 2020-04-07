/**
 * Define rules of constraint applied to a group
 * and UI in general. 
*/
class PM_group_constraint
{
    int max_player_number_in_group = 5;
    bool can_use_hud = true;

    void PM_group_constraint()
    {
        GetRPCManager().AddRPC("PartyMe", "UpdateServerConstraint", this, SingleplayerExecutionType.Both); 
    }

    bool IsLimitReach(int numberOfPlayersInGroup)
    {
        numberOfPlayersInGroup += 1;
        if (numberOfPlayersInGroup < max_player_number_in_group)
            return false;
        return true;
    }

    bool CanUseHud()
    {
        return can_use_hud;
    }

    void UpdateServerConstraint(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<int, bool> data;
        if (!ctx.Read(data)) return;

        max_player_number_in_group = data.param1;
        can_use_hud = data.param2;
    }
};

static ref PM_group_constraint g_PM_group_constraint;
static ref PM_group_constraint PM_GetGroupConstraint()
{
    if (!g_PM_group_constraint)
    {
        g_PM_group_constraint = new PM_group_constraint();
    }
    return g_PM_group_constraint;
}
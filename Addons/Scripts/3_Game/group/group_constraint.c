/**
 * Define rules of constraint applied to a group
 * and UI in general. 
*/
class PM_Group_Constraint
{
    int max_player_number_in_group = 5;
    bool can_use_hud = true;

    void PM_Group_Constraint()
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

    // RPC request receive by the server when connecting
    void UpdateServerConstraint(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<int, bool> data;
        if (!ctx.Read(data)) return;

        max_player_number_in_group = data.param1;
        can_use_hud = data.param2;
    }
};

static ref PM_Group_Constraint g_PM_group_constraint;
static ref PM_Group_Constraint PM_GetGroupConstraint()
{
    if (!g_PM_group_constraint)
    {
        g_PM_group_constraint = new PM_Group_Constraint();
    }
    return g_PM_group_constraint;
}
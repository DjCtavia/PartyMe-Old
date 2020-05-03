/*
    Description:
        The purpose of this class is to be temporary is to be created only at the call of an event.
        It allows to facilitate client-side and server-side calling of multiple events requiring to be
        in several places at the same time without having to create multiple RPC functions.
    Note:
        If you want to add a new event with other parameters than the one display below, you should
        mod this class.
*/
class PM_Event_Params
{
    string playerIdFrom;
    string playerIdTo;
    bool answer;
    vector position;
    float health;
};

class PM_events
{
    private ref map<string, ref array<Class>> m_events;

    void PM_events()
    {
        m_events = new map<string, ref array<Class>>;
        Print("[PartyMe][Event] Module loaded.");
    }

    void AddEvent(string eventName, Class instToAdd)
    {
        ref array<Class> events;

        if (!m_events.Contains(eventName))
        {
            events = new array<Class>;
            m_events.Set(eventName, events);
            Print("[PartyMe][Event] New event created: " + eventName + " with instance of: " + instToAdd);
        }
        else
        {
            events = m_events.Get(eventName);
        }
        events.Insert(instToAdd);
    }

    void RemoveEvent(string eventName, Class instToRemove)
    {
        ref array<Class> events = m_events.Get(eventName);

        if (!events) return;
        int indexToRemove = events.Find(instToRemove);

        if (indexToRemove != -1)
        {
            events.Remove(indexToRemove);
            if (!events.Count())
            {
                m_events.Remove(eventName);
            }
        }
    }

    /*
        Description:
            Call instances and add prefix "on" for events.
        Example:
            eventName given was "PlayerDied"
            the function will call: "OnPlayerDied"
    */
    void CallEvent(string eventName, PM_Event_Params params)
    {
        ref array<Class> events = m_events.Get(eventName);

        if (events)
        {
            for (int index = 0; index < events.Count(); index++)
            {
                Class inst = events.Get(index);
                if (inst)
                {
                    string funcName = "On" + eventName;
                    auto paramsConverted = new Param1<PM_Event_Params>(params);
                    GetGame().GameScript.CallFunctionParams(inst, funcName, NULL, paramsConverted);
                }
                else
                {
                    events.Remove(index);
                    index--;
                }
            }
        }
        else
        {
            Print("[PartyMe][Event] The " + eventName + " event tried to be called but doesn't exist.");
        }
    }
};

static ref PM_events g_PM_events;
static ref PM_events PM_GetEvents()
{
    if (!g_PM_events)
    {
        g_PM_events = new PM_events();
    }
    return g_PM_events;
}
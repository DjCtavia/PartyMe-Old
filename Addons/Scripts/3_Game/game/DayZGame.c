modded class DayZGame
{
    // ref array<ref eventTestPartyMe> m_testEvents;
    override void DeferredInit()
	{
		super.DeferredInit();

		PM_GetEvents();
	}
};
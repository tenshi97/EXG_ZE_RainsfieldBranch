enum struct RoundData {
	bool Infected;
	//Handle Timer;
}

RoundData Round;

/*void RoundOnMapStart() {
	Round.Timer = INVALID_HANDLE;
}*/

void RoundOnRoundPreStart() {
	Round.Infected = false;
}

/*void RoundOnRoundFreezeEnd() {
	If(Round.Timer!=INVALID_HANDLE)	KillTimer(Round.Timer);
	Round.Timer = CreateTimer(ConVarFind("zr_infect_spawntime_min").FloatValue, RoundInfectCallback, TIMER_FLAG_NO_MAPCHANGE);
}*/

void RoundOnInfected() {
	Round.Infected = true;
}

/*Action RoundInfectCallback(Handle timer) {
	Round.Timer = INVALID_HANDLE;
	RoundOnInfected();
	return Plugin_Handled;
}*/

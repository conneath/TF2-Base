//====== Copyright � 1996-2006, Valve Corporation, All rights reserved. =======
//
// Purpose: Teamplay game rules that manage a round based structure for you
//
//=============================================================================

#ifndef TEAMPLAYROUNDBASED_GAMERULES_H
#define TEAMPLAYROUNDBASED_GAMERULES_H
#ifdef _WIN32
#pragma once
#endif

#include "teamplay_gamerules.h"
#include "teamplay_round_timer.h"

#ifdef GAME_DLL
	extern ConVar mp_respawnwavetime;
	extern ConVar mp_showroundtransitions;
	extern ConVar mp_enableroundwaittime;
	extern ConVar mp_showcleanedupents;
	extern ConVar mp_bonusroundtime;
	extern ConVar mp_restartround;
	extern ConVar mp_winlimit;
	extern ConVar mp_stalemate_timelimit;
	extern ConVar mp_stalemate_enable;
#else
	#define CTeamplayRoundBasedRules C_TeamplayRoundBasedRules
	#define CTeamplayRoundBasedRulesProxy C_TeamplayRoundBasedRulesProxy
#endif

class CTeamplayRoundBasedRules;

//-----------------------------------------------------------------------------
// Round states
//-----------------------------------------------------------------------------
enum gamerules_roundstate_t
{
	// initialize the game, create teams
	GR_STATE_INIT = 0,

	//Before players have joined the game. Periodically checks to see if enough players are ready
	//to start a game. Also reverts to this when there are no active players
	GR_STATE_PREGAME,

	//The game is about to start, wait a bit and spawn everyone
	GR_STATE_STARTGAME,

	//All players are respawned, frozen in place
	GR_STATE_PREROUND,

	//Round is on, playing normally
	GR_STATE_RND_RUNNING,

	//Someone has won the round
	GR_STATE_TEAM_WIN,

	//Noone has won, manually restart the game, reset scores
	GR_STATE_RESTART,

	//Noone has won, restart the game
	GR_STATE_STALEMATE,

	//Game is over, showing the scoreboard etc
	GR_STATE_GAME_OVER,

	GR_NUM_ROUND_STATES
};

enum {
	WINREASON_NONE =0,
	WINREASON_ALL_POINTS_CAPTURED,
	WINREASON_OPPONENTS_DEAD,
	WINREASON_FLAG_CAPTURE_LIMIT,
	WINREASON_DEFEND_UNTIL_TIME_LIMIT,
	WINREASON_STALEMATE,
};

enum stalemate_reasons_t
{
	STALEMATE_JOIN_MID,
	STALEMATE_TIMER,
	STALEMATE_SERVER_TIMELIMIT,

	NUM_STALEMATE_REASONS,
};

//-----------------------------------------------------------------------------
// Purpose: Per-state data
//-----------------------------------------------------------------------------
class CGameRulesRoundStateInfo
{
public:
	gamerules_roundstate_t	m_iRoundState;
	const char				*m_pStateName;

	void (CTeamplayRoundBasedRules::*pfnEnterState)();	// Init and deinit the state.
	void (CTeamplayRoundBasedRules::*pfnLeaveState)();
	void (CTeamplayRoundBasedRules::*pfnThink)();	// Do a PreThink() in this state.
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTeamplayRoundBasedRulesProxy : public CGameRulesProxy
{
public:
	DECLARE_CLASS( CTeamplayRoundBasedRulesProxy, CGameRulesProxy );
	DECLARE_NETWORKCLASS();

#ifdef GAME_DLL
	DECLARE_DATADESC();
	void	InputSetStalemateOnTimelimit( inputdata_t &inputdata );
#endif

	//----------------------------------------------------------------------------------
	// Client specific
#ifdef CLIENT_DLL
	void			OnPreDataChanged( DataUpdateType_t updateType );
	void			OnDataChanged( DataUpdateType_t updateType );
#endif // CLIENT_DLL
};

//-----------------------------------------------------------------------------
// Purpose: Teamplay game rules that manage a round based structure for you
//-----------------------------------------------------------------------------
class CTeamplayRoundBasedRules : public CTeamplayRules
{
	DECLARE_CLASS( CTeamplayRoundBasedRules, CTeamplayRules );
public:
	CTeamplayRoundBasedRules();

#ifdef CLIENT_DLL
	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.

	void SetRoundState( int iRoundState );
	float m_flLastRoundStateChangeTime;
#else
	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.
#endif

	// Data accessors
	inline gamerules_roundstate_t State_Get( void ) { return m_iRoundState; }
	bool	IsInWaitingForPlayers( void ) { return m_bInWaitingForPlayers; }
	virtual bool InRoundRestart( void ) { return State_Get() == GR_STATE_PREROUND; }
	bool	InStalemate( void ) { return State_Get() == GR_STATE_STALEMATE; }
	bool	RoundHasBeenWon( void ) { return State_Get() == GR_STATE_TEAM_WIN; }

	float	GetNextRespawnWave( int iTeam, CBasePlayer *pPlayer );
	virtual bool HasPassedMinRespawnTime( CBasePlayer *pPlayer );
	float	GetRespawnTimeScalar( int iTeam );
	float	GetRespawnWaveMaxLength( int iTeam, bool bScaleWithNumPlayers = true );
	float	GetMinTimeWhenPlayerMaySpawn( CBasePlayer *pPlayer );

	// Return false if players aren't allowed to cap points at this time (i.e. in WaitingForPlayers)
	virtual bool PointsMayBeCaptured( void ) { return ((State_Get() == GR_STATE_RND_RUNNING || State_Get() == GR_STATE_STALEMATE) && !IsInWaitingForPlayers()); }

	virtual int GetWinningTeam( void ){ return m_iWinningTeam; }
	int GetWinReason() { return m_iWinReason; }

	bool InOvertime( void ){ return m_bInOvertime; }
	void SetOvertime( bool bOvertime );

	bool InSetup( void ){ return m_bInSetup; }
	void SetSetup( bool bSetup );

	void		BalanceTeams( bool bRequireSwitcheesToBeDead );

	bool		SwitchedTeamsThisRound( void ) { return m_bSwitchedTeamsThisRound; }

	//----------------------------------------------------------------------------------
	// Server specific
#ifdef GAME_DLL
	// Derived game rules class should override these
public:
	// Override this to prevent removal of game specific entities that need to persist
	virtual bool	RoundCleanupShouldIgnore( CBaseEntity *pEnt );
	virtual bool	ShouldCreateEntity( const char *pszClassName );

	// Called when a new round is being initialized
	virtual void	SetupOnRoundStart( void ) { return; }

	// Called when a new round is off and running
	virtual void	SetupOnRoundRunning( void ) { return; }

	// Called before a new round is started (so the previous round can end)
	virtual void	PreviousRoundEnd( void ) { return; }

	// Send the team scores down to the client
	virtual void	SendTeamScoresEvent( void ) { return; }

	// Send the end of round info displayed in the win panel
	virtual void	SendWinPanelInfo( void ) { return; }

	// Setup spawn points for the current round before it starts
	virtual void	SetupSpawnPointsForRound( void ) { return; }

	// Called when a round has entered stalemate mode (timer has run out)
	virtual void	SetupOnStalemateStart( void ) { return; }
	virtual void	SetupOnStalemateEnd( void ) { return; }

	bool PrevRoundWasWaitingForPlayers() { return m_bPrevRoundWasWaitingForPlayers; }

	virtual bool ShouldScorePerRound( void ){ return true; }

	bool CheckNextLevelCvar( void );

public:
	void State_Transition( gamerules_roundstate_t newState );

	void RespawnPlayers( bool bForceRespawn, bool bTeam = false, int iTeam = TEAM_UNASSIGNED );

	void SetForceMapReset( bool reset );

	void SetRoundToPlayNext( string_t strName ){ m_iszRoundToPlayNext = strName; }
	string_t GetRoundToPlayNext( void ){ return m_iszRoundToPlayNext; }
	void AddPlayedRound( string_t strName );
	bool IsPreviouslyPlayedRound ( string_t strName );
	string_t GetLastPlayedRound( void );

	virtual void SetWinningTeam( int team, int iWinReason, bool bForceMapReset = true, bool bSwitchTeams = false, bool bDontAddScore = false, bool bFinal = false ) OVERRIDE;
	virtual void SetStalemate( int iReason, bool bForceMapReset = true, bool bSwitchTeams = false );

	virtual void SetRoundOverlayDetails( void ){ return; }

	virtual float GetWaitingForPlayersTime( void ) { return mp_waitingforplayers_time.GetFloat(); }
	void ShouldResetScores( bool bResetTeam, bool bResetPlayer ){ m_bResetTeamScores = bResetTeam; m_bResetPlayerScores = bResetPlayer; }
	void ShouldResetRoundsPlayed( bool bResetRoundsPlayed ){ m_bResetRoundsPlayed = bResetRoundsPlayed; }

	void SetFirstRoundPlayed( string_t strName ){ m_iszFirstRoundPlayed = strName ; }
	string_t GetFirstRoundPlayed(){ return m_iszFirstRoundPlayed; }

	void SetTeamRespawnWaveTime( int iTeam, float flValue );
	void AddTeamRespawnWaveTime( int iTeam, float flValue );
	virtual void FillOutTeamplayRoundWinEvent( IGameEvent *event ) {}	// derived classes may implement to add fields to this event

	void SetStalemateOnTimelimit( bool bStalemate ) { m_bAllowStalemateAtTimelimit = bStalemate; }

	bool IsGameUnderTimeLimit( void );

	CTeamRoundTimer* GetActiveRoundTimer( void );

protected:
	virtual void Think( void );

	virtual void CheckChatText( CBasePlayer *pPlayer, char *pText );
	void		 CheckChatForReadySignal( CBasePlayer *pPlayer, const char *chatmsg );

	// Game beginning / end handling
	virtual void GoToIntermission( void );
	void		 SetInWaitingForPlayers( bool bWaitingForPlayers );
	void		 CheckWaitingForPlayers( void );
	void		 CheckRestartRound( void );
	bool		 CheckTimeLimit( void );
	int			 GetTimeLeft( void );
	bool		 CheckWinLimit( void );
	bool		 CheckMaxRounds( void );

	virtual bool CanChangelevelBecauseOfTimeLimit( void ) { return true; }
	virtual bool CanGoToStalemate( void ) { return true; }
	
	// State machine handling
	void State_Enter( gamerules_roundstate_t newState );	// Initialize the new state.
	void State_Leave();										// Cleanup the previous state.
	void State_Think();										// Update the current state.
	static CGameRulesRoundStateInfo* State_LookupInfo( gamerules_roundstate_t state );	// Find the state info for the specified state.

	// State Functions
	void State_Enter_INIT( void );
	void State_Think_INIT( void );

	void State_Enter_PREGAME( void );
	void State_Think_PREGAME( void );

	void State_Enter_STARTGAME( void );
	void State_Think_STARTGAME( void );

	void State_Enter_PREROUND( void );
	void State_Think_PREROUND( void );

	void State_Enter_RND_RUNNING( void );
	void State_Think_RND_RUNNING( void );

	void State_Enter_TEAM_WIN( void );
	void State_Think_TEAM_WIN( void );

	void State_Enter_RESTART( void );
	void State_Think_RESTART( void );

	void State_Enter_STALEMATE( void );
	void State_Think_STALEMATE( void );
	void State_Leave_STALEMATE( void );

protected:
	virtual void InitTeams( void );
	int	 CountActivePlayers( void );

	virtual void RoundRespawn( void );
	virtual void CleanUpMap( void );
	void CheckRespawnWaves( void );
	void ResetScores( void );
	void ResetMapTime( void );

	void PlayStartRoundVoice( void );
	void PlayWinSong( int team );
	void PlayStalemateSong( void );
	void PlaySuddenDeathSong( void );
	void BroadcastSound( int iTeam, const char *sound );

	inline void RespawnTeam( int iTeam ) { RespawnPlayers( false, true, iTeam ); }

	void HideActiveTimer( void );
	void RestoreActiveTimer( void );

	virtual void InternalHandleTeamWin( int iWinningTeam ){ return; }

protected:
	CGameRulesRoundStateInfo	*m_pCurStateInfo;			// Per-state data 
	float						m_flStateTransitionTime;	// Timer for round states

	float						m_flWaitingForPlayersTimeEnds;
	CHandle<CTeamRoundTimer>	m_hWaitingForPlayersTimer;

	float						m_flNextPeriodicThink;
	bool						m_bChangeLevelOnRoundEnd;

	bool						m_bResetTeamScores;
	bool						m_bResetPlayerScores;
	bool						m_bResetRoundsPlayed;

	// Stalemate
	EHANDLE						m_hPreviousActiveTimer;
	CHandle<CTeamRoundTimer>	m_hStalemateTimer;
	float						m_flStalemateStartTime;

	bool						m_bForceMapReset; // should the map be reset when a team wins and the round is restarted?
	bool						m_bPrevRoundWasWaitingForPlayers;	// was the previous map reset after a waiting for players period

	bool						m_bTeamReady[ MAX_TEAMS ];
	bool						m_bInitialSpawn;

	string_t					m_iszRoundToPlayNext;
	CUtlVector<string_t>		m_iszPreviousRounds; // we'll store the two previous rounds so we won't play them again right away if there are other rounds that can be played first
	string_t					m_iszFirstRoundPlayed; // store the first round played after a full restart so we can pick a different one next time if we have other options

	float						m_flOriginalTeamRespawnWaveTime[ MAX_TEAMS ];

	bool						m_bAllowStalemateAtTimelimit;
	bool						m_bChangelevelAfterStalemate;

	float						m_flRoundStartTime;		// time the current round started

	int							m_nRoundsPlayed;
#endif
	// End server specific
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	// Client specific
#ifdef CLIENT_DLL
public:
	virtual void	OnPreDataChanged( DataUpdateType_t updateType );
	virtual void	OnDataChanged( DataUpdateType_t updateType );
	virtual void	HandleOvertimeBegin(){}

private:
	bool			m_bOldInWaitingForPlayers;
	bool			m_bOldInOvertime;
	bool			m_bOldInSetup;
#endif // CLIENT_DLL

public:
	bool WouldChangeUnbalanceTeams( int iNewTeam, int iCurrentTeam  );
	bool AreTeamsUnbalanced( int &iHeaviestTeam, int &iLightestTeam );

protected:
	CNetworkVar( gamerules_roundstate_t, m_iRoundState );
	CNetworkVar( bool, m_bInOvertime ); // Are we currently in overtime?
	CNetworkVar( bool, m_bInSetup ); // Are we currently in setup?
	CNetworkVar( bool, m_bSwitchedTeamsThisRound );

protected:
	CNetworkVar( int,			m_iWinningTeam );				// Set before entering GR_STATE_TEAM_WIN
	CNetworkVar( int,			m_iWinReason );
	CNetworkVar( bool,			m_bInWaitingForPlayers );
	CNetworkVar( bool,			m_bAwaitingReadyRestart );
	CNetworkVar( float,			m_flRestartRoundTime );
	CNetworkVar( float,			m_flMapResetTime );						// Time that the map was reset
	CNetworkArray( float,		m_flNextRespawnWave, MAX_TEAMS );		// Minor waste, but cleaner code

public:
	CNetworkArray( float,		m_TeamRespawnWaveTimes, MAX_TEAMS );	// Time between each team's respawn wave

private:
	float m_flStartBalancingTeamsAt;
	float m_flNextBalanceTeamsTime;
	bool m_bPrintedUnbalanceWarning;
	float m_flFoundUnbalancedTeamsTime;
};

// Utility function
bool FindInList( const char **pStrings, const char *pToFind );

inline CTeamplayRoundBasedRules* TeamplayRoundBasedRules()
{
	return static_cast<CTeamplayRoundBasedRules*>(g_pGameRules);
}

#endif // TEAMPLAYROUNDBASED_GAMERULES_H

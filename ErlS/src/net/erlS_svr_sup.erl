%%%-------------------------------------------------------------------
%%% @author Administrator
%%% @copyright (C) 2016, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 21. 一月 2016 10:08
%%%-------------------------------------------------------------------
-module(erlS_svr_sup).
-author("Administrator").

-behaviour(supervisor).

%% API
-export([start_link/0,start_link/1,start_link/2]).

%% Supervisor callbacks
-export([init/1]).

-define(SERVER, ?MODULE).

%%%===================================================================
%%% API functions
%%%===================================================================

%%--------------------------------------------------------------------
%% @doc
%% Starts the supervisor
%%
%% @end
%%--------------------------------------------------------------------
-spec(start_link() ->
  {ok, Pid :: pid()} | ignore | {error, Reason :: term()}).
start_link() ->
  supervisor:start_link({local, ?SERVER}, ?MODULE, []).

start_link(Param) ->
  supervisor:start_link({local, ?SERVER}, ?MODULE, Param).


start_link(Name, Param) ->
  supervisor:start_link({local, Name}, ?MODULE, Param).

%%%===================================================================
%%% Supervisor callbacks
%%%===================================================================

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Whenever a supervisor is started using supervisor:start_link/[2,3],
%% this function is called by the new process to find out about
%% restart strategy, maximum restart frequency and child
%% specifications.
%%
%% @end
%%--------------------------------------------------------------------
-spec(init(Args :: term()) ->
  {ok, {SupFlags :: {RestartStrategy :: supervisor:strategy(),
    MaxR :: non_neg_integer(), MaxT :: non_neg_integer()},
    [ChildSpec :: supervisor:child_spec()]
  }} |
  ignore |
  {error, Reason :: term()}).
init(Param) ->
  logger:info("start erlS_sup(~p)...", [Param]),

  ListenerSup = {
    erlS_listener_sup,
    {erlS_listener_sup, start_link, [Param]},
    permanent,
    infinity,
    supervisor,
    [erlS_listener_sup]
  },


  SessionSup = {
    erlS_session_sup,
    {erlS_session_sup, start_link, []},
    permanent,
    infinity,
    supervisor,
    [erlS_session_sup]
  },


  {ok,
    {
      {one_for_one, 1, 10},
      [ListenerSup, SessionSup]
    }
  }.

%%%===================================================================
%%% Internal functions
%%%===================================================================

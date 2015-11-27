%%%-------------------------------------------------------------------
%%% @author Administrator
%%% @copyright (C) 2016, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 14. 一月 2016 19:43
%%%-------------------------------------------------------------------
-module(erlS_listener_sup).
-author("Administrator").

-behaviour(supervisor).

%% API
-export([start_link/0]).

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
init([]) ->

  Listener = {
    erlSListener,
    {erlS_listener, start_link, []},
    transient,
    100,
    worker,
    [erlS_listener]
  },

  AcceptorSup = {
    acceptorSup,
    {erlS_acceptor_sup, start_link, []},
    permanent,
    inifinity,
    supervisor,
    [erlS_acceptor_sup]
  },

  SessionSup = {
    erlS_session_sup,
    {erlS_session_sup, start_link, []},
    permanent,
    inifinity,
    supervisor,
    [erlS_session_sup]
  },

  {ok,
    {
      {one_for_all, 1, 10},
      [Listener,AcceptorSup, SessionSup]
    }
  }.

%%%===================================================================
%%% Internal functions
%%%===================================================================

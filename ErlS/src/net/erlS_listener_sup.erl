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
-export([start_link/0, start_link/1]).

%% Supervisor callbacks
-export([init/1]).

-define(SERVER, ?MODULE).
-include("common.hrl").

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
init(Port) ->
%%  ?DEV("will start listen at ~p", [Port]),
  AcceptorSup = {
    erlS_acceptor_sup,
    {erlS_acceptor_sup, start_link, []},
    % transient, infinity, supervisor,[erlS_acceptor_sup]
    temporary, infinity, supervisor,[erlS_acceptor_sup]
  },

  Listener = {
    erlS_listener,
    {erlS_listener, start_link, [Port]},
    % transient,100,worker,[erlS_listener]
    temporary, 16#ffffffff, worker, [erlS_listener]
  },


  {ok,
    {
      {one_for_all, 10, 10},
      [AcceptorSup, Listener]
    }
  }.

%%%===================================================================
%%% Internal functions
%%%===================================================================

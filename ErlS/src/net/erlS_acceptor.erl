%%%-------------------------------------------------------------------
%%% @author Administrator
%%% @copyright (C) 2016, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 14. 一月 2016 19:45
%%%-------------------------------------------------------------------
-module(erlS_acceptor).
-author("Administrator").

-behaviour(gen_server).
-include("common.hrl").
%% API
-export([start_link/0,start_link/1]).

%% gen_server callbacks
-export([init/1,
  handle_call/3,
  handle_cast/2,
  handle_info/2,
  terminate/2,
  code_change/3]).

-define(SERVER, ?MODULE).

-record(state, {listen_socket, ref}).

%%%===================================================================
%%% API
%%%===================================================================

%%--------------------------------------------------------------------
%% @doc
%% Starts the server
%%
%% @end
%%--------------------------------------------------------------------
-spec(start_link() ->
  {ok, Pid :: pid()} | ignore | {error, Reason :: term()}).
start_link() ->
  gen_server:start_link(?MODULE, ?MODULE, [], []).


start_link(LSock) ->
  gen_server:start_link(?MODULE, LSock, []).

%%%===================================================================
%%% gen_server callbacks
%%%===================================================================

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Initializes the server
%%
%% @spec init(Args) -> {ok, State} |
%%                     {ok, State, Timeout} |
%%                     ignore |
%%                     {stop, Reason}
%% @end
%%--------------------------------------------------------------------
-spec(init(Args :: term()) ->
  {ok, State :: #state{}} | {ok, State :: #state{}, timeout() | hibernate} |
  {stop, Reason :: term()} | ignore).
init(LSock) ->
  erlang:process_flag(trap_exit, true),
  logger:info("acceptor start ~p, sock=~p", [self(), LSock]),
  {ok, #state{listen_socket=LSock}}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Handling call messages
%%
%% @end
%%--------------------------------------------------------------------
-spec(handle_call(Request :: term(), From :: {pid(), Tag :: term()},
    State :: #state{}) ->
  {reply, Reply :: term(), NewState :: #state{}} |
  {reply, Reply :: term(), NewState :: #state{}, timeout() | hibernate} |
  {noreply, NewState :: #state{}} |
  {noreply, NewState :: #state{}, timeout() | hibernate} |
  {stop, Reason :: term(), Reply :: term(), NewState :: #state{}} |
  {stop, Reason :: term(), NewState :: #state{}}).
handle_call(_Request, _From, State) ->
  {reply, ok, State}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Handling cast messages
%%
%% @end
%%--------------------------------------------------------------------
-spec(handle_cast(Request :: term(), State :: #state{}) ->
  {noreply, NewState :: #state{}} |
  {noreply, NewState :: #state{}, timeout() | hibernate} |
  {stop, Reason :: term(), NewState :: #state{}}).
handle_cast(_Request, State) ->
  {noreply, State}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Handling all non call/cast messages
%%
%% @spec handle_info(Info, State) -> {noreply, State} |
%%                                   {noreply, State, Timeout} |
%%                                   {stop, Reason, State}
%% @end
%%--------------------------------------------------------------------
-spec(handle_info(Info :: timeout() | term(), State :: #state{}) ->
  {noreply, NewState :: #state{}} |
  {noreply, NewState :: #state{}, timeout() | hibernate} |
  {stop, Reason :: term(), NewState :: #state{}}).


handle_info({inet_async, LSock, Ref, {ok, Sock}},
            State = #state{listen_socket=LSock, ref=Ref}) ->

    accept_one(LSock, Sock),

    %% accept more
    accept(State);

handle_info({inet_async, LSock, Ref, {error, Reason}},
            State=#state{listen_socket=LSock, ref=Ref}) ->
  logger:error("acceptor error reason=~p", [Reason]),
    case Reason of
        closed       -> {stop, normal, State}; %% listening socket closed
        econnaborted -> accept(State); %% client sent RST before we accepted
        _            -> {stop, {accept_failed, Reason}, State}
    end;
handle_info({start_accept_now}, State) ->
     accept(State);

handle_info(_Info, State) ->
    logger:error("acceptor undeal info=~p", [_Info]),
    {noreply, State}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% This function is called by a gen_server when it is about to
%% terminate. It should be the opposite of Module:init/1 and do any
%% necessary cleaning up. When it returns, the gen_server terminates
%% with Reason. The return value is ignored.
%%
%% @spec terminate(Reason, State) -> void()
%% @end
%%--------------------------------------------------------------------
-spec(terminate(Reason :: (normal | shutdown | {shutdown, term()} | term()),
    State :: #state{}) -> term()).
terminate(_Reason, _State) ->
  ok.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Convert process state when code is changed
%%
%% @spec code_change(OldVsn, State, Extra) -> {ok, NewState}
%% @end
%%--------------------------------------------------------------------
-spec(code_change(OldVsn :: term() | {down, term()}, State :: #state{},
    Extra :: term()) ->
  {ok, NewState :: #state{}} | {error, Reason :: term()}).
code_change(_OldVsn, State, _Extra) ->
  {ok, State}.

%%%===================================================================
%%% Internal functions
%%%===================================================================

accept(State = #state{listen_socket=LSock}) ->
  case catch prim_inet:async_accept(LSock, -1) of
    {ok, Ref} ->
      {noreply, State#state{ref=Ref}};
    Error ->
      logger:error("async_accept error:~p, LSock=~p", [Error, LSock]),
      self() ! {start_accept_now},
      {noreply, State}
  end.

accept_one(LSock, Sock) ->
  try
    {ok, Mod} = inet_db:lookup_socket(LSock),
    inet_db:register_socket(Sock, Mod),
    %% report
    {ok, {Address, Port}} = inet:sockname(Sock),
    {ok, {PeerAddress, PeerPort}} = inet:peername(Sock),

    logger:info("accept new sock=~p,~p:~p | ~p:~p", [Sock, utils:ip_to_str(Address), Port, utils:ip_to_str(PeerAddress), PeerPort]),

    spawn_socket_controller(Sock)
  catch Error:Reason ->
    logger:error("accept_one error=~p:~p", [Error,Reason]),
    gen_tcp:close(Sock)
  end.


spawn_socket_controller(ClientSock) ->
  try
  logger:info("sock_controller sock=~p ...", [ClientSock]),
  case supervisor:start_child(erlS_session_sup, [ClientSock]) of
      {ok, SPid} ->
        inet:setopts(ClientSock, ?TCP_C_OPTS),
        case catch gen_tcp:controlling_process(ClientSock, SPid) of
          MSG -> logger:info("controlling_process sock=~p, pid=~p, res=~p ", [ClientSock,SPid,MSG])
        end,
        SPid ! {start_recv_now};
      {error, closed} -> %% 这种无聊的人 尝试来连我们的，就不管啦
        pass;
      Other -> logger:error("socket_controller sock=~p, error=~p ", [ClientSock,Other])
    end
  catch
     _ : _  -> ok
  end .


%%  case gen_tcp:recv(ClientSock, 500, 30000) of
%%    {ok, Bin} ->
%%      case supervisor:start_child(mgeeg_tcp_client_sup, [ClientSock, Line, Bin]) of
%%        {ok, CPid} ->
%%          inet:setopts(ClientSock, [binary, {packet, 0}, {active, false}, {nodelay, true}, {delay_send, false}]),
%%          gen_tcp:controlling_process(ClientSock, CPid),
%%          CPid ! start;
%%        {error, Error} ->
%%          ?CRITICAL_MSG("cannt accept client:~w", [Error]),
%%            catch gen_tcp:close(ClientSock)
%%      end;
%%    {error, closed} -> %% 这种无聊的人 尝试来连我们的，就不管啦
%%      pass;
%%    Other ->
%%      case catch inet:peername(ClientSock) of
%%        {ok, {PeerAddress, PeerPort}} ->
%%          mgeew_liushi_server:stat_liushi(common_tool:ip_to_str(PeerAddress), ?LOGIN_ACCEPT_LIUSHI),
%%          ?ERROR_MSG("recv packet error:~w", [{PeerAddress, PeerPort, Other}]);
%%        _ ->
%%          ?ERROR_MSG("recv packet error:~w", [Other])
%%      end,
%%        catch gen_tcp:close(ClientSock)
%%  end.
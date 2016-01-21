%%%-------------------------------------------------------------------
%%% @author Administrator
%%% @copyright (C) 2016, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 17. 一月 2016 21:19
%%%-------------------------------------------------------------------
-module(erlS_session).
-author("Administrator").

-behaviour(gen_server).

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

-record(state, { socket, halfMsg }).

-include("common_define.hrl").

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
  gen_server:start_link(?MODULE, [], []).

start_link(ClientSock) ->
  gen_server:start_link(?MODULE, ClientSock, []).

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
init(ClientSock) ->
  log4erl:info("session process(~p) init, sock=~p", [self(), ClientSock]),
  set_half_msg(<<>>),
  {ok, #state{socket = ClientSock}}.

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

handle_info({start_recv_now}, #state{socket=Socket} = State) ->
  start_async_recv(Socket, -1),
  {noreply, State};

handle_info({send_msg, Bin}, #state{socket=Socket} = State) ->
  start_async_send(Socket, Bin),
  {noreply, State};

handle_info( {inet_async, Socket, _Ref, {ok, Data}}, #state{socket=Socket} = State) ->
  log4erl:debug("session socket recv=~p, sock=~p pid=~p", [Data, Socket, self()]),
  doMsg(Data),
  start_async_recv(Socket, -1),
  {noreply, State};

handle_info({inet_async, _Socket, _Ref, {error, closed}}, #state{socket=Socket} = State) ->
  log4erl:error("session socket close sock=~p, pid=~p", [Socket, self()]),
  {stop, normal, State};

handle_info({inet_async, _Socket, _Ref, {error, _Reason}}, #state{socket=Socket} = State) ->
  log4erl:error("session socket close sock=~p, pid=~p", [Socket, self()]),
   {stop, normal, State};

handle_info({inet_reply, _S, _Status}, #state{socket=Socket} = State) ->
  log4erl:debug("session socket inet_reply=~p, pid=~p, status=~p", [Socket, self(), State]),
   {noreply, State};

handle_info(_Info, #state{socket=Socket} = State) ->
  log4erl:error("session socket scok=~p, pid=~p, undealmsg=~p", [Socket, self(), _Info]),
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

set_half_msg(Data) -> put(halfMsg, Data).
get_half_msg() -> 	get(halfMsg).


start_async_recv(Socket, Len) ->

  case catch prim_inet:async_recv(Socket, 0, Len) of
    MSG -> log4erl:info("start async_recv ~p, ~p", [MSG, Socket])
  end.

start_async_send(Socket, Bin) -> erlang:port_command(Socket, Bin, [force]).


doMsg(<<>>) -> ok;
doMsg(Data) ->
  HalfMsg = get_half_msg(),
  _HalfMsgSize = erlang:byte_size(HalfMsg),
  _MsgSize = erlang:byte_size(Data),
  self() !  {send_msg, Data}.



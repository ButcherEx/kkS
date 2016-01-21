%%%-------------------------------------------------------------------
%%% @author Administrator
%%% @copyright (C) 2016, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 19. 一月 2016 19:38
%%%-------------------------------------------------------------------
-module(erlS_cc).
-author("Administrator").
-include("common_define.hrl").
%% API
-export([client/2, start/2, server/1]).

client(PortNo,Message) ->
  {ok,Sock} = gen_tcp:connect("127.0.0.1",PortNo,[binary, {packet, 0}, {active, false}]),
  ?DEV("connect ok", []),
  case catch gen_tcp:send(Sock,Message) of
     MSG -> ?DEV("send =~p", [MSG])
  end,
  A = gen_tcp:recv(Sock,0),
  gen_tcp:close(Sock),
  A.


start(Num,LPort) ->
  case gen_tcp:listen(LPort,[{active, false},{packet,2}]) of
    {ok, ListenSock} ->
      start_servers(Num,ListenSock),
      {ok, Port} = inet:port(ListenSock),
      Port;
    {error,Reason} ->
      {error,Reason}
  end.

server(LS) ->
  case catch prim_inet:async_accept(LS, -1) of
    {ok, Ref} ->
      io:format("accept returned ~w - goodbye!~n",[Ref]);
     Other ->
      io:format("accept error ~w - goodbye!~n",[Other])
  end.
%%  case gen_tcp:accept(LS) of
%%    {ok,S} ->
%%      io:format("Socket ~w accept [~w]~n",[S,self()]),
%%      loop(S),
%%      server(LS);
%%    Other ->
%%      io:format("accept returned ~w - goodbye!~n",[Other]),
%%      ok
%%  end.

start_servers(0,_) ->
  ok;
start_servers(Num,LS) ->
  spawn(?MODULE,server,[LS]),
  start_servers(Num-1,LS).


loop(S) ->
  inet:setopts(S,[{active,once}]),
  receive
    {tcp,S,Data} ->
%%      Answer = process(Data), % Not implemented in this example
      gen_tcp:send(S,Data),
      loop(S);
    {tcp_closed,S} ->
      io:format("Socket ~w closed [~w]~n",[S,self()]),
      ok
  end.
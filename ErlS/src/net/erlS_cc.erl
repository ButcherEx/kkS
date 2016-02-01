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
-include("common.hrl").
%% API
-export([client/2, client/3,loop_client/1, loop_client/2]).

client(PortNo,Message) ->
  {ok,Sock} = gen_tcp:connect("127.0.0.1",PortNo,[binary, {packet, 0}, {active, false}]),
  ?DEV("connect ok", []),
  case catch gen_tcp:send(Sock,Message) of
     MSG -> ?DEV("send =~p", [MSG])
  end,
  loop(Sock),
  % {ok, Data} = gen_tcp:recv(Sock,0),
  % gen_tcp:send(Sock,Data),
  % {ok, Data} = gen_tcp:recv(Sock,0),
  % gen_tcp:close(Sock),
  ok.

client(N,PortNo,Message)->
  start_client(N,PortNo,Message),
  ok.

start_client(0, _PortNo, _Message) ->
  ok;
start_client(Num,PortNo,Message) ->
  ?DEV("start_client =~p", [Num]),
  case catch spawn(?MODULE,loop_client,[PortNo,Message]) of
    MSG -> ?DEV("spawn =~p", [MSG])
  end,
  start_client(Num-1,PortNo,Message).

loop_client(Param) ->
  ?DEV("connect ok ~p", [Param])
  .
loop_client(PortNo,Message) ->
  {ok,Sock} = gen_tcp:connect("127.0.0.1",PortNo,[binary, {packet, 0}, {active, false}]),
  ?DEV("connect ok", []),
  case catch gen_tcp:send(Sock,Message) of
     MSG -> ?DEV("send =~p", [MSG])
  end,
  loop(Sock).

loop(Sock) ->
    case catch gen_tcp:recv(Sock,0) of
      {ok,Data} ->
        %%  ?DEV("recv ~p", [Data]),
        gen_tcp:send(Sock,Data), loop(Sock);
      _ -> ok
    end.  


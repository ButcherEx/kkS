%%%-------------------------------------------------------------------
%%% @author Administrator
%%% @copyright (C) 2016, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 22. 一月 2016 17:20
%%%-------------------------------------------------------------------
-module(net_user).
-author("Administrator").
-export([onMsg/2,flowControl/0]).

%%%===================================================================
%%% Internal functions
%%%===================================================================
onMsg(CMD, Pk) ->
  logger:debug("recv(~p,~p)",[CMD, Pk]),
  ok.


flowControl() ->
  true.
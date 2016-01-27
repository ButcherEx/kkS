%%%-------------------------------------------------------------------
%%% @author Administrator
%%% @copyright (C) 2016, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 27. 一月 2016 11:24
%%%-------------------------------------------------------------------
-module(svr).
-author("Administrator").

-export([start/0,
  stop/0]).

%%
%% API Functions
%%
%% 启动应用
start() ->
  case application:start(erlS) of
    ok ->
      ok;
    Msg ->
      {failur, Msg}
  end.

%% 关闭应用
stop() ->
  try
    logger:info("svr:stop.....")
  catch
    _:_Why ->
      logger:error("svr:stop exception, reason=~p,bt=~p",[_Why, erlang:get_stacktrace()])
  end,

  timer:sleep(20000),     %%等待2000毫秒
  case application:stop(erlS) of
    ok ->
      logger:info("svr stopped");
    Msg ->
      logger:error("svr stop not ok,msg=~p",[Msg])
  end,
  erlang:halt().
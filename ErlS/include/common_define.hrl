%%%-------------------------------------------------------------------
%%% @author Administrator
%%% @copyright (C) 2016, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 19. 一月 2016 15:25
%%%-------------------------------------------------------------------
-author("Administrator").

-define(DEV(Format, Args), io:format(Format++"~n",Args)).

-define(TCP_L_OPTS, [
  binary,
  {packet, 0},
  {reuseaddr, true},
  {nodelay, true},
  {delay_send, true},
  {active, false},
  {backlog, 1024},
  {exit_on_close, false},
  {send_timeout, 15000}
]).

-define(TCP_C_OPTS,[
  binary,
  {packet, 0},
  {active, false},
  {nodelay, true},
  {delay_send, false},
  {send_timeout, 30000},
  {send_timeout_close, true},
  {exit_on_close, true}
]).


-define(OFFLINE_REASON_SOCK_CLOSE, 1).
-define(OFFLINE_REASON_SOCK_ERROR, 2).
-define(OFFLINE_REASON_SOCK_LOGOUT, 3).
-define(OFFLINE_REASON_SOCK_KICK, 4).
-define(OFFLINE_REASON_EXCEPTION, 4).
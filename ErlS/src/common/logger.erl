%%%-------------------------------------------------------------------
%%% @author Administrator
%%% @copyright (C) 2016, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 27. 一月 2016 10:52
%%%-------------------------------------------------------------------
-module(logger).
-author("Administrator").
-include("common.hrl").
%% API
-compile(export_all).

start()->
  case catch application:start(log4erl) of
    Msg -> ?DEV("log4erl started! ~p", [Msg])
  end,

  log4erl:conf("../priv/log4erl.conf"),
  log4erl:info("load log4erl.conf ok.").


log(Level, Log) ->
  log4erl:log(Level, Log).
log(Level, Log, Data) ->
  log4erl:log(Level, Log, Data).
log(Logger, Level, Log, Data) ->
  log4erl:log(Logger, Level, Log, Data).



warn(Log) ->
  log4erl:warn(Log).
%% If 1st parameter is atom, then it is Logger
warn(Logger, Log) when is_atom(Logger) ->
  log4erl:warn(Logger, Log);
warn(Log, Data) ->
  log4erl:warn(Log, Data).
warn(Logger, Log, Data) ->
  log4erl:warn(Logger, Log, Data).

info(Log) ->
  log4erl:info(Log).
info(Logger, Log) when is_atom(Logger) ->
  log4erl:info(Logger, Log);
info(Log, Data) ->
  log4erl:info(Log, Data).
info(Logger, Log, Data) ->
  log4erl:info(Logger, Log, Data).

error(Log) ->
  log4erl:error(error, Log).
error(Logger, Log) when is_atom(Logger) ->
  log4erl:error(Logger, error, Log, []);
error(Log, Data) ->
  log4erl:error(Log, Data).
error(Logger, Log, Data) ->
  log4erl:error(Logger, Log, Data).

fatal(Log) ->
  log4erl:fatal(Log).
fatal(Logger, Log) when is_atom(Logger) ->
  log4erl:fatal(Logger, Log);
fatal(Log, Data) ->
  log4erl:fatal(Log, Data).
fatal(Logger, Log, Data) ->
  log4erl:fatal(Logger, Log, Data).

debug(Log) ->
  log4erl:debug(Log).
debug(Logger, Log) when is_atom(Logger) ->
  log4erl:debug(Logger, Log);
debug(Log, Data) ->
  log4erl:debug(Log, Data).
debug(Logger, Log, Data) ->
  log4erl:debug(Logger, Log, Data).

-module(reload).

-export([rl/1, rr/1]).


rl(Module) ->
  io:format(" reload Module: ~p  ~n", [Module]),
  code:purge(Module),
  case code:get_object_code(Module) of
    {_Module, Binary, Filename} ->
      case code:load_binary(Module, Filename, Binary) of
        {module, Module} ->
          case erlang:function_exported(Module, reLoad, 1) of
            true ->
              io:format(" - Calling ~p:reLoad() ...", [Module]),
              case catch Module:reLoad([]) of
                ok ->
                  io:format(" ok.~n");
                Reason ->
                  io:format(" fail: ~p.~n", [Reason])
              end;
            false -> ok
          end,
          io:format("reload  ~p success   ~n", [Module]);
        {error, What} ->
          io:format("reload  ~p fail,reason:~p   ~n", [Module, What])
      end;
    error ->
      io:format(" get_object_code ~p fail  ~n", [Module])
  end.


rr(Param) ->
  [Node | LeftParm] = Param,
  [Module | _] = LeftParm,
  io:format(" Node: ~p, Module: ~p  ~n", [Node, Module]),
  case code:get_object_code(Module) of
    {_Module, Binary, Filename} ->
      case rpc:call(Node, code, load_binary, [Module, Filename, Binary]) of
        {module, Module} ->
          case erlang:function_exported(Module, reLoad, 1) of
            true ->
              io:format(" - Calling ~p:reLoad() ...", [Module]),
              case catch Module:reLoad([]) of
                ok ->
                  io:format(" ok.~n");
                Reason ->
                  io:format(" fail: ~p.~n", [Reason])
              end;
            false -> ok
          end,
          io:format("reload  ~p success   ~n", [Module]);
        {error, What} ->
          io:format("reload  ~p fail,reason:~p   ~n", [Module, What])
      end;
    error ->
      io:format(" get_object_code ~p fail  ~n", [Module])
  end,
  ok.



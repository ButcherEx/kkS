
-module(utils).

-compile(nowarn_unused_vars).
-compile(nowarn_unused_function).
-compile(nowarn_export_all).
-compile(export_all).


-include("common.hrl").
-include("error_code.hrl").



%% @doc get IP address string from Socket
ip(Socket) ->
    {ok, {IP, _Port}} = inet:peername(Socket),
    {Ip0,Ip1,Ip2,Ip3} = IP,
    list_to_binary(integer_to_list(Ip0)++"."++integer_to_list(Ip1)++"."++integer_to_list(Ip2)++"."++integer_to_list(Ip3)).


%% @doc quick sort
sort([]) ->
    [];
sort([H|T]) -> 
    sort([X||X<-T,X<H]) ++ [H] ++ sort([X||X<-T,X>=H]).

%% for
for(Max,Max,F)->[F(Max)];
for(I,Max,F)->[F(I)|for(I+1,Max,F)].


%% @doc convert float to string,  f2s(1.5678) -> 1.57
f2s(N) when is_integer(N) ->
    integer_to_list(N) ++ ".00";
f2s(F) when is_float(F) ->
    [A] = io_lib:format("~.2f", [F]),
    A.


%% @doc convert other type to atom
to_atom(Msg) when is_atom(Msg) -> 
    Msg;
to_atom(Msg) when is_binary(Msg) -> 
    utils:list_to_atom(binary_to_list(Msg));
to_atom(Msg) when is_list(Msg) -> 
    utils:list_to_atom(Msg);
to_atom(_) -> 
    throw(other_value).  %%list_to_atom("").

%% @doc convert other type to list
to_list(Msg) when is_list(Msg) -> 
    Msg;
to_list(Msg) when is_atom(Msg) -> 
    atom_to_list(Msg);
to_list(Msg) when is_binary(Msg) -> 
    binary_to_list(Msg);
to_list(Msg) when is_integer(Msg) -> 
    integer_to_list(Msg);
to_list(Msg) when is_float(Msg) -> 
    f2s(Msg);
to_list(_) ->
    throw(other_value).

%% @doc convert other type to binary
to_binary(Msg) when is_binary(Msg) -> 
    Msg;
to_binary(Msg) when is_atom(Msg) ->
    list_to_binary(atom_to_list(Msg));
%%atom_to_binary(Msg, utf8);
to_binary(Msg) when is_list(Msg) -> 
    list_to_binary(Msg);
to_binary(Msg) when is_integer(Msg) -> 
    list_to_binary(integer_to_list(Msg));
to_binary(Msg) when is_float(Msg) -> 
    list_to_binary(f2s(Msg));
to_binary(_Msg) ->
    throw(other_value).

%% @doc convert other type to float
to_float(Msg)->
    Msg2 = to_list(Msg),
    list_to_float(Msg2).

%% @doc convert other type to integer
-spec to_integer(Msg :: any()) -> integer().
to_integer(Msg) when is_integer(Msg) -> 
    Msg;
to_integer(Msg) when is_binary(Msg) ->
    Msg2 = binary_to_list(Msg),
    list_to_integer(Msg2);
to_integer(Msg) when is_list(Msg) -> 
    list_to_integer(Msg);
to_integer(Msg) when is_float(Msg) -> 
    round(Msg);
to_integer(_Msg) ->
    throw(other_value).

%% @doc convert other type to tuple
to_tuple(T) when is_tuple(T) -> T;
to_tuple(T) -> {T}.

%% @doc convert IP(tuple) to string()
ip_to_str(IP) ->
    case IP of
        {A, B, C, D} ->
            lists:concat([A, ".", B, ".", C, ".", D]);
        {A, B, C, D, E, F, G, H} ->
            lists:concat([A, ":", B, ":", C, ":", D, ":", E, ":", F, ":", G, ":", H]);
        Str when is_list(Str) ->
            Str;
        _ ->
            []
    end.

%% @doc get data type {0=integer,1=list,2=atom,3=binary}
get_type(DataValue,DataType)->
    case DataType of
        0 ->
            DataValue2 = binary_to_list(DataValue),
            list_to_integer(DataValue2);
        1 ->
            binary_to_list(DataValue);
        2 ->
            DataValue2 = binary_to_list(DataValue),
            utils:list_to_atom(DataValue2);
        3 -> 
            DataValue
    end.



%% @doc get random list
list_random(List)->
    case List of
        [] ->
            {};
        _ ->
            RS		=	lists:nth(random:uniform(length(List)), List),
            ListTail	= 	lists:delete(RS,List),
            {RS,ListTail}
    end.

%% @doc get a random integer between Min and Max
random(Min,Max)->
    Min2 = Min-1,
    random:uniform(Max-Min2)+Min2.

random_dice(Face,Times)->
    if
        Times == 1 ->
            random(1,Face);
        true ->
            lists:sum(for(1,Times, fun(_)-> random(1,Face) end))
    end.

odds(Numerator,Denominator)->
    Odds = random:uniform(Denominator),
    if
        Odds =< Numerator -> 
            true;
        true ->
            false
    end.
odds_list(List)->
    Sum = odds_list_sum(List),
    odds_list(List,Sum).
odds_list([{Id,Odds}|List],Sum)->
    case odds(Odds,Sum) of
        true ->
            Id;
        false ->
            odds_list(List,Sum-Odds)
    end.
odds_list_sum(List)->
    {_List1,List2} = lists:unzip(List),
    lists:sum(List2).


%% @doc get the minimum number that is bigger than X 
ceil(X) ->
    T = trunc(X),
    if 
        X - T == 0 ->
            T;
        true ->
            if
                X > 0 ->
                    T + 1;
                true ->
                    T
            end			
    end.


%% @doc get the maximum number that is smaller than X
floor(X) when X >= 0 ->
	trunc(X);
floor(X) ->
	T = trunc(X),
	case X of
		T ->
			T;
		_ ->
			T - 1
	end.

subatom(Atom,Len)->	
    utils:list_to_atom(lists:sublist(atom_to_list(Atom),Len)).

sleep(Msec) ->
    receive
    after Msec ->
            true
    end.

md5(S) ->        
    Md5_bin =  erlang:md5(S), 
    Md5_list = binary_to_list(Md5_bin), 
    lists:flatten(list_to_hex(Md5_list)). 

list_to_hex(L) -> 
    lists:map(fun(X) -> int_to_hex(X) end, L). 

int_to_hex(N) when N < 256 -> 
    [hex(N div 16), hex(N rem 16)]. 
hex(N) when N < 10 -> 
    $0+N; 
hex(N) when N >= 10, N < 16 ->      
    $a + (N-10).

list_to_atom(List) when is_list(List) ->
    case catch(list_to_existing_atom(List)) of
        {'EXIT', _} -> erlang:list_to_atom(List);
        Atom when is_atom(Atom) -> Atom
    end.

combine_lists(L1, L2) ->
    Rtn = 
	lists:foldl(
          fun(T, Acc) ->
                  case lists:member(T, Acc) of
                      true ->
                          Acc;
                      false ->
                          [T|Acc]
                  end
          end, L1, L2),
    Rtn.

get_process_info_and_zero_value(InfoName) ->
    PList = erlang:processes(),
    ZList = lists:filter( 
              fun(T) -> 
                      case erlang:process_info(T, InfoName) of 
                          {InfoName, 0} -> false; 
                          _ -> true 	
                      end
              end, PList ),
    ZZList = lists:map( 
               fun(T) -> {T, erlang:process_info(T, InfoName), erlang:process_info(T, registered_name)} 
               end, ZList ),
    [ length(PList), InfoName, length(ZZList), ZZList ].

get_memory_pids(Memory) ->
    PList = erlang:processes(),
    lists:filter( 
      fun(T) -> 
              case erlang:process_info(T, memory) of 
                  {_, VV} -> 
                      if VV >  Memory -> true;
                         true -> false
                      end;
                  _ -> true 	
              end
      end, PList ).

gc(Memory) ->
    lists:foreach(
      fun(PID) ->
              erlang:garbage_collect(PID)
      end, get_memory_pids(Memory)).

gc_nodes(Memory) ->
    lists:foreach(
      fun(Node) ->
              lists:foreach(
                fun(PID) ->
                        rpc:call(Node, erlang, garbage_collect, [PID])
                end, rpc:call(Node, utils, get_memory_pids, [Memory]))
      end, [node() | nodes()]).

get_process_info_and_large_than_value(InfoName, Value) ->
    PList = erlang:processes(),
    ZList = lists:filter( 
              fun(T) -> 
                      case erlang:process_info(T, InfoName) of 
                          {InfoName, VV} -> 
                              if VV >  Value -> true;
                                 true -> false
                              end;
                          _ -> true 	
                      end
              end, PList ),
    ZZList = lists:map( 
               fun(T) -> {T, erlang:process_info(T, InfoName), erlang:process_info(T, registered_name)} 
               end, ZList ),
    [ length(PList), InfoName, Value, length(ZZList), ZZList ].


get_msg_queue() ->
  get_process_info_and_zero_value(message_queue_len).

get_memory() ->
  get_process_info_and_large_than_value(memory, 1048576).

get_memory(Value) ->
  get_process_info_and_large_than_value(memory, Value).


get_heap() ->
  get_process_info_and_large_than_value(heap_size, 1048576).

get_heap(Value) ->
  get_process_info_and_large_than_value(heap_size, Value).

get_processes() ->
  get_process_info_and_large_than_value(memory, 0).

url_encode([H|T]) -> 
     if
          H >= $a, $z >= H -> 
	[H|url_encode(T)];
         H >= $A, $Z >= H ->
             [H|url_encode(T)];
         H >= $0, $9 >= H ->
             [H|url_encode(T)];
         H == $_; H == $.; H == $-; H == $/; H == $: -> % FIXME: more..
             [H|url_encode(T)];
         true ->
             case yaws:integer_to_hex(H) of
                 [X, Y] ->
                     [$%, X, Y | url_encode(T)];
                 [X] ->
                     [$%, $0, X | url_encode(T)]
             end
      end;
 
url_encode([]) ->
     [].





%% 格式化资源
get_format_lang_resources(LangResources,ParamList)
  when erlang:is_list(ParamList)->
    lists:flatten(io_lib:format(LangResources,[utils:to_list(PR)|| PR <- ParamList]));
get_format_lang_resources(LangResources,Param) ->
    lists:flatten(io_lib:format(LangResources,[utils:to_list(Param)])).


today(H,M,S) ->
    A = calendar:datetime_to_gregorian_seconds({common_time:date(),{H,M,S}}),
    B = calendar:datetime_to_gregorian_seconds({{1970,1,1}, {8,0,0}}),
    A-B.

%% return list()
get_intranet_address() ->
    Result = os:cmd("ifconfig -a | grep 'inet ' | grep '192.168.' | awk '{print $2}' | cut -d ':' -f 2 | grep -v '^127'"),
    string:tokens(Result, "\n").

get_all_bind_address() ->
    Result = os:cmd("/sbin/ifconfig | grep 'inet addr' | awk '{print $2}'|cut -d':' -f2"),
    string:tokens(Result, "\n").
    

utf8_len(List) when erlang:is_list(List) ->
    len(List, 0);
utf8_len(Binary) when erlang:is_binary(Binary) ->
    len(erlang:binary_to_list(Binary), 0).
    
    
len([], N) ->
    N;
len([A, _, _, _, _, _ | T], N) when A =:= 252 orelse A =:= 253 ->
    len(T, N+1);
len([A, _, _, _, _ | T], N) when A >=248 andalso A =< 251 ->
    len(T, N+1);
len([A, _, _, _ |T], N) when A >= 240 andalso A =< 247 ->
    len(T, N+1);
len([A, _, _ | T], N) when A >= 224 ->
    len(T, N+1);
len([A, _ | T], N) when A >= 192 ->
    len(T, N+1);
len([_A | T], N) ->
    len(T, N+1).



sublist_utf8(List, Start, Length) when erlang:is_list(List) ->
    sublist_utf8_2(List, Start, Start + Length - 1, 0, []);
sublist_utf8(Binary, Start, Length) when erlang:is_binary(Binary) ->
    sublist_utf8_2(erlang:binary_to_list(Binary), Start, Start + Length - 1, 0, []).

sublist_utf8_2(List, Start, End, Cur, Result) ->
    if Cur =:= End ->
            lists:reverse(Result);
       true ->
            sublist_utf8_3(List, Start, End, Cur, Result)
    end.

sublist_utf8_3([], _Start, _End, _Cur, Result) ->
    lists:reverse(Result);
sublist_utf8_3([A, A2, A3, A4, A5, A6 | T], Start, End, Cur, Result) when A =:= 252 orelse A =:= 253 ->
  Result2 =  case  Cur + 1 >= Start of
               true ->  [A6, A5, A4, A3, A2, A | Result];
               _ -> Result
             end,
  sublist_utf8_2(T, Start, End, Cur+1, Result2);
sublist_utf8_3([A, A2, A3, A4, A5 | T], Start, End, Cur, Result) when A >= 248 andalso A =< 251 ->
  Result2 =  case  Cur + 1 >= Start of
               true ->  [A5, A4, A3, A2, A | Result];
               _ -> Result
             end,
  sublist_utf8_2(T, Start, End, Cur+1, Result2);
sublist_utf8_3([A, A2, A3, A4 | T], Start, End, Cur, Result) when A >= 240 andalso A =< 247 ->
  Result2 =  case  Cur + 1 >= Start of
               true ->  [A4, A3, A2, A | Result];
               _ -> Result
             end,
  sublist_utf8_2(T, Start, End, Cur+1, Result2);
sublist_utf8_3([A, A2, A3 | T], Start, End, Cur, Result) when A >= 224 ->
  Result2 =  case  Cur + 1 >= Start of
               true ->  [A3, A2, A | Result];
               _ -> Result
             end,
  sublist_utf8_2(T, Start, End, Cur+1, Result2);
sublist_utf8_3([A, A2 | T], Start, End, Cur, Result) when A >= 192 ->
  Result2 =  case  Cur + 1 >= Start of
               true ->  [A2, A | Result];
               _ -> Result
             end,
  sublist_utf8_2(T, Start, End, Cur+1, Result2);
sublist_utf8_3([A | T], Start, End, Cur, Result) ->
  Result2 =  case  Cur + 1 >= Start of
               true ->  [A | Result];
               _ -> Result
             end,
  sublist_utf8_2(T, Start, End, Cur+1, Result2).


%% 随机重排列表
shuffle_list(L) ->
	random:seed(erlang:now()),
	List1 = [{random:uniform(), X} || X <- L],
	List2 = lists:keysort(1, List1),
	[E || {_, E} <- List2].

shuffle_list(L,N) ->
	random:seed(erlang:now()),
	List1 = [{random:uniform(), X} || X <- L],
	List2 = lists:keysort(1, List1),
	lists:sublist([E || {_, E} <- List2],N).

%% 取平均数
%% 没做异常处理
average(L) ->
	F = fun(E,{Sum,Len}) ->
		{E + Sum,Len + 1}
	end,
	
	{Sum,Len} = lists:foldl(F,{0,0},L),

	Sum / Len.

%% 执行公式
expr_formula(Formula, TupList) ->
	Tokens = 
    case erl_scan:string(Formula) of
        {ok, TTokens, _Endline} ->
            TTokens;
        ScanError ->
            logger:error("~ts, Error=~w",["解析公式出错",ScanError]),
            erlang:throw({error, ?_LANG_PARSE_FORMULA_ERROR})
    end,
    Exprlist = 
    case erl_parse:parse_exprs(Tokens) of
        {ok, TExprlist} ->
            TExprlist;
        {error, ParseError} ->
          logger:error("~ts,Error=~w",["解析公式出错2",ParseError]),
            erlang:throw({error, ?_LANG_PARSE_FORMULA_ERROR})
    end,
    BindingsT = erl_eval:new_bindings(),
	Bindings = lists:foldl(fun({Key, Val}, Acc) -> erl_eval:add_binding(Key, Val, Acc) end, BindingsT, TupList),
    {_, Value, _} = erl_eval:exprs(Exprlist, Bindings),
	Value.

%% 获取进程名字
get_process_name_by_pid(Pid) ->
	case catch(erlang:process_info(Pid, registered_name)) of
	{'EXIT', _} ->
		case ets:select(global_names, [{{'_', Pid, '_', '_', '_'}, [], ['$_']}])	of
		[] ->
			Pid;
		[{Name, _, _, _, _}] ->
			Name;
		_ ->
			Pid
		end;
	[] ->
		case ets:select(global_names, [{{'_', Pid, '_', '_', '_'}, [], ['$_']}])of
		[] ->
			Pid;
		[{Name, _, _, _, _}] ->
			Name;
		_ ->
			Pid
		end;
	{_, Name} ->
		Name;
	_ ->
		Pid
	end.

list_replace_elem(Elem, N, List) when is_list(List) andalso is_integer(N) andalso N > 0 ->
    case length(List) of
        Len when N =:= 1 andalso Len =:= 0 ->
            [Elem];
        _Len when N =:= 1 ->
            [_H|T] = List,
            [Elem|T];
        Len when N =:= Len + 1 ->
            List ++ [Elem];
        Len when N =< Len ->
            {ListH, [_H|ListT]} = lists:split(N-1, List),
            ListH ++ [Elem|ListT];
        true ->
            Msg = utils:get_format_lang_resources("list_replace_elem(Elem=~s, N=~s, List=~s) can not process",[Elem, N, List]),
            throw(Msg)
    end;

list_replace_elem(Elem, N, List) ->
    Msg = utils:get_format_lang_resources("list_replace_elem(Elem=~s, N=~s, List=~s) can not process",[Elem, N, List]),
    throw(Msg).

rand(L0) ->
	Sum = lists:foldl(fun({NR, _}, Acc) -> NR + Acc end, 0, L0),
	Rd = utils:random(1, Sum),
	odds_func(Rd, L0).
odds_func(Rd, [{R, V} | _]) when Rd =< R ->
	V;
odds_func(Rd, [{R, _} | T]) ->
	odds_func(Rd - R, T).

loop_function(Result, []) ->
	{ok, Result};
loop_function(State, [F|T]) ->
	case F(State) of
		{break, Reason} ->
			{break, Reason};
		{ok, Result} ->
			loop_function(Result, T)
	end.

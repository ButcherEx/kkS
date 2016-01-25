%%%网络消息处理的基础模块
-compile(nowarn_unused_vars).
-module(netmsg).

%% ====================================================================
%% API functions
%% ====================================================================
-export([
	binary_read_bool/1,
	binary_read_int8/1,
	binary_read_int16/1,
	binary_read_int32/1,
	binary_read_int/1,
	binary_read_int64/1,
	binary_read_uint8/1,
	binary_read_uint16/1,
	binary_read_uint32/1,
	binary_read_uint/1,
	binary_read_uint64/1,
	binary_read_float/1,
	binary_read_double/1,
	binary_read_string/1,
	binary_read_binary/1,
	binary_read_array/2
]).

-export([
	binary_write_bool/1,
	binary_write_int8/1,
	binary_write_int16/1,
	binary_write_int32/1,
	binary_write_int/1,
	binary_write_int64/1,
	binary_write_uint8/1,
	binary_write_uint16/1,
	binary_write_uint32/1,
	binary_write_uint/1,
	binary_write_uint64/1,
	binary_write_float/1,
	binary_write_double/1,
	binary_write_string/1,
	binary_write_binary/1,
	binary_write_array/2,
	sendPackage/3
]).

-include("type.hrl").

%%==========================================
%% 以下几个函数为基本的网络二进制读取函数
%%==========================================

-spec binary_read_bool(Bin) -> {boolean(), Remain} when
	Bin :: binary(), Remain :: binary().
binary_read_bool(Bin) when erlang:is_binary(Bin) ->
	<<C:?S8, Left/binary>> = Bin,
	case C > 0 of
		true ->
			{true, Left};
		_ ->
			{false, Left}
	end.

%binary_read_int8(Bin) -> {Char,LeftBin}
%@spec读取Bin中的一个Char
-spec binary_read_int8(Bin) -> {int8(), Remain} when
	Bin :: binary(), Remain :: binary().
binary_read_int8(Bin) when erlang:is_binary(Bin) ->
	<<C:?S8, Left/binary>> = Bin,
	{C, Left}.

%binary_read_int16(Bin) -> {Short,LeftBin}
%@spec读取Bin中的一个Short
-spec binary_read_int16(Bin :: binary()) -> {int16(), Left} when
	Left :: binary().
binary_read_int16(Bin) when erlang:is_binary(Bin) ->
	<<C:?S16, Left/binary>> = Bin,
	{C, Left}.

%binary_read_int32(Bin) -> {Int,LeftBin}
%@spec读取Bin中的一个Int
-spec binary_read_int32(Bin :: binary()) -> {C, Left} when
	C :: int32(), Left :: binary().
binary_read_int32(Bin) when erlang:is_binary(Bin) ->
	<<C:?S32, Left/binary>> = Bin,
	{C, Left}.

-spec binary_read_int(Bin :: binary()) -> {C, Left} when
	C :: int(), Left :: binary().
binary_read_int(Bin) when erlang:is_binary(Bin) ->
	<<C:?S32, Left/binary>> = Bin,
	{C, Left}.

-spec binary_read_int64(Bin :: binary()) -> {C, Left} when
	C :: int64(), Left :: binary().
binary_read_int64(Bin) when erlang:is_binary(Bin) ->
	<<C:?S64, Left/binary>> = Bin,
	{C, Left}.

-spec binary_read_uint8(Bin :: binary()) -> {C, Left} when
	C :: uint8(), Left :: binary().
binary_read_uint8(Bin) when erlang:is_binary(Bin) ->
	<<C:?U8, Left/binary>> = Bin,
	{C, Left}.

-spec binary_read_uint16(Bin :: binary()) -> {C, Left} when
	C :: uint16(), Left :: binary().
binary_read_uint16(Bin) when erlang:is_binary(Bin) ->
	<<C:?U16, Left/binary>> = Bin,
	{C, Left}.

-spec binary_read_uint32(Bin :: binary()) -> {C, Left} when
	C :: uint32(), Left :: binary().
binary_read_uint32(Bin) when erlang:is_binary(Bin) ->
	<<C:?U32, Left/binary>> = Bin,
	{C, Left}.

-spec binary_read_uint(Bin :: binary()) -> {C, Left} when
	C :: uint(), Left :: binary().
binary_read_uint(Bin) when erlang:is_binary(Bin) ->
	<<C:?U32, Left/binary>> = Bin,
	{C, Left}.

-spec binary_read_uint64(Bin :: binary()) -> {C, Left} when
	C :: uint64(), Left :: binary().
binary_read_uint64(Bin) when erlang:is_binary(Bin) ->
	<<C:?U64, Left/binary>> = Bin,
	{C, Left}.

%binary_read_float(Bin) -> {Float,LeftBin}
%@spec读取Bin中的单精度浮点数
-spec binary_read_float(Bin :: binary()) -> {C, Left} when
	C :: float(), Left :: binary().
binary_read_float(Bin) when erlang:is_binary(Bin) ->
	<<C:?F32, Left/binary>> = Bin,
	{C, Left}.

%binary_read_double(Bin) -> {Double,LeftBin}
%@spec读取Bin中的双精度浮点数
-spec binary_read_double(Bin :: binary()) -> {C, Left} when
	C :: float(), Left :: binary().
binary_read_double(Bin) when erlang:is_binary(Bin) ->
	<<C:?F64, Left/binary>> = Bin,
	{C, Left}.

%@spec binary_read_string(Bin) -> {String,LeftBin} | {[],LeftBin} | {[],Bin}
%从Bin中提取字符串，成功返回{String,LeftBin},失败返回{[],LeftBin}
%如果Bin中包括的字符串长度超过Bin本身的长度，则返回{[],Bin}
-spec binary_read_string(Bin) -> {string(), Remain} when
	Bin :: binary(), Remain :: binary().
binary_read_string(Bin) when erlang:is_binary(Bin) ->
	<<Len:16/little, Left/binary>> = Bin,
	Size = erlang:byte_size(Left),
	if
		Len =< Size ->
			case Len of
				0 ->
					{[], Left};
				_ ->
					{Str, LeftData} = split_binary(Left, Len),
					{binary_to_list(Str), LeftData}
			end;
		true ->
			{[], Bin}
	end.

%@spec binary_read_binary(Bin) -> {Binary,LeftBin} | {<<>>,LeftBin} | {<<>>,Bin}
%从Bin中提取二进制，成功返回{Binary,LeftBin},失败返回{<<>>,LeftBin}
%如果Bin中包括的二进制长度超过Bin本身的长度，则返回{<<>>,Bin}
-spec binary_read_binary(Bin) -> {binary(), Remain} when
	Bin :: binary(), Remain :: binary().
binary_read_binary(Bin) when erlang:is_binary(Bin) ->
	<<Len:16/little, Left/binary>> = Bin,
	Size = erlang:byte_size(Left),
	if
		Len =< Size ->
			case Len of
				0 -> {<<>>, Left};
				_ -> split_binary(Left, Len)
			end;
		true -> {<<>>, Bin}
	end.

%%读取bin数据中读取一个数组
-spec binary_read_array(Bin, Fun) -> {list(), Remain} when
	Bin :: binary(), Fun :: function(), Remain :: binary().
binary_read_array(Bin, Fun) when erlang:is_binary(Bin) andalso erlang:is_function(Fun, 1) ->
	<<Len:16/little, Bin1/binary>> = Bin,
	{Data, Bin2} = binary_read_n(Len, Bin1, Fun),
	{Data, Bin2}.

%%==========================================
%% 以上几个函数为基本的网络二进制读取函数
%%==========================================


%%==========================================
%% 以下几个函数为基本的网络二进制写入函数
%%==========================================
binary_write_bool(1) ->
	<<1:?S8>>;
binary_write_bool(0) ->
	<<0:?S8>>;
binary_write_bool(true) ->
	<<1:?S8>>;
binary_write_bool(false) ->
	<<0:?S8>>.

binary_write_int8(Data) ->
	<<Data:?S8>>.

binary_write_int16(Data) ->
	<<Data:?S16>>.

binary_write_int(Data) ->
	<<Data:?S32>>.

binary_write_int32(Data) ->
	<<Data:?S32>>.

binary_write_int64(Data) ->
	<<Data:?S64>>.

binary_write_uint8(Data) ->
	<<Data:?U8>>.

binary_write_uint16(Data) ->
	<<Data:?U16>>.

binary_write_uint(Data) ->
	<<Data:?U32>>.

binary_write_uint32(Data) ->
	<<Data:?U32>>.

binary_write_uint64(Data) ->
	<<Data:?U64>>.

binary_write_float(Data) ->
	<<Data:?F32>>.

binary_write_double(Data) ->
	<<Data:?F64>>.

binary_write_string(Data) ->
	Bin = list_to_binary(Data),
	Len = erlang:byte_size(Bin),
	<<Len:16/little, Bin/binary>>.

binary_write_binary(Data) ->
	Len = erlang:byte_size(Data),
	<<Len:16/little, Data/binary>>.

-spec binary_write_array(Data, Fun) -> binary() when
	Data :: list(), Fun :: function().
binary_write_array(Data, Fun) when erlang:is_function(Fun, 1) ->
	Len = erlang:length(Data),
	Bin2 = binary_write_array_data(Data, Fun),
	<<Len:16/little, Bin2/binary>>.

-spec sendPackage(Socket, Cmd, Bin) -> any() when
	Socket :: port(), Cmd :: non_neg_integer(), Bin :: binary().
sendPackage(Socket, Cmd, Bin) ->
	Len = byte_size(Bin) + 4,
	Bin1 = <<Len:16/little, Cmd:16/little, Bin/binary>>,
	case gen_tcp:send(Socket, Bin1) of
		ok ->
			ok;
		{error, Reason} ->
			throw({'sendErrorBySocket', Reason})
	end.
%%==========================================
%% 以上几个函数为基本的网络二进制写入函数
%%==========================================

%% ====================================================================
%% Internal functions
%% ====================================================================
%%读取bin数据中用户自定义的N个数据项
-spec binary_read_n(N, Bin, Fun) -> {list(), Remain} when
	N :: uint(), Bin :: binary(), Fun :: function(), Remain :: binary().
binary_read_n(0, Bin, _) ->
	{[], Bin};
binary_read_n(N, Bin, Fun) ->
	{Data1, Bin1} = Fun(Bin),
	{Data2, Bin2} = binary_read_n(N - 1, Bin1, Fun),
	{[Data1] ++ Data2, Bin2}.

-spec binary_write_array_data(Data, Fun) -> binary() when
	Data :: list(), Fun :: function().
binary_write_array_data([], _) ->
	<<>>;
binary_write_array_data(Data, Fun) ->
	[Data1|Data2] = Data,
	Bin1 = Fun(Data1),
	Bin2 = binary_write_array_data(Data2, Fun),
	<<Bin1/binary, Bin2/binary>>.


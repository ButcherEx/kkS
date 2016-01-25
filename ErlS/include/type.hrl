-ifndef(Type_hrl).
-define(Type_hrl, 1).

%% -define(INT16, 16 / signed - little - integer).
%% -define(INT8, 8 / signed - little - integer).
%% -define(INT, 32 / signed - little - integer).
%% -define(INT64, 64 / signed - little - integer).
%% -define(UINT, 32 / unsigned - little - integer).
%%
-define(BYTE, 8 / unsigned - little - integer).
-define(WORD, 16 / unsigned - little - integer).
-define(DWORD, 32 / unsigned - little - integer).
-define(QWORD, 64 / unsigned - little - integer).

-define(Char, 8 / signed - little - integer).
-define(Short, 16 / signed - little - integer).
-define(Int, 32 / signed - little - integer).
-define(Int64, 64 / signed - little - integer).

-define(U8, 8 / unsigned - little - integer).
-define(U16, 16 / unsigned - little - integer).
-define(U32, 32 / unsigned - little - integer).
-define(U64, 64 / unsigned - little - integer).

-define(S8, 8 / signed - little - integer).
-define(S16, 16 / signed - little - integer).
-define(S32, 32 / signed - little - integer).
-define(S64, 64 / signed - little - integer).

-define(F32, 32 / little - float).
-define(F64, 64 / little - float).

-type id() :: non_neg_integer().
-type uint() :: non_neg_integer().
-type int() :: integer().
-type etsTab() :: integer() | atom().
-type bool01() :: 0 | 1.

-type uint8() :: 0 .. 255.
-type uint16() :: 0 .. 65535.
-type uint32() :: 0 .. 4294967295.
-type uint64() :: 0 .. 18446744073709551615.

-type int8() :: -128 .. 127.
-type int16() :: -32768 .. 32767.
-type int32() :: -2147483648 .. 2147483647.
-type int64() :: -9223372036854775808 .. 9223372036854775807.

-type skillId() :: uint16().
-type itemId() :: uint16().
-type monsterId() :: uint16().
-type mapId() :: uint16().


-endif.

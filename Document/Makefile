VCProject=Server.vcxproj

TARGET=./../../Bin/TLServer

CC=g++

CPPFLAGS=-D__LINUX__ -g -O2\
	-D__Server__\
	-DNDEUG\
	-D_CRT_SECURE_NO_WARNINGS\
	-I./\
	-I../../Common \
	-I./Main

LINKFLAGS=../lib/libRecast.a \
	-lstdc++ \
	-lm \
	-lpthread 

OBJS=$(shell cat $(VCProject) | grep 'ClCompile Include=' | sed 's/<ClCompile Include="//g'|sed's/\\/\//g'|sed 's/cpp" \/>/o /g')

define Compler_O
$(1):$(subst .o,.cpp,$(cf)) -o $(cf))
	$(CC) $(CPPFLAGS) $(subst .o,.cpp, $$(1)) -c -o $$@
enddef

$(foreach cf,$(OBJS),$(call Compler_O, $($cf)))

all:$(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) $(LINKFLAGS) -o $@


clean:
	rm -rf $(OBJS)




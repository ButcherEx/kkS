#ifndef __PACKET_DEFINE_H__
#define __PACKET_DEFINE_H__


namespace Packets
{
	//消息类型值描述格式：PACKET_XX_YYYYYY
	//XX可以描述为：GC、CG、CL、LC、GL、LG
	//G游戏服务器端、C客户端、L登陆服务器端、S服务器端程序（可以是登陆，也可以是游戏）
	//YYYYYY表示消息内容
	//例如：PACKET_CG_ATTACK 表示客户端发给游戏服务器端关于攻击的消息
	enum PACKET_DEFINE
	{
		PACKET_CL_ASKCHARLIST = 450,									//客户端请求Login登录
		PACKET_MAX													//消息类型的最大值
	};
};


#endif


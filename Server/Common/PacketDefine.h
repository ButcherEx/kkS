#ifndef __PACKET_DEFINE_H__
#define __PACKET_DEFINE_H__


namespace Packets
{
	//��Ϣ����ֵ������ʽ��PACKET_XX_YYYYYY
	//XX��������Ϊ��GC��CG��CL��LC��GL��LG
	//G��Ϸ�������ˡ�C�ͻ��ˡ�L��½�������ˡ�S�������˳��򣨿����ǵ�½��Ҳ��������Ϸ��
	//YYYYYY��ʾ��Ϣ����
	//���磺PACKET_CG_ATTACK ��ʾ�ͻ��˷�����Ϸ�������˹��ڹ�������Ϣ
	enum PACKET_DEFINE
	{
		PACKET_CL_ASKCHARLIST = 450,									//�ͻ�������Login��¼
		PACKET_MAX													//��Ϣ���͵����ֵ
	};
};


#endif


#include "Player.h"
#include "Role.h"

Role::Role(Player& rPlayer)
:m_rPlayer(rPlayer)
{

}

uint32_t Role::Handle(const CG_LOGIN& rMsg)
{

	_MY_TRY
	{

		::CG_LOGIN login;
		login.set_vtype(1);
		login.set_gameversion(1);
		login.set_programversion(1);
		login.set_publicresourceversion(1);
		login.set_maxpacketid(99);
		login.set_forceenter(8);
		login.set_deviceid("ma.CY5929");
		login.set_devicetype("WINDOWS");
		login.set_deviceversion("Win8.1");
		int32 byteSize  = login.ByteSize();
		CHAR Buf[256] = {0};
		login.SerializeToArray(Buf,sizeof Buf);
		printf("%s\n", Buf);

		Packet* pPak = new Packets::CG_LOGIN_PAK();
		(*pPak).msg_ref.ParseFromArray(Buf, byteSize);


		Packets::CG_LOGIN_PAK loginPak;
		loginPak.msg_ref.ParseFromArray(Buf, byteSize);

		bool isEqual = loginPak.msg_ref.vtype() == login.vtype();
		Assert(isEqual);
	}
	_MY_CATCH
	{

	}

	return 0;
}
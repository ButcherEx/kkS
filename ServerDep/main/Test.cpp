//////////////////////////////////////////////////////////////////////////
void boost_xml()
{
	_MY_TRY
	{
		boost::property_tree::ptree pt, root;
		boost::property_tree::xml_parser::read_xml("./plugin.xml", pt);
		root = pt.get_child("ProductList");
		std::string attr = pt.get<std::string>("ProductList.plugin.<xmlattr>.name");
		for(boost::property_tree::ptree::iterator iter = root.begin();
			iter != root.end(); iter++)
		{
			boost::property_tree::ptree plugin = iter->second;
			std::string name = plugin.get<std::string>("<xmlattr>.name");
			std::string status = plugin.get<std::string>("<xmlattr>.status");
			std::string nickname = plugin.get<std::string>("<xmlattr>.nickname");
			boost::property_tree::ptree plugin_ch;
			plugin_ch = plugin.get_child("PID");
			std::string pid = plugin_ch.get_value("-1");
			std::string pid2 = plugin_ch.data();
			plugin_ch = plugin.get_child("MD5");
			std::string md5 = plugin_ch.data();
			plugin_ch = plugin.get_child("PATH");
			std::string dir = plugin_ch.data();


			int j = 0;
			j = 0;



		}
	}
	_MY_CATCH
	{

	}
}
//////////////////////////////////////////////////////////////////////////
void boost_ini()
{
	_MY_TRY
	{
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini("../config.ini", pt);
		std::string ip = pt.get<std::string>("ServerList.ip", "localhost");

		std::string logLevel = pt.get<std::string>("Log.loglevel", "10");
	}
	_MY_CATCH
	{

	}
	
}
//////////////////////////////////////////////////////////////////////////
void container_test()
{
	typedef TVector<int32_t>  IntVector;
	typedef IntVector::iterator IntVectorIter;
	IntVector vec;

	_MY_TRY
	{
		for (int32_t i = 0; i < 10; i++)
			vec.push_back(rand());

		vec[1] = 0xdeadbeef;
		vec[10] = 9000;


		TArray<int32_t, 10> IntArray;
		for (int32_t i = 0; i < 10; i++)
			IntArray[i] = rand();


		IntArray[1] = 0xdeadbeef;
		IntArray[10] = 9000;
	}
	_MY_CATCH
	{

	}
}
//////////////////////////////////////////////////////////////////////////
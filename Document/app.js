var util = require('util')
var url = require('url');
var http = require('http'); 
var EventEmitter = require('events').EventEmitter;

function DataBase() {
	this.name = "mg";
	this.host = "localhost";
	this.port = 3306;
}

util.inherits(DataBase, EventEmitter);

DataBase.prototype.select = function(res, sql) {
	res.write("<h2>"+sql+"</h2>");
	res.end(); 
}

DataBase.prototype.del = function(res, sql) {
	res.write("<h2>"+sql+"</h2>");
	res.end(); 
}

DataBase.prototype.start = function( ){
	var source = this;
	function selectData(res, sql) {
		source.select(res, sql);
	}
	
	source.on("select", selectData);
	
	function deleteData(res, sql) {
		source.del(res,sql);
	}
	source.on("delete", deleteData);
	
	function hello(res) {
		res.end("<h2>Hello world!</h2>"); 
	}
	source.on("hello", hello);
}

var globalDB = new DataBase();
globalDB.start();

http.createServer(function(req, res) { 
	res.writeHead(200, {'Content-Type': 'text/html'}); 
	var params = url.parse(req.url, true).query;
	console.log(req.url);
	console.log(params.zzl);
	switch(req.url)
	{
		case "/":
		globalDB.emit("hello", res);
		break;
		case "/del":
		globalDB.emit("select", res, "delete * from t");
		break;
		case "/sel":
		globalDB.emit("delete", res, "select * from t");
		break;
		default:
		break;
	};
}).listen(3000); 

console.log("HTTP server is listening at port 3000."); 
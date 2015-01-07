
module.exports = exports = dbcache;

var util = require('util');
var memcached = require('memcached');
var mysql = require('mysql');

function dbcache( ) {
	this.memcached_client = null;
	this.mysql_connection = null;
	this.memconfig = {};
	this.mysqlconfig = {};
}

dbcache.prototype.init = function(mem, mysqlconfig) {
	this.memconfig = mem;
	this.mysqlconfig = mysqlconfig;
//	this.memcached = new memcached(this.memconfig);
	this.mysql_connection = mysql.createConnection(this.mysqlconfig);
	this.mysql_connection.set
};

dbcache.prototype.query = function(sql, callback) {
	this.mysql_connection.query(sql,function (err, results, fields) {  
	    callback(err, results);
	});
	
};

dbcache.prototype.insert = function(sql, callback) {
    this.mysql_connection.query(sql, function (err, results) {  
        callback(err, results);
    });  
};

dbcache.prototype.delete = function(sql, callback) {
	this.mysql_connection.query(sql, function (err, results) {  
		callback(err, results);
    });  
};

dbcache.prototype.update = function(sql, callback) {
	this.mysql_connection.query(sql, function (err, results) {  
		callback(err, results);
	});  
};



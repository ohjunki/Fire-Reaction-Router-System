var express = require('express');
var exec = require('child_process').exec;
var router = express.Router();
global.ap = {};
global.mode = 0;
/* GET home page. */
router.get('/mode', function(req, res, next) {
        if( 'mode' in req.query ){
                if( global.mode != req.query.mode ){
                        if( req.query.mode == "0" ){
							console.log( "\n\n\n@@@@@   Change to General Mode @@@@\n\n\n");
                                //exec('../general.sh' );
                        }else if( req.query.mode == "1" ){
							console.log( "\n\n\n@@@@@Change to Fire Mode\n\n\n" );
                                //exec('../mode_fire.sh' );
                        }
                }
                global.mode = req.query.mode;
                console.log( global.mode );
                res.send({ 'status' : 200, 'mode' : global.mode  } );
        }else{
                res.send({ 'status' : 400, 'message' : 'no mode query' } );
        }

});

/* GET home page. */
router.get('/update', function(req, res, next) {
	if( 'id' in req.query ){
		var data = {
			'id' : req.query.id,
			'location' : req.query.location,
			'level' : req.query.level,
			'temp' : req.query.temp,
			'hum' : req.query.hum,
			'flame' : req.query.flame,
			'gastwo' : req.query.gastwo,
			'gasseven' : req.query.gasseven
		};
		global.ap[''+req.query.id]=data;
		res.send( { 'status' : 200, 'data' : data } );
	}else{
		res.send( { 'status' : 400, 'message' : 'no id' } );
	}
});
router.get('/list', function(req, res, next) {
	res.send( { 'status' : 200, 'data' : { 'list':global.ap,'mode':global.mode} } );
});
module.exports = router;
